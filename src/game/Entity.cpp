#include "Entity.h"

Entity::Entity(const char* texName, 
          sf::Shader* shader, 
          sf::Vector2i numSprites, 
          sf::Vector2f pos, 
          sf::Vector2f origin)
  :
  shader(shader),
  position(pos),
  positionTransition(&position),
  scaleTransition(&scale),
  rotationTransition(&rotation)
{
  Utility::InitSprite(sprite, texName, position, numSprites, origin);

  anim = AnimationHandler(&sprite, numSprites.y, numSprites.x, sprite.getTextureRect().getSize());
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

  if (shader == nullptr)
  {
    win->draw(sprite);
    return;
  }

  win->draw(sprite, shader);
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

void Entity::ClearTransitions()
{
  positionTransition.Clear();
  scaleTransition.Clear();
  rotationTransition.Clear();
}

void Entity::PushPositionTransition(Curve curve, float duration, sf::Vector2f offset)
{
  positionTransition.Push(curve, duration, offset);
}

void Entity::PushPositionTransition(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  positionTransition.Push(curve, duration, start, end);
}

void Entity::PushScaleTransition(Curve curve, float duration, sf::Vector2f offset)
{
  scaleTransition.Push(curve, duration, offset);
}

void Entity::PushScaleTransition(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  scaleTransition.Push(curve, duration, start, end);
}

void Entity::PushRotationTransition(Curve curve, float duration, float offset)
{
  rotationTransition.Push(curve, duration, offset);
}

void Entity::PushRotationTransition(Curve curve, float duration, float start, float end)
{
  rotationTransition.Push(curve, duration, start, end);
}