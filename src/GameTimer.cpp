#include "GameTimer.h"

GameTimer::GameTimer(sf::Int32 time, sf::Vector2f bottomLeft)
  :
  timeRemaining(time),
  maxTime(time)
{
  timeRect = sf::RectangleShape(Utility::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setPosition(bottomLeft + sf::Vector2f(0.0f, - Utility::gameScale));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  if (!texture.loadFromFile("assets/timer.png"))
  {
    std::cout << "\tTimer texture could not be loaded!\n";
  }
  sprite.setTexture(texture);
  sprite.setScale(DEFAULT_SCALE);
  sprite.setPosition(bottomLeft);
  sprite.setOrigin(sf::Vector2f(0.0f, sprite.getTextureRect().height));

  lastFrameTime = CUR_TIME;
}

bool GameTimer::Update()
{
  if (paused)
  {
    return false;
  }
  sf::Int32 delta = CUR_TIME - lastFrameTime;
  lastFrameTime = CUR_TIME;
  timeRemaining -= delta;
  timeRect.setSize(Utility::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));

  if (timeRemaining <= 0)
  {
    return true;
  }

  return false;
}

void GameTimer::Pause()
{
  paused = true;
}

void GameTimer::Unpause()
{
  paused = false;
  lastFrameTime = CUR_TIME;
}

bool GameTimer::IsPaused() const
{
  return paused;
}

void GameTimer::Render(sf::RenderWindow* win) const
{
  win->draw(timeRect);
  win->draw(sprite);
}

void GameTimer::AddTime(sf::Int32 addition)
{
  timeRemaining += addition;
  if (timeRemaining > maxTime)
  {
    timeRemaining = maxTime;
  }
  
  timeRect.setSize(Utility::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));
}
