#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& worldBounds)
  :
  worldBounds(worldBounds),
  entity("objects", &Utility::GetInstance()->GetEntityShader(), {4, 4})
{
  pos = entity.GetPosition();
}

void GameObject::Update()
{
  if (pos->x < -(worldBounds.x + SCALED_DIM) || pos->x > worldBounds.x + SCALED_DIM)
    tombstone = true;

  tombstoneTimer -= DELTA_TIME;

  if (tombstone && tombstoneTimer <= 0)
    return;

  if (activated && !tombstone)
    pos->x += DELTA_TIME * vel;

  entity.Update();
}

void GameObject::HandleCollision(Character* character)
{
  if (tombstone)
    return;

  std::pair<sf::Vector2f, sf::Vector2f> collision = character->GetLineHitBox();
  float squaredThreshold = SCALED_DIM * SCALED_DIM;

  // Do a broad check of proximity to prematurely stop calculations if the object is far away
  if (pos->x + SCALED_DIM < std::min(collision.first.x, collision.second.x) 
    || pos->x - SCALED_DIM > std::max(collision.first.x, collision.second.x))
      return ;
  
  float squaredDistance = Utility::GetInstance()->GetSquaredDistanceToLineSegment(*pos, collision);
  if (squaredDistance > squaredThreshold)
    return;
  
  if (tag != -1 && squaredDistance > tagSquareDist)
    return;

  tag = character->GetID();
  tagSquareDist = squaredDistance;
}

void GameObject::ProcessTag()
{
  if (tag == -1 || tombstone)
    return;

  tagEvent.collision = {tag, pos->x, pos->y};
  Event::events.push(tagEvent);
}

void GameObject::Render(sf::RenderWindow* win) const
{
  entity.Render(win);
}

bool GameObject::IsTombstoned() const
{
  return tombstone && tombstoneTimer <= 0;
}

sf::Vector2f GameObject::GetPosition()
{
  return *pos;
}

void GameObject::Deactivate()
{
  activated = false;
}

void GameObject::Activate()
{
  activated = true;
}

// = --------- =
// = Saw Class =
// = --------- =

Saw::Saw(const sf::Vector2f& worldBounds)
  :
  GameObject(worldBounds)
{
  tagEvent.type = Event::Type::collisionSaw;

  destructable = false;

  entity.PushAnimation(0, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution dist(0, 1);

  int isOnTop = dist(Utility::GetInstance()->GetRNG());
  int isGoingRight = dist(Utility::GetInstance()->GetRNG());

  vel = 0.0625f * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
  pos->x = (isGoingRight ? -1.0f : 1.0f) * worldBounds.x + posBuffer;
  pos->y = (isOnTop ? -1.0f : 1.0f) * worldBounds.y;
}

void Saw::Update()
{
  GameObject::Update();

  if (freezeTimer == -1)
    return;

  freezeTimer -= DELTA_TIME;

  if (freezeTimer <= 0)
  {
    Deactivate();
    freezeTimer = -1;
  }
}

void Saw::HandleCollision(Character* character)
{
  if (!character->IsInvincible())
    GameObject::HandleCollision(character);
}

void Saw::ProcessTag()
{
  GameObject::ProcessTag();

  if (tag == -1 || tombstone)
    return;

  freezeTimer = 1000;
  tombstoneTimer = 1200;
  tombstone = true;
  entity.ClearAnimation();
}

void Saw::Deactivate()
{
  entity.PushPositionTransition(LINEAR_CURVE, 200, (pos->y < 0 ? -1.0f : 1.0f) * sf::Vector2f(0.0f, SCALED_DIM));
  GameObject::Deactivate();
}

void Saw::Activate()
{
  GameObject::Activate();
}

// = ------------ =
// = Target Class =
// = ------------ =

MovingTarget::MovingTarget(const sf::Vector2f& worldBounds)
  :
  GameObject(worldBounds)
{
  tagEvent.type = Event::Type::collisionTarget;

  entity.PushAnimation(1, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution xDist(0, 1);
  int isGoingRight = xDist(Utility::GetInstance()->GetRNG());
  pos->x = (isGoingRight ? -1.0f : 1.0f) * (worldBounds.x + posBuffer);

  std::uniform_int_distribution yDist(
    int(2 * SCALED_DIM - worldBounds.y), 
    int(worldBounds.y - 2 * SCALED_DIM));
  pos->y = yDist(Utility::GetInstance()->GetRNG());
  yBase = pos->y;

  std::uniform_real_distribution<float> floatDist(0.3f, 1.0f);
  vel = 0.0625 * floatDist(Utility::GetInstance()->GetRNG()) * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
  oscillationSpeed = 64.0f * vel;
  
  if (!isGoingRight)
    entity.FlipX();
}

void MovingTarget::Update()
{  
  GameObject::Update();

  if (!activated || tombstone)
    return;

  float rad = (float)Clock::GetInstance()->Elapsed() / 1024.0f * oscillationSpeed;
  pos->y = yBase + 0.5f * ProgramSettings::gameScale * std::sin(rad);
}

void MovingTarget::HandleCollision(Character *character)
{
  if (character->GetCurState() == Character::State::airborne)
    GameObject::HandleCollision(character);
}

void MovingTarget::ProcessTag()
{
  GameObject::ProcessTag();

  if (tag == -1 || tombstone)
    return;

  tombstone = true;
  ParticleManager::GetInstance()->CreateParticle(Explosion(*pos));
}

// = --------------- =
// = TimeBonus Class =
// = --------------- =

TimeBonus::TimeBonus(const sf::Vector2f& worldBounds)
  :
  GameObject(worldBounds)
{
  tagEvent.type = Event::Type::collisionTimeBonus;

  entity.PushAnimation(2, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution xDist(0, 1);
  int isGoingRight = xDist(Utility::GetInstance()->GetRNG());
  pos->x = (isGoingRight ? -1.0f : 1.0f) * (worldBounds.x + posBuffer);

  std::uniform_int_distribution yDist(
    int(SCALED_DIM - worldBounds.y) + posBuffer, 
    int(worldBounds.y - SCALED_DIM) - posBuffer);
  pos->y = yDist(Utility::GetInstance()->GetRNG());

  std::uniform_real_distribution<float> floatDist(0.3f, 1.0f);
  vel = 1.5f * 0.0625f * floatDist(Utility::GetInstance()->GetRNG()) * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
}

void TimeBonus::HandleCollision(Character *character)
{
  if (character->GetCurState() == Character::State::airborne)
    GameObject::HandleCollision(character);
}

void TimeBonus::ProcessTag()
{
  GameObject::ProcessTag();

  if (tag == -1 || tombstone)
    return;

  tombstone = true;
  ParticleManager::GetInstance()->CreateParticle(Explosion(*pos));
}
