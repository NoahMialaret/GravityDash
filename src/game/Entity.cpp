#include "Entity.h"

Entity::Entity(const char *textureName, sf::Shader* shader, sf::Vector2i frameSize, sf::Vector2f origin)
  :
  shader(shader)
{
  sprite = std::make_unique<sf::Sprite>(Textures::textures.at(textureName));
  sprite.get()->setScale(DEFAULT_SCALE);

  anim = AnimationHandler(sprite.get(), frameSize);
  motion = MotionHandler(sprite.get());
  scaleHand = ScaleHandler(sprite.get());
  rot = RotationHandler(sprite.get());

  // Sets the origin to the middle of the rendered sprite
  sf::Vector2f size(sprite.get()->getTextureRect().width, sprite.get()->getTextureRect().height);
  sprite.get()->setOrigin({origin.x * size.x, origin.y * size.y});
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

void Entity::CoupleScale(sf::Vector2f* scale)
{
  this->scale = scale;
  sprite.get()->setScale(*scale);  
}

void Entity::DecoupleScale()
{
  sprite.get()->setScale(*position);
  scale = nullptr;
}

void Entity::CoupleRotation(float* rot)
{
  rotation = rot;
  sprite.get()->setRotation(*rotation);  
}

void Entity::DecoupleRotation()
{
  sprite.get()->setRotation(*rotation);
  rotation = nullptr;
}

void Entity::Update()
{
  anim.Update();

  if (position == nullptr)
    motion.Update();

  if (scale == nullptr)
    scaleHand.Update();

  if (rotation == nullptr)
    rot.Update();
}

void Entity::Render(sf::RenderWindow *win) const
{
  if (position != nullptr)
    sprite.get()->setPosition(*position);

  if (scale != nullptr)
    sprite.get()->setScale(*scale);

  if (rotation != nullptr)
    sprite.get()->setRotation(*rotation);

  if (shader == nullptr)
  {
    win->draw(*sprite.get());
    return;
  }

  win->draw(*sprite.get(), shader);
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

sf::Vector2f Entity::GetScale() const
{
  return sprite.get()->getScale();
}

float Entity::GetRotation() const
{
  return sprite.get()->getRotation();
}

void Entity::ClearHandlers()
{
  anim.Clear();
  motion.Clear();
  scaleHand.Clear();
  rot.Clear();
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

void Entity::QueueMotion(Curve curve, float duration, sf::Vector2f offSet)
{
  QueueMotion(curve, duration, motion.GetEndPoint(), motion.GetEndPoint() + offSet);
}

void Entity::QueueMotion(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  QueueMotion(curve, duration, Bezier({start, end}));
}

void Entity::QueueMotion(Curve curve, float duration, Bezier points)
{
  motion.Queue(curve, duration, points);
}

void Entity::QueueScale(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  QueueScale(curve, duration, Bezier({start, end}));
}

void Entity::QueueScale(Curve curve, float duration, Bezier scales)
{
  scaleHand.Queue(curve, duration, scales);
}

void Entity::QueueRotation(Curve curve, float duration, float start, float end)
{
  rot.Queue(curve, duration, start, end);
}
