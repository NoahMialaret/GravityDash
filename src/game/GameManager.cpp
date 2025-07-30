#include "GameManager.h"

GameManager::GameManager(Preset preset)
  :
  preset(preset)
{
  switch (preset)
  {
  case Preset::title:
    LoadTitlePreset();
    break;

  case Preset::minute:
    LoadMinutePreset();
    break;

  case Preset::rush:
    LoadRushPreset();
    break;
  
  case Preset::coop:
    LoadCoopPreset();
    break;
  
  case Preset::vs:
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

  if (game.get()->IsGameOver())
    return;
    
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
    comp.get()->ProcessEvent(event);
}

GameManager::Preset GameManager::GetPreset() const
{
  return preset;
}

void GameManager::LoadTitlePreset()
{
  game = std::make_unique<Game>(0, 4);
  components.push_front(std::make_unique<ObjectSpawnComponent<MovingTarget>>(game.get(), 200, 100, 0.9f));
}

void GameManager::LoadMinutePreset()
{
  game = std::make_unique<Game>(1, 0);

  components.push_front(std::make_unique<TimerComponent>(game.get(), 60000));
  components.push_front(std::make_unique<BoostComponent>(game.get(), 30000));
  components.push_front(std::make_unique<ObjectSpawnComponent<MovingTarget>>(game.get(), 150, 100, 0.9f));
  components.push_front(std::make_unique<ObjectSpawnComponent<Saw>>(game.get(), 1500, 500));
  components.push_front(std::make_unique<ScoreComponent>(game.get(), GetStatGameType()));
}

void GameManager::LoadRushPreset()
{
  LoadMinutePreset();
  
  components.push_front(std::make_unique<ObjectSpawnComponent<TimeBonus>>(game.get(),  5000, 1000));
}

void GameManager::LoadCoopPreset()
{
  game = std::make_unique<Game>(2, 0);
}

void GameManager::LoadVsPreset()
{
  game = std::make_unique<Game>(2, 0);
}

Stats::StatType GameManager::GetStatGameType() const
{
  switch (preset)
  {
  case Preset::minute:
    return Stats::StatType::minScores;

  case Preset::rush:
    return Stats::StatType::rushScores;

  case Preset::coop:
    return Stats::StatType::coopScores;
  
  default:
    break;
  }
  
  return Stats::StatType::null;
}
