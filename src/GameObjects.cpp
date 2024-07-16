#include "GameObject.h"

GameObject::GameObject(int maxID)
{
  entity = Entity("entities", Entity::Params());

  std::uniform_int_distribution id(0, maxID - 1);
  entID = id(Utility::rng);
}

bool GameObject::EndOfLife() const
{
  return endOfLife;
}

void GameObject::Freeze()
{
  isFrozen = true;
}

void GameObject::Unfreeze()
{
  isFrozen = false;
}

void GameObject::Render(sf::RenderWindow* win) const
{
  //Utility::shaderTest.setUniform("texture", sprite.getTexture());
  Utility::entShad.setUniform("colorID", entID);
  entity.Render(win);
}

bool GameObject::operator<=(GameObject& rhs)
{
  return this->pos.y <= rhs.pos.y;
}

bool GameObject::operator<(GameObject& rhs)
{
  return this->pos.y < rhs.pos.y;
}

bool GameObject::operator>=(GameObject& rhs)
{
  return this->pos.y >= rhs.pos.y;
}

bool GameObject::operator>(GameObject& rhs)
{
  return this->pos.y > rhs.pos.y;
}

bool GameObject::operator<=(float rhs)
{
  return this->pos.y <= rhs;
}

bool GameObject::operator<(float rhs)
{
  return this->pos.y < rhs;
}

bool GameObject::operator>=(float rhs)
{
  return this->pos.y >= rhs;
}

bool GameObject::operator>(float rhs)
{
  return this->pos.y > rhs;
}

sf::Vector2f GameObject::GetPosition()
{
  return pos;
}

// = --------- =
// = Saw Class =
// = --------- =

Saw::Saw(sf::IntRect& worldBorder, int maxID)
  :
  GameObject(maxID)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution dist(0, 1);
  int isOnTop = dist(Utility::rng);
  int isGoingRight = dist(Utility::rng);

  vel = 1.0f * (isGoingRight ? Utility::gameScale : -Utility::gameScale);

  pos.x = isGoingRight ? worldBorder.left - SCALED_DIM : worldBorder.left + worldBorder.width;
  pos.y = isOnTop ? worldBorder.top : worldBorder.top + worldBorder.height;
  entity.CouplePosition(&pos);

  // TODO: change scaling now that it is a saw instead of a spike
  if (isOnTop)
  {
    entity.FlipY();
  }
  cutOffPoint = worldBorder.left + (isGoingRight ? worldBorder.width : 0.0f);

  entity.QueueAnimation(0, 50);
}

void Saw::Update(std::vector<Character*> players)
{
  if ((vel > 0.0f && pos.x > cutOffPoint) ||
      (vel < 0.0f && pos.x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }

  if (!isFrozen)
  {
    pos.x += (Clock::Delta() / 16.0f) * vel;
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

    //   //   sf::Vector2f pointVel = 0.2f * (pos - middlePos);
    //   //   p->AddNewPoint(-1000, pos, pointVel);
    //   // }
    // }

    // needs to loop through all line segments if there are multiple
    // maybe try to optimise by doing a broad check of proximity before doing line segment calculations
    float squaredDistance = Utility::GetSquaredDistanceToLineSegment(pos, p->GetLineHitBox());
    // std::cout << "Distance: " << squaredDistance << '\n';
    if (squaredDistance < distanceSquaredThresh)
    {
      p->Hit(pos);
    }
  }

  entity.Update();
}

// = ------------ =
// = Target Class =
// = ------------ =

MovingTarget::MovingTarget(sf::IntRect &worldBorder, int maxID)
  :
  GameObject(maxID)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution yDist((worldBorder.top) + int(SCALED_DIM) + posBuffer, -worldBorder.top - (int(SCALED_DIM) + posBuffer));
  std::uniform_int_distribution xDist(0, 1);
  std::uniform_real_distribution<float> velDist(0.3f, 1.0f);

  int isGoingRight = xDist(Utility::rng);

  vel = Utility::gameScale * velDist(Utility::rng) * (isGoingRight ? 1.0f : -1.0f);

  pos.x = isGoingRight ? worldBorder.left - SCALED_DIM + posBuffer : worldBorder.left + worldBorder.width + posBuffer;
  pos.y = yDist(Utility::rng);
  entity.CouplePosition(&pos);

  cutOffPoint = worldBorder.left + posBuffer + (isGoingRight ? worldBorder.width : 0.0f);

  entity.QueueAnimation(1, 100);
}


void MovingTarget::Update(std::vector<Character*> players)
{

  if ((vel > 0.0f && pos.x > cutOffPoint) ||
      (vel < 0.0f && pos.x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }
  
  if (!isFrozen)
  {
    pos.x += (Clock::Delta() / 16.0f) * vel;
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
      float squaredDistance = Utility::GetSquaredDistanceToLineSegment(pos, players[i]->GetLineHitBox());
      if (squaredDistance < closestDistance)
      {
        closestIndex = i;
        closestDistance = squaredDistance;
      }
    }
  }

  if (closestIndex == -1)
  {
    entity.Update();
    return;
  }

  // Utility::particles.push_front(Particle(Particle::Type::targetExplosion, ZERO_VECTOR, pos, sprite.getScale()));

  //players[closestIndex].TargetHit(pos);

  // in player, loop through vector of points (that represent the segment), and take the average
  sf::Vector2f middlePos = 0.5f * (players[closestIndex]->GetLineHitBox().second + players[closestIndex]->GetLineHitBox().first);

  sf::Vector2f pointVel = 0.02f * (pos - middlePos);

  players[closestIndex]->AddNewPoint(pos, pointVel);

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