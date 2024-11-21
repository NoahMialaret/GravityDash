#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Character.h"
#include "Clock.h"
#include "Controls.h"
#include "Event.h"
#include "GameObject.h"
#include "GameStats.h"
#include "GameTimer.h"
#include "Keyboard.h"
#include "Number.h"
#include "PlayerBoost.h"
#include "Textures.h"
#include "World.h"
#include "ProgramSettings.h"

#include <iostream>
#include <list>
#include <memory>

// Handles logic and objects to be used in the main gameplay loop
class Game
{
public:
  Game(Event::GameConfig& config);
  ~Game();

  // Handles game states, transitioning between states, and game objects
  virtual void Update();
  // Attempts to spawn game objects using RNG
  virtual void SpawnObjects();
  // Renders game objects
  virtual void Render(sf::RenderWindow *win) const;

  // Corrects the player's position based on the bouding playable region
  void CorrectCharacterPos(Character *player);
  // Returns whether or not the game is over
  bool IsGameOver() const;

  Event::GameConfig GetConfig() const;

protected:
  // std::unique_ptr<Mode> gameMode = nullptr;

  std::vector<std::unique_ptr<Character>> characters; // The player characters
  std::unique_ptr<World> world = nullptr;             // The playable region of the game

  std::forward_list<std::unique_ptr<GameObject>> objects; // A list of game objects

  int spawnTimer = 16;     // Timer for when a target should be attempted to spawn
  int spikeSpawnTimer = 0; // The time at which the next spike should spawn

  bool gameOver = false; // Whether the game has been finished

  bool canSpawnObjects = true;

  Event::GameConfig config; // The configuration for various game variables
};

class Min : public Game
{
public:
  Min(Event::GameConfig& config);
  virtual void Update() override;
  virtual void Render(sf::RenderWindow* win) const override;
  virtual void UpdateTimer();

protected:
  std::unique_ptr<GameScore> score = nullptr; // The total accumulated score made during the game
  std::unique_ptr<GameTimer> timer = nullptr; // The timer used by the game
  bool timeUp = false;
};

class Rush : public Min
{
public:
  Rush(Event::GameConfig& config);
  void Update() override;
  void Render(sf::RenderWindow* win) const override;
  void UpdateTimer() override;
  void SpawnObjects() override;

protected:
  int timeBonusTimer = 16;
  int timeBonusCooldown = 0;
  int phase = 0;
  int storedTime = 0; // How much time to put back into the timer when it has run out

  Entity arrow; // The arrow representing visually how much time will be put into the clock
  sf::Vector2f arrowBottom; // The position the arrow starts at, and gets reset to

  sf::Text multiplierText;
};

class Wild : public Game
{
public:
  // The different phases of gameplay that can happen
  enum class Phase
  {
    transition,
    standard,
    skinny,
    spikeDodge
  };

public:
  Wild(Event::GameConfig& config);
  void Update() override;
  void Render(sf::RenderWindow* win) const override;

protected:  
  Phase curPhase  = Phase::standard; // The current phase of gameplay
  Phase nextPhase = Phase::standard; // The next phase of gameplay to start after a transition has finished
  sf::Int32 BeginNextPhase;          // The time at which to begin transitioning
};

#endif