#include "MotionHandler.h"

std::vector<Bezier> MotionHandler::curves =
{
  Bezier({{0, 0},{1, 1}}),
  Bezier({{0, 0},{0,1},{1, 1}}),
  Bezier({{0, 0},{1,0},{1, 1}})
};

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

  sprite->setPosition(cur->startPoint + curves[(int)cur->type].GetValue(timer / cur->duration) * (cur->endPoint - cur->startPoint));
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


void MotionHandler::QueueMotion(Motion& motion)
{
  motions.push(motion);

  if (motions.size() == 1) // This motion was pushed to an empty queue
  {
    timer = 0.0f;
    cur = &motions.front();
    sprite->setPosition(cur->startPoint);
  }
}

void MotionHandler::QueueMotion(Type type, float duration, sf::Vector2f start, sf::Vector2f end)
{
  Motion motion;
  motion.type = type;
  motion.duration = duration;
  motion.startPoint = start;
  motion.endPoint = end;
  QueueMotion(motion);
}