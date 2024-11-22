#include "TimerComponent.h"

TimerComponent::TimerComponent(Game* game, int maxTime)
  :
  GameComponent(game),
  timeRemaining(maxTime),
  maxTime(maxTime)
{
  sf::Vector2f bottomLeft = ZERO_VECTOR;
  timeRect = sf::RectangleShape(ProgramSettings::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setPosition(bottomLeft + sf::Vector2f(0.0f, - ProgramSettings::gameScale));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  sprite.setTexture(Textures::textures.at("timer"));
  sprite.setScale(DEFAULT_SCALE);
  sprite.setPosition(bottomLeft);
  sprite.setOrigin(sf::Vector2f(0.0f, sprite.getTextureRect().height));
}

void TimerComponent::Update()
{
  if (done)
    return;
  
  timeRemaining -= Clock::Delta();

  if (timeRemaining > 0)
  {
    timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));
    return;
  }
  
  timeRect.setSize(ZERO_VECTOR);
  timeRemaining = 0;
  Event event;
  event.type = Event::Type::gameTimeUp;
  Event::events.push(event);
  done = true;
}

void TimerComponent::Render(sf::RenderWindow* win) const
{
  win->draw(timeRect);
  win->draw(sprite);
}

void TimerComponent::AddTime(int addition)
{
  timeRemaining += addition;
  if (timeRemaining > maxTime)
  {
    timeRemaining = maxTime;
  }
  
  timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)(60.0f * timeRemaining / maxTime)));
}