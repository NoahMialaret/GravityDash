#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Character.h"
#include "Clock.h"
#include "Controls.h"
#include "Event.h"
#include "GameObject.h"
#include "GameStats.h"
#include "Keyboard.h"
#include "Number.h"
#include "BoostComponent.h"
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
  Game(int numHumans, int numComputers);
  ~Game();

  // Handles game states, transitioning between states, and game objects
  void Update();
  // Renders game objects
  void Render(sf::RenderWindow* win) const;

  // Returns whether or not the game is over
  bool IsGameOver() const;

  void ProcessEvent(Event& event);

  void SpawnObject(std::unique_ptr<GameObject> newObject);
  int NumPlayers() const;
  const World* GetWorld() const;

private:
  // Corrects the player's position based on the bouding playable region
  void CorrectCharacterPos(Character* player);
  void SpawnObjects();

protected:
  // std::unique_ptr<Mode> gameMode = nullptr;
  std::unique_ptr<World> world = nullptr;             // The playable region of the game

  std::vector<std::unique_ptr<Character>> characters; // The player characters
  std::forward_list<std::unique_ptr<GameObject>> objects; // A list of game objects

  bool spawnersEnabled = true;

  int spawnTimer = 16;     // Timer for when a target should be attempted to spawn
  int spikeSpawnTimer = 0; // The time at which the next spike should spawn

  bool gameOver = false; // Whether the game has been finished
};

// class Min : public Game
// {
// public:
//   Min(Event::GameConfig& config);
//   virtual void Update() override;
//   virtual void Render(sf::RenderWindow* win) const override;
//   virtual void UpdateTimer();

// protected:
//   std::unique_ptr<GameScore> score = nullptr; // The total accumulated score made during the game
//   bool timeUp = false;
// };

// class Rush : public Min
// {
// public:
//   Rush(Event::GameConfig& config);
//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;
//   void UpdateTimer() override;
//   void SpawnObjects() override;

// protected:
//   int timeBonusTimer = 16;
//   int timeBonusCooldown = 0;
//   int phase = 0;
//   int storedTime = 0; // How much time to put back into the timer when it has run out

//   Entity arrow; // The arrow representing visually how much time will be put into the clock
//   sf::Vector2f arrowBottom; // The position the arrow starts at, and gets reset to

//   sf::Text multiplierText;
// };

// class Wild : public Game
// {
// public:
//   // The different phases of gameplay that can happen
//   enum class Phase
//   {
//     transition,
//     standard,
//     skinny,
//     spikeDodge
//   };

// public:
//   Wild(Event::GameConfig& config);
//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// protected:  
//   Phase curPhase  = Phase::standard; // The current phase of gameplay
//   Phase nextPhase = Phase::standard; // The next phase of gameplay to start after a transition has finished
//   sf::Int32 BeginNextPhase;          // The time at which to begin transitioning
// };

#endif