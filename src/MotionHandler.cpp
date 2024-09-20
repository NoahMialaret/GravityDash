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

  sprite->setPosition(cur->points.GetPoint(Utility::curves[(int)cur->curve].GetValue(timer / cur->duration)));
}

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
  sprite->setPosition(cur->points.GetPoint(1.0f));

  motions.pop();
  if (motions.empty())
  {
    cur = nullptr;
    return;
  }

  cur = &motions.front();
}



void MotionHandler::Queue(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  Queue(curve, duration, Bezier({start, end}));
}

void MotionHandler::Queue(Curve curve, float duration, Bezier points)
{
  Motion motion;
  motion.curve = curve;
  motion.duration = duration;
  motion.points = points;
  Queue(motion);
}

void MotionHandler::Queue(Motion& motion)
{
  motions.push(motion);

  if (motions.size() == 1) // This motion was pushed to an empty queue
  {
    timer = 0.0f;
    cur = &motions.front();
    sprite->setPosition(cur->points.GetPoint(0.0f));
  }
}

sf::Vector2f MotionHandler::GetEndPoint() const
{
  if (motions.empty())
  {
    return sprite->getPosition();
  }

  return motions.back().points.GetPoint(1.0f);
}
