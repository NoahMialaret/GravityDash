#include "BoostComponent.h"

BoostComponent::BoostComponent(Game* game, int limit)
  :
  GameComponent(game),
  boosts(game->NumCharacters())
{
  for (int i = 0; i < boosts.size(); i++)
    boosts[i] = BoostMeter(game, i, limit);
}

void BoostComponent::ProcessEvent(Event& event)
{
  switch (event.type)
  {
  case Event::Type::playerSuper:
    boosts[event.data.value].Clear();
    break;

  case Event::Type::playerCombo:
    if (event.data.combo.count >= 2 && !event.data.combo.wasSuperJump)
      boosts[event.data.combo.charID].Increment(2000);
    break;
  
  default:
    break;
  }
}

void BoostComponent::Update()
{
  for (auto& boost : boosts)
    boost.Update();
}

void BoostComponent::Render(sf::RenderWindow* win) const
{
  for (auto& boost : boosts)
    boost.Render(win);
}

BoostComponent::BoostMeter::BoostMeter(Game *game, int id, int limit)
  :
  id(id),
  limit(limit)
{
  Utility::InitSprite(gauge, "boost_meter", ZERO_VECTOR, {1, 1}, {0.5f, 0.0f});

  fill.setFillColor(sf::Color(255, 229, 181));
  fill.setSize({0.0f, 2.0f * ProgramSettings::gameScale});

  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    gauge.setPosition(pos);
    fill.setPosition(pos - sf::Vector2f(gauge.getGlobalBounds().width / 2 - ProgramSettings::gameScale, 0.0f));
  };
  game->Attach(World::AttachPoint((int)World::AttachPoint::topLeft + id), updatePosFunction);
  
  // Meters placed at the top of the world need to be flipped vertically
  if (id < 2)
  {
    gauge.scale({1.0f, -1.0f});
    fill.scale({1.0f, -1.0f});
  }
}

void BoostComponent::BoostMeter::Update()
{
  if (fillAmount >= limit)
    return;
  
  fillAmount = std::max(fillAmount - DELTA_TIME, 0);

  int width = (gauge.getTextureRect().width - 2.0f) * (float)fillAmount / (float)limit;
  fill.setSize(ProgramSettings::gameScale * sf::Vector2f((float)width, 2.0f));
}

void BoostComponent::BoostMeter::Render(sf::RenderWindow* win) const
{
  win->draw(fill);
  win->draw(gauge);
}

void BoostComponent::BoostMeter::Increment(int amount)
{
  if (fillAmount >= limit)
    return;

  fillAmount += amount;
  if (fillAmount >= limit)
  {
    fill.setSize(ProgramSettings::gameScale * sf::Vector2f(gauge.getTextureRect().width - 2.0f, 2.0f));
    
    PUSH_EVENT(Event::Type::boostFull, {.value = id});
  }
}

void BoostComponent::BoostMeter::Clear()
{
  fillAmount = 0;
  fill.setSize({0.0f, 2.0f * ProgramSettings::gameScale});
}
