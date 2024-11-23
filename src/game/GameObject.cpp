#include "GameObject.h"

GameObject::GameObject(const sf::Vector2f& worldBounds)
  :
  worldBounds(worldBounds),
  // bounds({worldBorder.left - SCALED_DIM, worldBorder.left + worldBorder.width + SCALED_DIM}),
  entity(Entity("objects"))
{}

void GameObject::Update()
{
  if (pos.x < -(worldBounds.x + SCALED_DIM) || pos.x > worldBounds.x + SCALED_DIM)
    tombstone = true;

  if (tombstone)
    return;

  if (activated)
    pos.x += Clock::Delta() * vel;

  entity.Update();
}

void GameObject::HandleCollision(Character* character)
{
  std::pair<sf::Vector2f, sf::Vector2f> collision = character->GetLineHitBox();
  float squaredThreshold = SCALED_DIM * SCALED_DIM;

  // Do a broad check of proximity to prematurely stop calculations if the object is far away
  if (pos.x + SCALED_DIM < std::min(collision.first.x, collision.second.x) 
    || pos.x - SCALED_DIM > std::max(collision.first.x, collision.second.x))
      return ;
  
  float squaredDistance = Utility::GetSquaredDistanceToLineSegment(pos, collision);
  if (squaredDistance > squaredThreshold)
    return;

  if (!destructable)
  {
    tagEvent.collision = {character->GetID(), pos.x, pos.y};
    Event::events.push(tagEvent);
    return;
  }
  
  if (tag != -1 && squaredDistance > tagSquareDist)
    return;

  tag = character->GetID();
  tagSquareDist = squaredDistance;
}

void GameObject::ProcessTag()
{
  if (tag == -1)
    return;

  tagEvent.collision = {tag, pos.x, pos.y};
  Event::events.push(tagEvent);

  if (destructable)
  {
    tombstone = true;
    Utility::particles.push_front(std::make_unique<Explosion>(pos));
  }
}

void GameObject::Render(sf::RenderWindow* win) const
{
  if (!tombstone)
    entity.Render(win);
}

bool GameObject::IsTombstoned() const
{
  return tombstone;
}

sf::Vector2f GameObject::GetPosition()
{
  return pos;
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

  entity.CouplePosition(&pos);
  entity.QueueAnimation(0, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution dist(0, 1);

  int isOnTop = dist(Utility::rng);
  int isGoingRight = dist(Utility::rng);

  vel = 0.0625f * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
  pos.x = (isGoingRight ? -1.0f : 1.0f) * worldBounds.x + posBuffer;
  pos.y = (isOnTop ? -1.0f : 1.0f) * worldBounds.y;
}

void Saw::Deactivate()
{
  entity.DecouplePosition();
  entity.QueueMotion(Curve::linear, 200, (pos.y < 0 ? -1.0f : 1.0f) * sf::Vector2f(0.0f, SCALED_DIM));
  GameObject::Deactivate();
}

void Saw::Activate()
{
  entity.CouplePosition(&pos);
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

  entity.CouplePosition(&pos);
  entity.QueueAnimation(1, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution xDist(0, 1);
  int isGoingRight = xDist(Utility::rng);
  pos.x = (isGoingRight ? -1.0f : 1.0f) * (worldBounds.x + posBuffer);

  std::uniform_int_distribution yDist(
    int(2 * SCALED_DIM - worldBounds.y), 
    int(worldBounds.y - 2 * SCALED_DIM));
  pos.y = yDist(Utility::rng);
  yBase = pos.y;

  std::uniform_real_distribution<float> floatDist(0.3f, 1.0f);
  vel = 0.0625 * floatDist(Utility::rng) * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
  oscillationSpeed = 64.0f * vel;
  
  if (!isGoingRight)
    entity.FlipX();
}

void MovingTarget::Update()
{  
  GameObject::Update();

  if (!activated || tombstone)
    return;

  float rad = (float)Clock::Elapsed() / 1024.0f * oscillationSpeed;
  pos.y = yBase + 0.5f * ProgramSettings::gameScale * std::sin(rad);
}

// = --------------- =
// = TimeBonus Class =
// = --------------- =

TimeBonus::TimeBonus(const sf::Vector2f& worldBounds)
  :
  GameObject(worldBounds)
{
  tagEvent.type = Event::Type::collisionTimeBonus;

  entity.CouplePosition(&pos);
  entity.QueueAnimation(2, 50);

  // buffer between the centre and edge of the object
	int posBuffer = SCALED_DIM / 2;

  std::uniform_int_distribution xDist(0, 1);
  int isGoingRight = xDist(Utility::rng);
  pos.x = (isGoingRight ? -1.0f : 1.0f) * (worldBounds.x + posBuffer);

  std::uniform_int_distribution yDist(
    int(SCALED_DIM - worldBounds.y) + posBuffer, 
    int(worldBounds.y - SCALED_DIM) - posBuffer);
  pos.y = yDist(Utility::rng);

  std::uniform_real_distribution<float> floatDist(0.3f, 1.0f);
  vel = 1.5f * 0.0625f * floatDist(Utility::rng) * ProgramSettings::gameScale * (isGoingRight ? 1.0f : -1.0f);
}