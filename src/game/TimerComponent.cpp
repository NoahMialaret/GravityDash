#include "TimerComponent.h"

TimerComponent::TimerComponent(Game* game, int maxTime)
  :
  GameComponent(game),
  timeRemaining(maxTime),
  maxTime(maxTime),
  boostCount(game->NumCharacters(), 0)
{
  refillArrow = Entity("arrow", nullptr, (sf::Vector2i)Textures::textures.at("arrow").getSize());

  timeRect = sf::RectangleShape(ProgramSettings::gameScale * sf::Vector2f(4.0f, 60.0f));
  timeRect.setFillColor(sf::Color(255, 229, 181));
  timeRect.setScale(sf::Vector2f(1.0f, -1.0f));

  Utility::InitSprite(gauge, "timer", ZERO_VECTOR, {1, 1}, {0.0f, 0.5f});

  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    gauge.setPosition(pos);
    timeRect.setPosition(pos + sf::Vector2f(0.0f, 0.5f * gauge.getGlobalBounds().height - ProgramSettings::gameScale));
    MoveArrow(pos); // TODO: change so only X value is attached
  };
  game->Attach(World::AttachPoint::right, updatePosFunction);
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
      MoveArrow(gauge.getPosition());
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

  refillArrow.Update();

  timeRemaining -= Clock::Delta();

  if (timeRemaining > 0)
  {
    timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
    return;
  }

  if (timeRefill > 0)
  {
    timeRemaining = timeRefill;
    timeRefill = 0;
    MoveArrow(gauge.getPosition());
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
    refillArrow.Render(win);
}

void TimerComponent::AddTime(int addition)
{
  timeRemaining += std::min(timeRemaining + addition, maxTime);
  timeRect.setSize(ProgramSettings::gameScale * sf::Vector2f(4.0f, (int)((gauge.getTextureRect().height - 1) * timeRemaining / maxTime)));
}

void TimerComponent::MoveArrow(sf::Vector2f reference)
{
  refillArrow.ClearHandlers();
  refillArrow.QueueMotion(Curve::easeIn, 1000, refillArrow.GetPosition(), {reference + sf::Vector2f(SCALED_DIM, gauge.getGlobalBounds().height * (0.5f - (float)timeRefill / (float)maxTime))});
}
