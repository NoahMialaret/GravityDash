#include "Entity.h"

Entity::Entity(sf::Texture *tex, int maxID)
{
  sprite.setTexture(*tex);
  sprite.setOrigin(CENTRED_ORIGIN);
  std::uniform_int_distribution id(0, maxID - 1);
  entID = id(Utility::rng);
  // std::cout << "ID is: " << entID << '\n';
}

bool Entity::EndOfLife() const
{
  return endOfLife;
}

void Entity::Freeze()
{
  isFrozen = true;
}

void Entity::Unfreeze()
{
  isFrozen = false;
}

void Entity::Render(sf::RenderWindow *win) const
{
  //Utility::shaderTest.setUniform("texture", sprite.getTexture());
  Utility::entShad.setUniform("colorID", entID);
  win->draw(sprite, &Utility::entShad);
}

bool Entity::operator<=(Entity& rhs)
{
  return this->sprite.getPosition().y <= rhs.sprite.getPosition().y;
}

bool Entity::operator<(Entity& rhs)
{
  return this->sprite.getPosition().y < rhs.sprite.getPosition().y;
}

bool Entity::operator>=(Entity& rhs)
{
  return this->sprite.getPosition().y >= rhs.sprite.getPosition().y;
}

bool Entity::operator>(Entity& rhs)
{
  return this->sprite.getPosition().y > rhs.sprite.getPosition().y;
}

bool Entity::operator<=(float rhs)
{
  return this->sprite.getPosition().y <= rhs;
}

bool Entity::operator<(float rhs)
{
  return this->sprite.getPosition().y < rhs;
}

bool Entity::operator>=(float rhs)
{
  return this->sprite.getPosition().y >= rhs;
}

bool Entity::operator>(float rhs)
{
  return this->sprite.getPosition().y > rhs;
}

// = --------- =
// = Saw Class =
// = --------- =

Saw::Saw(sf::Texture* tex, sf::IntRect &worldBorder, int maxID)
  :
  Entity(tex, maxID)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution dist(0, 1);
  int isOnTop = dist(Utility::rng);
  int isGoingRight = dist(Utility::rng);

  vel = 1.0f * (isGoingRight ? Utility::gameScale : -Utility::gameScale);

  float xPos = isGoingRight ? worldBorder.left - SCALED_DIM : worldBorder.left + worldBorder.width;
  float yPos = isOnTop ? worldBorder.top : worldBorder.top + worldBorder.height;

  sprite.setPosition({xPos, yPos});
  // TODO: change scaling now that it is a saw instead of a spike
  sprite.setScale({Utility::gameScale, (isOnTop ? 1.0f : -1.0f) * Utility::gameScale});
  cutOffPoint = worldBorder.left + (isGoingRight ? worldBorder.width : 0.0f);

  anims = AnimationHandler(&sprite);
  anims.QueueAnimation(0, 50);
}

