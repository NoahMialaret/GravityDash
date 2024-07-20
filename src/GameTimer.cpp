#include "GameTimer.h"

GameTimer::GameTimer(int time, sf::Vector2f bottomLeft)
  :
  timeRemaining(time),
  maxTime(time)
{
  timeRect = sf::RectangleShape(Utility::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setPosition(bottomLeft + sf::Vector2f(0.0f, - Utility::gameScale));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  sprite.setTexture(Textures::textures.at("timer"));
  sprite.setScale(DEFAULT_SCALE);
  sprite.setPosition(bottomLeft);
  sprite.setOrigin(sf::Vector2f(0.0f, sprite.getTextureRect().height));
}

bool GameTimer::Update()
{
  if (paused)
  {
    return false;
  }
  
  timeRemaining -= Clock::Delta();
  timeRect.setSize(Utility::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));

  if (timeRemaining <= 0)
  {
    timeRect.setSize(ZERO_VECTOR);
    return true;
  }

  return false;
}

void GameTimer::Render(sf::RenderWindow* win) const
{
  win->draw(timeRect);
  win->draw(sprite);
}

void GameTimer::AddTime(int addition)
{
  timeRemaining += addition;
  if (timeRemaining > maxTime)
  {
    timeRemaining = maxTime;
  }
  
  timeRect.setSize(Utility::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));
}

void GameTimer::Pause()
{
  paused = true;
}

void GameTimer::Unpause()
{
  paused = false;
}

bool GameTimer::IsPaused() const
{
  return paused;
}