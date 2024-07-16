#include "Entity.h"

Entity::Entity(const char* textureName, Params params)
  :
  params(params)
{
  sprite = std::make_unique<sf::Sprite>(Textures::textures.at(textureName));
  sprite.get()->setScale(params.scaleModifier * DEFAULT_SCALE);

  anim = AnimationHandler(sprite.get(), params.frameSize);
  motion = MotionHandler(sprite.get());

  // Sets the origin to the middle of the rendered sprite
  sf::Vector2f origin(sprite.get()->getTextureRect().width / 2.0f, sprite.get()->getTextureRect().height / 2.0f);
  sprite.get()->setOrigin(origin);
}

void Entity::CouplePosition(sf::Vector2f *pos)
{
  position = pos;
  sprite.get()->setPosition(*position);  
}

void Entity::DecouplePosition()
{
  sprite.get()->setPosition(*position);
  position = nullptr;
}

void Entity::Update()
{
  anim.Update();

  if (position != nullptr)
  {
    sprite.get()->setPosition(*position);
    return;
  }

  motion.Update();
}

void Entity::Render(sf::RenderWindow *win) const
{
  if (params.shader == nullptr)
  {
    win->draw(*sprite.get());
    return;
  }

  win->draw(*sprite.get(), params.shader);
}

void Entity::FlipX()
{
  sprite.get()->scale(sf::Vector2f(-1.0f, 1.0f));
}

void Entity::FlipY()
{
  sprite.get()->scale(sf::Vector2f(1.0f, -1.0f));
}

void Entity::SetXDir(bool right)
{
  float scale = (right ? 1.0f : -1.0f);
  if (Utility::GetSign(scale) != Utility::GetSign(sprite.get()->getScale().x))
    FlipX();
}

void Entity::SetYDir(bool up)
{
  float scale = (up ? 1.0f : -1.0f);
  if (Utility::GetSign(scale) != Utility::GetSign(sprite.get()->getScale().y))
    FlipY();
}

sf::Vector2f Entity::GetPosition() const
{
  return sprite.get()->getPosition();
}

void Entity::SetRotation(float angle)
{
  sprite.get()->setRotation(angle);
}

void Entity::ClearHandlers()
{
  anim.Clear();
  motion.Clear();
}

sf::FloatRect Entity::HitBox() const
{
  return sprite.get()->getGlobalBounds();
}

void Entity::QueueAnimation(int index, int dur, int loops, int hold)
{
  anim.QueueAnimation(index, dur, loops, hold);
}

void Entity::SetAnimation(int index, int dur, int loops, int hold)
{
  anim.Clear();
  QueueAnimation(index, dur, loops, hold);  
}

void Entity::QueueMotion(MotionHandler::Type type, float duration, sf::Vector2f offSet)
{
  QueueMotion(type, duration, sprite.get()->getPosition(), sprite.get()->getPosition() + offSet);
}

void Entity::QueueMotion(MotionHandler::Type type, float duration, sf::Vector2f start, sf::Vector2f end)
{
  motion.QueueMotion(type, duration, start, end);
}
