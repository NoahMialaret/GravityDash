#include "Entity.hpp"

Entity::Entity(Texture* tex)
{
  sprite.SetTexture(tex);
  sprite.SetOrigin(CENTRED_ORIGIN);
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

void Entity::Render() const
{
  //Utility::shaderTest.setUniform("texture", sprite.getTexture());
  sprite.Render();
}

bool Entity::operator<=(Entity& rhs)
{
  return this->sprite.GetTranslation().y <= rhs.sprite.GetTranslation().y;
}

bool Entity::operator<(Entity& rhs)
{
  return this->sprite.GetTranslation().y < rhs.sprite.GetTranslation().y;
}

bool Entity::operator>=(Entity& rhs)
{
  return this->sprite.GetTranslation().y >= rhs.sprite.GetTranslation().y;
}

bool Entity::operator>(Entity& rhs)
{
  return this->sprite.GetTranslation().y > rhs.sprite.GetTranslation().y;
}

bool Entity::operator<=(float rhs)
{
  return this->sprite.GetTranslation().y <= rhs;
}

bool Entity::operator<(float rhs)
{
  return this->sprite.GetTranslation().y < rhs;
}

bool Entity::operator>=(float rhs)
{
  return this->sprite.GetTranslation().y >= rhs;
}

bool Entity::operator>(float rhs)
{
  return this->sprite.GetTranslation().y > rhs;
}

fVec2 Entity::GetPosition()
{
  return sprite.GetTranslation();
} 

// = --------- =
// = Saw Class =
// = --------- =

Saw::Saw(Texture* tex, IRect &worldBorder)
  :
  Entity(tex)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution dist(0, 1);
  int isOnTop = dist(Utility::rng);
  int isGoingRight = dist(Utility::rng);

  vel = 1.0f * (isGoingRight ? Utility::gameScale : -Utility::gameScale);

  float xPos = isGoingRight ? worldBorder.left - SCALED_DIM : worldBorder.left + worldBorder.width;
  float yPos = isOnTop ? worldBorder.top : worldBorder.top + worldBorder.height;

  sprite.SetTranslation({xPos, yPos});
  // TODO: change scaling now that it is a saw instead of a spike
  sprite.SetScale({Utility::gameScale, (isOnTop ? 1.0f : -1.0f) * Utility::gameScale});
  cutOffPoint = worldBorder.left + (isGoingRight ? worldBorder.width : 0.0f);

  anim = Animation(&sprite);
  anim.ChangeAnimation(0, 0.050);
}

void Saw::Update(std::vector<Character*> players)
{
  if ((vel > 0.0f && sprite.GetTranslation().x > cutOffPoint) ||
      (vel < 0.0f && sprite.GetTranslation().x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }

  if (!isFrozen)
  {
    sprite.Translate({vel, 0.0f});
  }

  for (auto& p : players)
  {
    if (sprite.GetBoundingRectangle().IsColliding(p->GetHitBox()))
    // Change to radial distance chenck (is spritedim * gamescale < distance between the two)
    {
      p->Hit();
      // if (p->Hit())
      // {
      //   fVec2 middlePos = 0.5f * (p->GetLineHitBox().second + p->GetLineHitBox().first);

      //   fVec2 pointVel = 0.2f * (sprite.GetTranslation() - middlePos);
      //   p->AddNewPoint(-1000, sprite.GetTranslation(), pointVel);
      // }
    }
  }

  anim.Update();
}

// = ------------ =
// = Target Class =
// = ------------ =

MovingTarget::MovingTarget(Texture* tex, IRect &worldBorder)
  :
  Entity(tex)
{
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution yDist((worldBorder.top) + int(SCALED_DIM) + posBuffer, -worldBorder.top - (int(SCALED_DIM) + posBuffer));
  std::uniform_int_distribution xDist(0, 1);
  std::uniform_real_distribution<float> velDist(0.3f, 1.0f);

  int isGoingRight = xDist(Utility::rng);

  vel = Utility::gameScale * velDist(Utility::rng) * (isGoingRight ? 1.0f : -1.0f);

  float xPos = isGoingRight ? worldBorder.left - SCALED_DIM + posBuffer : worldBorder.left + worldBorder.width + posBuffer;
  float yPos = yDist(Utility::rng);

  sprite.SetTranslation({xPos, yPos});
  sprite.SetScale(DEFAULT_SCALE);
  cutOffPoint = worldBorder.left + posBuffer + (isGoingRight ? worldBorder.width : 0.0f);

  anim = Animation(&sprite);
  anim.ChangeAnimation(1, 0.100);
}


void MovingTarget::Update(std::vector<Character*> players)
{

  if ((vel > 0.0f && sprite.GetTranslation().x > cutOffPoint) ||
      (vel < 0.0f && sprite.GetTranslation().x + SCALED_DIM < cutOffPoint))
  {
    endOfLife = true;
    return;
  }
  
  if (!isFrozen)
  {
    sprite.Translate({vel, 0.0f});
  }

  float distanceSquaredThresh = SCALED_DIM * SCALED_DIM;
  // change to pointer to use range based for loop
  int closestIndex = -1;
  float closestDistance = SCALED_DIM * SCALED_DIM;

  for (int i = 0; i < players.size(); i++)
  {
    if (players[i]->GetCurState() == Character::State::airborne)
    {
      // needs to loop through all line segments if there are multiple
      // maybe try to optimise by doing a broad check of proximity before doing line segment calculations
      float squaredDistance = Utility::GetSquaredDistanceToLineSegment(sprite.GetTranslation(), players[i]->GetLineHitBox());
      if (squaredDistance < closestDistance)
      {
        closestIndex = i;
        closestDistance = squaredDistance;
      }
    }
  }

  if (closestIndex == -1)
  {
    anim.Update();
    return;
  }

  Particle *temp = new Particle(Particle::Type::targetExplosion, ZERO_VECTOR, sprite.GetTranslation(), sprite.GetScale());
  Particle::CreateNewParticle(temp);

  //players[closestIndex].TargetHit(sprite.GetTranslation());

  // in player, loop through vector of points (that represent the segment), and take the average
  fVec2 middlePos = (players[closestIndex]->GetLineHitBox().second + players[closestIndex]->GetLineHitBox().first) * 0.5f;

  fVec2 pointVel = (sprite.GetTranslation() - middlePos) * 0.2f;

  players[closestIndex]->AddNewPoint(sprite.GetTranslation(), pointVel);

  endOfLife = true;
  players[closestIndex]->IncrementComboCount();
}

// Target::StationaryTarget(fVec2 pos) // Maybe seperate "MovingTarget" class
// {
//   sprite.setTexture(tex);

//   sprite.setOrigin(CENTRED_ORIGIN);
//   sprite.setPosition(pos);
//   sprite.setScale(DEFAULT_SCALE);

//   anim = Animation(&sprite);
//   anim.ChangeAnimation(1, 100);
// }

// Update() if isFrozen (dont increment lifespan timer)