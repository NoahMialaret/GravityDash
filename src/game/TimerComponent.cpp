#include "TimerComponent.h"

TimerComponent::TimerComponent(Game* game, int maxTime)
  :
  GameComponent(game),
  timeRemaining(maxTime),
  maxTime(maxTime),
  boostCount(game->NumCharacters(), 0),
  arrowYMove(&arrowPos.y)
{
  Utility::InitSprite(refillArrow, "arrow", ZERO_VECTOR);

  timeRect = sf::RectangleShape(ProgramSettings::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  Utility::InitSprite(gauge, "timer", ZERO_VECTOR, {1, 1}, {0.0f, 0.5f});

  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    gauge.setPosition(pos);
    timeRect.setPosition(pos + sf::Vector2f(0.0f, 0.5f * gauge.getGlobalBounds().height - ProgramSettings::gameScale));
    arrowPos.x = pos.x + SCALED_DIM;
  };
  game->Attach(World::AttachPoint::right, updatePosFunction);
  MoveArrow();
}

void TimerComponent::ProcessEvent(Event &event)
{
  switch (event.type)
  {
  case Event::Type::collisionTimeBonus:
    boostCount[event.collision.charID]++;
    break;

  case Event::Type::playerCombo:
    if (event.combo.count >= 3 && boostCount[event.combo.charID] != 0)
    {
      timeRefill = std::min(timeRefill + 5000 * boostCount[event.combo.charID], maxTime);
      MoveArrow();
      showArrow = true;
    }
    boostCount[event.combo.charID] = 0;
    break;
  
  default:
    break;
  }
}

void TimerComponent::Update()
{
  if (done)
    return;

  arrowYMove.Update();
  refillArrow.setPosition(arrowPos);

  timeRemaining -= DELTA_TIME;

  if (timeRemaining > 0)
  {
    timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
    return;
  }

  if (timeRefill > 0)
  {
    timeRemaining = timeRefill;
    timeRefill = 0;
    MoveArrow();
    Event event;
    event.type = Event::Type::timerRefill;
    Event::events.push(event);
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
  if (!done && showArrow)
    win->draw(refillArrow);
}

void TimerComponent::AddTime(int addition)
{
  timeRemaining += std::min(timeRemaining + addition, maxTime);
  timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
}

void TimerComponent::MoveArrow()
{
  arrowYMove.Clear();
  arrowYMove.Push(EASE_IN_CURVE, 1000, arrowPos.y, 
    gauge.getPosition().y + gauge.getGlobalBounds().height * (0.5f - (float)timeRefill / (float)maxTime));
}
