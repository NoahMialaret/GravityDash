#include "TimerComponent.h"

TimerComponent::TimerComponent(Game* game, int maxTime)
  :
  GameComponent(game),
  arrowYMove(&arrowPos.y),
  timeRemaining(maxTime),
  maxTime(maxTime),
  boostCount(game->NumCharacters(), 0)
{
  Utility::InitSprite(refillArrow, "arrow");

  timeRect = sf::RectangleShape(sf::Vector2f(4.0f, 60.0f));
  timeRect.setFillColor(LIGHTEST);
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  Utility::InitSprite(gauge, "timer", {1, 1}, {0.0f, 0.5f});

  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    gauge.setPosition(pos);
    timeRect.setPosition(pos + sf::Vector2f(0.0f, 0.5f * gauge.getGlobalBounds().height - 1.0f));
    arrowPos.x = pos.x + SPRITE_DIM;
  };
  game->Attach(World::AttachPoint::right, updatePosFunction);
  MoveArrow();
}

void TimerComponent::ProcessEvent(Event &event)
{
  switch (event.type)
  {
  case Event::Type::collisionTimeBonus:
    boostCount[event.data.collision.charID]++;
    break;

  case Event::Type::playerCombo:
    if (event.data.combo.count >= 3 
        && boostCount[event.data.combo.charID] != 0)
    {
      timeRefill = std::min(timeRefill + 5000 * boostCount[event.data.combo.charID], maxTime);
      MoveArrow();
      showArrow = true;
    }
    boostCount[event.data.combo.charID] = 0;
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
    timeRect.setSize(sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
    return;
  }

  if (timeRefill > 0)
  {
    timeRemaining = timeRefill;
    timeRefill = 0;
    MoveArrow();

    PUSH_EVENT(Event::Type::timerRefill);
    
    return;
  }
  
  timeRect.setSize(ZERO_VECTOR);
  timeRemaining = 0;
  done = true;

  PUSH_EVENT(Event::Type::gameTimeUp);
}

void TimerComponent::Render(sf::RenderWindow* win) const
{
  Utility::RenderRectWithScale(win, timeRect, &STATIC_SHADER);
  Utility::RenderSpriteWithScale(win, gauge, &STATIC_SHADER);
  if (!done && showArrow)
    Utility::RenderSpriteWithScale(win, refillArrow, &STATIC_SHADER);
}

void TimerComponent::AddTime(int addition)
{
  timeRemaining += std::min(timeRemaining + addition, maxTime);
  timeRect.setSize(sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
}

void TimerComponent::MoveArrow()
{
  arrowYMove.Clear();
  arrowYMove.Push(EASE_IN_CURVE, 1000, arrowPos.y, 
    gauge.getPosition().y + gauge.getGlobalBounds().height * (0.5f - (float)timeRefill / (float)maxTime));
}
