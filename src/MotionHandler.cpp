#include "MotionHandler.h"

MotionHandler::MotionHandler(sf::Sprite* sprite)
  : 
  sprite(sprite)
{}

void MotionHandler::Update()
{
  if (motions.empty())
  {
    return;
  }

  timer += Clock::Delta();

  while (timer >= cur->duration)
  {
    NextMotion();
    if (motions.empty())
    {
      return;
    }
  }

  sprite->setPosition(cur->startPoint + Utility::curves[(int)cur->curve].GetValue(timer / cur->duration) * (cur->endPoint - cur->startPoint));
}

// void MotionHandler::Update(sf::Sprite* sprite)
// {
//   this->sprite = sprite;
//   Update();
// }

void MotionHandler::Clear()
{
  while (!motions.empty())
  {
    motions.pop();
  }
}

void MotionHandler::NextMotion()
{
  timer -= cur->duration;
  sprite->setPosition(cur->endPoint);

  motions.pop();
  if (motions.empty())
  {
    cur = nullptr;
    return;
  }

  cur = &motions.front();
}


void MotionHandler::Queue(Motion& motion)
{
  motions.push(motion);

  if (motions.size() == 1) // This motion was pushed to an empty queue
  {
    timer = 0.0f;
    cur = &motions.front();
    sprite->setPosition(cur->startPoint);
  }
}

void MotionHandler::Queue(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  Motion motion;
  motion.curve = curve;
  motion.duration = duration;
  motion.startPoint = start;
  motion.endPoint = end;
  Queue(motion);
}

sf::Vector2f MotionHandler::GetEndPoint() const
{
  if (motions.empty())
  {
    return sprite->getPosition();
  }

  return motions.back().endPoint;
}
