#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>

#include "BoostComponent.h"
#include "Event.h"
#include "Game.h"
#include "GameComponent.h"
#include "ObjectSpawnComponent.h"
#include "ScoreComponent.h"
#include "TimerComponent.h"
#include "Settings.h"

#include <memory>
#include <list>

// Manages the creation and interaction of `Game` and its components
class GameManager
{
public:
  // The different preset options to create a game with, determines which components are used
  enum class Preset
  {
    title,
    minute,
    rush,
    coop,
    vs,
    null
  };

public:
  // Initialises the game and its components using a preset
  GameManager(Preset preset);

  // Updates the game and its components
  void Update();
  // Renders the game and its components
  void Render(sf::RenderWindow* win) const;

  // Precesses events relevant to gameplay, such as collisions
  void ProcessEvents(Event& event);

  // Returns the preset used to initialise the game
  Preset GetPreset() const;

private:
  // Loads the game configuration used as background on menus
  void LoadTitlePreset();
  // Loads the game configuration used for the "1-minute" mode
  void LoadMinutePreset();
  // Loads the game configuration used for the "Rush" mode
  void LoadRushPreset();
  // Loads the game configuration used coop play
  void LoadCoopPreset();
  // Loads the game configuration used for versus play
  void LoadVsPreset();

  // Returns the `StatType` associated with the preset chosen
  Stats::StatType GetStatGameType() const;

private:
  Preset preset; // The preset used to create the game, also used for statistics

  std::unique_ptr<Game> game; // The main game class, holds game critical components like players and game objects
  std::forward_list<std::unique_ptr<GameComponent>> components; // Holds non-critical, modular components to extend game functionality
};

#endif