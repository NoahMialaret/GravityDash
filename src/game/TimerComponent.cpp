#include "TimerComponent.h"

TimerComponent::TimerComponent(Game* game, int maxTime)
  :
  GameComponent(game),
  timeRemaining(maxTime),
  maxTime(maxTime)
{
  timeRect = sf::RectangleShape(ProgramSettings::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  Utility::InitSprite(gauge, "timer", ZERO_VECTOR, {1, 1}, {0.0f, 0.5f});

  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    gauge.setPosition(pos);
    timeRect.setPosition(pos + sf::Vector2f(0.0f, 0.5f * gauge.getGlobalBounds().height - ProgramSettings::gameScale));
  };
  game->Attach(World::AttachPoint::right, updatePosFunction);
}

void TimerComponent::Update()
{
  if (done)
    return;
  
  timeRemaining -= Clock::Delta();

  if (timeRemaining > 0)
  {
    timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
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
  win->draw(gauge);
}

void TimerComponent::AddTime(int addition)
{
  timeRemaining += std::min(timeRemaining + addition, maxTime);
  timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
}