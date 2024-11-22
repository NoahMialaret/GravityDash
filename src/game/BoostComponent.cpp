#include "BoostComponent.h"

PlayerBoost::PlayerBoost(sf::Vector2f bottomLeft, int limit)
  :
  limit(limit)
{

  meter = Entity("boost_meter", nullptr, (sf::Vector2i)Textures::textures.at("boost_meter").getSize(), {0.0f, 1.0f});
  meter.QueueMotion(Curve::linear, 0, Bezier({bottomLeft}));
  meter.Update();
  if (bottomLeft.x > 0)
    meter.FlipX();
  if (bottomLeft.y > 0)
    meter.FlipY();

  fill.setFillColor(sf::Color(255, 229, 181));
  fill.setPosition(bottomLeft + ProgramSettings::gameScale * 
    sf::Vector2f((bottomLeft.x < 0 ? 1.0f : -1.0f) * 1.0f, (bottomLeft.y < 0 ? -1.0f : 1.0f) * 2.0f));
  fill.setSize({0.0f, 2.0f * ProgramSettings::gameScale});
  if (bottomLeft.x > 0)
    fill.scale({-1.0f, 1.0f});
  if (bottomLeft.y > 0)
    fill.scale({1.0f, -1.0f});
}

void PlayerBoost::Update()
{
  if (IsFull())
    return;
  
  timer -= Clock::Delta();
  
  if (timer <= 0)
  {
    timer = 0;
  }

  int width = 30.0f * (float)timer / (float)limit;

  fill.setSize(ProgramSettings::gameScale * sf::Vector2f((float)width, 2.0f));
}

void PlayerBoost::Render(sf::RenderWindow *win) const
{
  win->draw(fill);
  meter.Render(win);
}

void PlayerBoost::Increment(int amount)
{
  timer += amount;
  if (timer > limit)
  {
    timer = limit;
    fill.setSize(ProgramSettings::gameScale * sf::Vector2f(30.0f, 2.0f));
  }
}

void PlayerBoost::Clear()
{
  timer = 0;
  fill.setSize({0.0f, 2.0f * ProgramSettings::gameScale});
}

bool PlayerBoost::IsFull() const
{
  return timer >= limit;
}