void Saw::Update(std::vector<Character*> players)
{
  if ((vel > 0.0f && sprite.getPosition().x > cutOffPoint) ||
      (vel < 0.0f && sprite.getPosition().x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }

  if (!isFrozen)
  {
    sprite.move({vel, 0.0f});
  }

  float distanceSquaredThresh = SCALED_DIM * SCALED_DIM;

  for (auto& p : players)
  {
    // if (sprite.getGlobalBounds().intersects(p->GetHitBox()))
    // // Change to radial distance chenck (is spritedim * gamescale < distance between the two)
    // {
    //   p->Hit();
    //   // if (p->Hit())
    //   // {
    //   //   sf::Vector2f middlePos = 0.5f * (p->GetLineHitBox().second + p->GetLineHitBox().first);

    //   //   sf::Vector2f pointVel = 0.2f * (sprite.getPosition() - middlePos);
    //   //   p->AddNewPoint(-1000, sprite.getPosition(), pointVel);
    //   // }
    // }

    // needs to loop through all line segments if there are multiple
    // maybe try to optimise by doing a broad check of proximity before doing line segment calculations
    float squaredDistance = Utility::GetSquaredDistanceToLineSegment(sprite.getPosition(), p->GetLineHitBox());
    // std::cout << "Distance: " << squaredDistance << '\n';
    if (squaredDistance < distanceSquaredThresh)
    {
      p->Hit(sprite.getPosition());
    }
  }

  anims.Update();
}

// = ------------ =
// = Target Class =
// = ------------ =

MovingTarget::MovingTarget(sf::Texture* tex, sf::IntRect &worldBorder, int maxID)
  :
  Entity(tex, maxID)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution yDist((worldBorder.top) + int(SCALED_DIM) + posBuffer, -worldBorder.top - (int(SCALED_DIM) + posBuffer));
  std::uniform_int_distribution xDist(0, 1);
  std::uniform_real_distribution<float> velDist(0.3f, 1.0f);

  int isGoingRight = xDist(Utility::rng);

  vel = Utility::gameScale * velDist(Utility::rng) * (isGoingRight ? 1.0f : -1.0f);

  float xPos = isGoingRight ? worldBorder.left - SCALED_DIM + posBuffer : worldBorder.left + worldBorder.width + posBuffer;
  float yPos = yDist(Utility::rng);

  sprite.setPosition({xPos, yPos});
  sprite.setScale(DEFAULT_SCALE);
  cutOffPoint = worldBorder.left + posBuffer + (isGoingRight ? worldBorder.width : 0.0f);

  anims = AnimationHandler(&sprite);
  anims.QueueAnimation(1, 100);
}


void MovingTarget::Update(std::vector<Character*> players)
{

  if ((vel > 0.0f && sprite.getPosition().x > cutOffPoint) ||
      (vel < 0.0f && sprite.getPosition().x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }
  
  if (!isFrozen)
  {
    sprite.move({vel, 0.0f});
  }

  // float distanceSquaredThresh = SCALED_DIM * SCALED_DIM;
  // change to pointer to use range based for loop
  int closestIndex = -1;
  float closestDistance = SCALED_DIM * SCALED_DIM;

  for (int i = 0; i < players.size(); i++)
  {
    if (players[i]->GetCurState() == Character::State::airborne)
    {
      // needs to loop through all line segments if there are multiple
      // maybe try to optimise by doing a broad check of proximity before doing line segment calculations
      float squaredDistance = Utility::GetSquaredDistanceToLineSegment(sprite.getPosition(), players[i]->GetLineHitBox());
      if (squaredDistance < closestDistance)
      {
        closestIndex = i;
        closestDistance = squaredDistance;
      }
    }
  }

  if (closestIndex == -1)
  {
    anims.Update();
    return;
  }

  Utility::particles.push_front(Particle(Particle::Type::targetExplosion, ZERO_VECTOR, sprite.getPosition(), sprite.getScale()));

  //players[closestIndex].TargetHit(sprite.getPosition());

  // in player, loop through vector of points (that represent the segment), and take the average
  sf::Vector2f middlePos = 0.5f * (players[closestIndex]->GetLineHitBox().second + players[closestIndex]->GetLineHitBox().first);

  sf::Vector2f pointVel = 0.02f * (sprite.getPosition() - middlePos);

  players[closestIndex]->AddNewPoint(sprite.getPosition(), pointVel);

  endOfLife = true;
  players[closestIndex]->IncrementComboCount();
}

// Target::StationaryTarget(sf::Vector2f pos) // Maybe seperate "MovingTarget" class
// {
//   sprite.setTexture(tex);

//   sprite.setOrigin(CENTRED_ORIGIN);
//   sprite.setPosition(pos);
//   sprite.setScale(DEFAULT_SCALE);

//   anim = Animation(&sprite);
//   anim.ChangeAnimation(1, 100);
// }

// Update() if isFrozen (dont increment lifespan timer)