#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Character.h"
#include "Controls.h"
#include "Entity.h"
#include "Number.h"
#include "SortedDblyLnkdList.h"
#include "GameTimer.h"
#include "World.h"

#include <list>
#include <iostream>
#include <memory>

enum class Mode
{
  title,
  time,
  survive
};

struct GameConfig
{
  float speed = 1.0f; // should be in utility
  int targetSpawnChance = 90;
  int numPlayers = 1;
  int numComputers = 0;
  int sawFrequency = 10;
  Mode mode = Mode::time;
  // std::vector<ids> transitionIDs
};

// Handles logic and objects to be used in the main gameplay loop
class Game
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
  Game(GameConfig& config);
  ~Game();

  // Handles game states, transitioning between states, and game objects
  void Update();
  // Renders game objects
  void Render(sf::RenderWindow *win) const;

  // Corrects the player's position based on the bouding playable region
  void CorrectCharacterPos(Character *player);
  // Lands the player and adds the total points collected to the score
  void HandleLandingSequence(Character *player);
  // Returns whether or not the game is over
  bool IsGameOver() const;

private:
  std::vector<std::unique_ptr<Character>> characters; // The player characters
  std::unique_ptr<World> world = nullptr;             // The playable region of the game
  std::unique_ptr<GameScore> score = nullptr;         // The total accumulated score made during the game

  SortedDblyLnkdList<Entity> entities; // A sorted (by vertical position) linked list of entites
  sf::Texture entityTex;               // The sprite sheet used by all entities

  std::list<TotalPoints> totalPoints; // Holds the

  std::unique_ptr<GameTimer> timer = nullptr; // The timer used by the game in 'time' mode

  Phase curPhase  = Phase::standard; // The current phase of gameplay
  Phase nextPhase = Phase::standard; // The next phase of gameplay to start after a transition has finished
  sf::Int32 BeginNextPhase;          // The time at which to begin transitioning

  sf::Int32 nextSpikeSpawnTimeMin = 0; // The minimum time at which the next spike should spawn

  bool gameOver = false; // Whether the game has been finished

  GameConfig config; // The configuration for various game variables
  // std::vector<worldTransitions>
};

#endif