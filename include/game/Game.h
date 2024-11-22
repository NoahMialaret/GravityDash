#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>

#include "Character.h"
#include "Event.h"
#include "GameObject.h"
#include "World.h"
#include "ProgramSettings.h"
#include "Utility.h"

#include <iostream>
#include <list>
#include <memory>

// Handles logic and objects to be used in the main gameplay loop
class Game
{
public:
  // Constructs Game, the game world, and the characters given the number of characters in play
  Game(int numHumans, int numComputers);

  // Updates the game world, characters, and objects
  void Update();
  // Renders the game world, characters, and objects
  void Render(sf::RenderWindow* win) const;
  // Processes game related events
  void ProcessEvent(Event& event);

  // Returns whether or not the game is over
  bool IsGameOver() const;

  // Adds the given object to the objects list
  void SpawnObject(std::unique_ptr<GameObject> newObject);
  // Returns the number of characters in the game
  int NumCharacters() const;
  // Returns a pointer to the game world
  const World* GetWorld() const;

private:
  // Corrects the character positions based on the bouding playable region
  void CorrectCharacterPos(Character* character);

protected:
  std::unique_ptr<World> world = nullptr;                 // The playable region of the game
  std::vector<std::unique_ptr<Character>> characters;     // The characters controlled by users or computers
  std::forward_list<std::unique_ptr<GameObject>> objects; // A list of interactable game objects

  bool spawnersEnabled = true;  // Whether objects are able to be spawned

  bool gameOver = false; // Whether the game has been finished
};

#endif