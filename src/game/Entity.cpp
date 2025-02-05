#include "Entity.h"

Entity::Entity(const char* texName, 
          sf::Vector2i numSprites, 
          sf::Vector2f pos, 
          sf::Vector2f origin)
  :
  position(pos),
  positionTransition(&position),
  scaleTransition(&scale),
  rotationTransition(&rotation)
{
  Utility::InitSprite(sprite, texName, numSprites, origin);
  sprite.setPosition(position);

  anim = AnimationHandler(&sprite, numSprites.y, numSprites.x, sprite.getTextureRect().getSize());
}

Entity::Entity(const Entity& rhs)
{
  anim = rhs.anim;
  anim.ChangeSprite(&sprite);

  positionTransition = rhs.positionTransition;
  positionTransition.ChangeData(&position);

  scaleTransition = rhs.scaleTransition;
  scaleTransition.ChangeData(&scale);

  rotationTransition = rhs.rotationTransition;
  rotationTransition.ChangeData(&rotation);

  position = rhs.position;
  scale = rhs.scale;
  rotation = rhs.rotation;
  sprite = rhs.sprite;
}

Entity& Entity::operator=(const Entity &rhs)
{
  anim = rhs.anim;
  anim.ChangeSprite(&sprite);

  positionTransition = rhs.positionTransition;
  positionTransition.ChangeData(&position);

  scaleTransition = rhs.scaleTransition;
  scaleTransition.ChangeData(&scale);

  rotationTransition = rhs.rotationTransition;
  rotationTransition.ChangeData(&rotation);

  position = rhs.position;
  scale = rhs.scale;
  rotation = rhs.rotation;
  sprite = rhs.sprite;

  return *this;
}

sf::Vector2f* Entity::GetPosition()
{
  return &position;
}

sf::Vector2f* Entity::GetScale()
{
  return &scale;
}

float* Entity::GetRotation()
{
  return &rotation;
}

void Entity::Update()
{
  anim.Update();

  positionTransition.Update();

  scaleTransition.Update();

  rotationTransition.Update();
}

void Entity::Render(sf::RenderWindow *win) const
{
  sprite.setPosition(position);

  sprite.setScale(scale);

  sprite.setRotation(rotation);

  Utility::RenderSpriteWithScale(win, sprite, &ENTITY_SHADER);
  // win->draw(sprite);
}

void Entity::FlipX()
{
  scale.x *= -1.0f;
}

void Entity::FlipY()
{
  scale.y *= -1.0f;
}

void Entity::SetXDir(bool right)
{
  float dir = (right ? 1.0f : -1.0f);
  if (Utility::GetSign(dir) != Utility::GetSign(scale.x))
    FlipX();
}

void Entity::SetYDir(bool up)
{
  float dir = (up ? 1.0f : -1.0f);
  if (Utility::GetSign(dir) != Utility::GetSign(scale.y))
    FlipY();
}

sf::FloatRect Entity::HitBox() const
{
  return sprite.getGlobalBounds();
}

void Entity::PushAnimation(int index, int dur, int loops, int hold)
{
  anim.QueueAnimation(index, dur, loops, hold);
}

void Entity::SetAnimation(int index, int dur, int loops, int hold)
{
  anim.Clear();
  PushAnimation(index, dur, loops, hold);  
}

void Entity::ClearAnimation()
{
  anim.Clear();
}

void Entity::ClearTransitions()
{
  positionTransition.Clear();
  scaleTransition.Clear();
  rotationTransition.Clear();
}

void Entity::PushPositionTransition(Bezier curve, float duration, sf::Vector2f offset)
{
  positionTransition.Push(curve, duration, offset);
}

void Entity::PushPositionTransition(Bezier curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  positionTransition.Push(curve, duration, start, end);
}

void Entity::PushScaleTransition(Bezier curve, float duration, sf::Vector2f offset)
{
  scaleTransition.Push(curve, duration, offset);
}

void Entity::PushScaleTransition(Bezier curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  scaleTransition.Push(curve, duration, start, end);
}

void Entity::PushRotationTransition(Bezier curve, float duration, float offset)
{
  rotationTransition.Push(curve, duration, offset);
}

void Entity::PushRotationTransition(Bezier curve, float duration, float start, float end)
{
  rotationTransition.Push(curve, duration, start, end);
}