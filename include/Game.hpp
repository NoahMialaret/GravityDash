#ifndef GAME_HPP
#define GAME_HPP

#include "Character.hpp"
#include "Controls.hpp"
#include "Entity.hpp"
#include "Score.hpp"
#include "SortedDblyLnkdList.hpp"
#include "Texture.hpp"
#include "World.hpp"

#include <iostream>
#include <memory>

struct GameConfig
{
  float speed = 1.0f;
  int targetSpawnChance = 90;
  int numCharacters = 1;
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
  Game(GameConfig &config);
  ~Game();

  // Handles game states, transitioning between states, and game objects
  void Update();
  // Renders game objects
  void Render() const;

  // Corrects the player's position based on the bouding playable region
  void CorrectCharacterPos(Character* player);
  // Lands the player and adds the total points collected to the score
  void HandleLandingSequence(Character* player);
  // Returns whether or not the game is over
  bool IsGameOver() const;

private:
  std::vector<std::unique_ptr<Character>> players; // The player characters
  std::unique_ptr<World> world = nullptr;          // The playable region of the game
  std::unique_ptr<Score> score = nullptr;          // The total accumulated score made during the game

  SortedDblyLnkdList<Entity> entities; // A sorted (by vertical position) linked list of entites
  Texture entityTex; // The sprite sheet used by all entities
  Texture charTex;

  Phase curPhase = Phase::standard;  // The current phase of gameplay
  Phase nextPhase = Phase::standard; // The next phase of gameplay to start after a transition has finished
  double BeginNextPhase;          // The time at which to begin transitioning

  double nextSpikeSpawnTimeMin = 0; // The minimum time at which the next spike should spawn

  bool gameOver = false; // Whether the game has been finished

  GameConfig config; // The configuration for various game variables
  // std::vector<worldTransitions>

  int timer = 0;

  // sf::RectangleShape timerRect;
};

#endif