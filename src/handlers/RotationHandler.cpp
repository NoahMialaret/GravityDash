#include "RotationHandler.h"

RotationHandler::RotationHandler(sf::Sprite* sprite)
  : 
  sprite(sprite)
{}

void RotationHandler::Update()
{
  if (rotations.empty())
  {
    return;
  }

  timer += Clock::Delta();

  while (timer >= cur->duration)
  {
    NextRotation();
    if (rotations.empty())
    {
      return;
    }
  }

  sprite->setRotation(cur->start + Utility::curves[(int)cur->curve].GetValue(timer / cur->duration) * (cur->end - cur->start));
}

// void RotationHandler::Update(sf::Sprite* sprite)
// {
//   this->sprite = sprite;
//   Update();
// }

void RotationHandler::Clear()
{
  while (!rotations.empty())
  {
    rotations.pop();
  }
}

void RotationHandler::NextRotation()
{
  timer -= cur->duration;
  sprite->setRotation(cur->end);

  rotations.pop();
  if (rotations.empty())
  {
    cur = nullptr;
    return;
  }

  cur = &rotations.front();
}


void RotationHandler::Queue(Rotation& rotation)
{
  rotations.push(rotation);

  if (rotations.size() == 1) // This rotation was pushed to an empty queue
  {
    timer = 0.0f;
    cur = &rotations.front();
    sprite->setRotation(cur->start);
  }
}

void RotationHandler::Queue(Curve curve, float duration, float start, float end)
{
  Rotation rotation;
  rotation.curve = curve;
  rotation.duration = duration;
  rotation.start = start;
  rotation.end = end;
  Queue(rotation);
}

float RotationHandler::GetEndRotation() const
{
  if (rotations.empty())
  {
    return sprite->getRotation();
  }

  return rotations.back().end;
}
