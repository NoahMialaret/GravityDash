#include "ScaleHandler.h"

ScaleHandler::ScaleHandler(sf::Sprite* sprite)
  : 
  sprite(sprite)
{}

void ScaleHandler::Update()
{
  if (scales.empty())
  {
    return;
  }

  timer += Clock::Delta();

  while (timer >= cur->duration)
  {
    NextScale();
    if (scales.empty())
    {
      return;
    }
  }

  sprite->setScale(cur->start + Utility::curves[(int)cur->curve].GetValue(timer / cur->duration) * (cur->end - cur->start));
}

void ScaleHandler::Clear()
{
  while (!scales.empty())
  {
    scales.pop();
  }
}

void ScaleHandler::NextScale()
{
  timer -= cur->duration;
  sprite->setScale(cur->end);

  scales.pop();
  if (scales.empty())
  {
    cur = nullptr;
    return;
  }

  cur = &scales.front();
}


void ScaleHandler::Queue(Scale& scale)
{
  scales.push(scale);

  if (scales.size() == 1) // This scale was pushed to an empty queue
  {
    timer = 0.0f;
    cur = &scales.front();
    sprite->setScale(cur->start);
  }
}

void ScaleHandler::Queue(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end)
{
  Scale scale;
  scale.curve = curve;
  scale.duration = duration;
  scale.start = start;
  scale.end = end;
  Queue(scale);
}

sf::Vector2f ScaleHandler::GetEndScale() const
{
  if (scales.empty())
  {
    return sprite->getScale();
  }

  return scales.back().end;
}
