#include "GameManager.h"

GameManager::GameManager(Event::GamePreset preset)
  :
  preset(preset)
{
  switch (preset)
  {
  case Event::GamePreset::title:
    LoadTitlePreset();
    break;

  case Event::GamePreset::minute:
    LoadMinutePreset();
    break;

  case Event::GamePreset::rush:
    LoadRushPreset();
    break;
  
  case Event::GamePreset::coop:
    LoadCoopPreset();
    break;
  
  case Event::GamePreset::vs:
    LoadVsPreset();
    break;
  
  default:
    // Custom games are not currently supported
    break;
  }
}

void GameManager::Update()
{
  game.get()->Update();
  for (auto& comp : components)
    comp.get()->Update();
}

void GameManager::Render(sf::RenderWindow* win) const
{
  game.get()->Render(win);
  for (auto& comp : components)
    comp.get()->Render(win);
}

void GameManager::ProcessEvents(Event& event)
{
  game.get()->ProcessEvent(event);
  for (auto& comp : components)
    comp.get()->ProcessEvents(event);
}

Event::GamePreset GameManager::GetPreset() const
{
  return preset;
}

void GameManager::LoadTitlePreset()
{
  game = std::make_unique<Game>(0, 4);
  components.push_front(std::make_unique<BoostComponent>(game.get(), 30000));
}

void GameManager::LoadMinutePreset()
{
  game = std::make_unique<Game>(1, 0);

  components.push_front(std::make_unique<TimerComponent>(game.get(), 5000));
  components.push_front(std::make_unique<BoostComponent>(game.get(), 30000));
}

void GameManager::LoadRushPreset()
{
  game = std::make_unique<Game>(1, 0);
}

void GameManager::LoadCoopPreset()
{
  game = std::make_unique<Game>(1, 1);
}

void GameManager::LoadVsPreset()
{
  game = std::make_unique<Game>(1, 1);
}
