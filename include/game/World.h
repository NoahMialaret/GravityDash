#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Utility.h"
#include "ProgramSettings.h"

#include <iostream>

#define DEFAULT_WORLD_SIZE sf::Vector2i(16, 8)

// A class representing the playable region of the game
class World
{
public:
  // Constructs World based on the given (tile) dimensions
  World(sf::Vector2i size = DEFAULT_WORLD_SIZE);
  // Updates the playable region of the world if the game is currently transitioning to a different state
  void Update();
  // Renders the playable region as a rectangle
  void Render(sf::RenderWindow* win) const;

  // Gets the playable region of the game
  sf::IntRect GetRegion() const;
  // Changes the desired size of the region based upon the left position on the region
  void SetTargetLeft(int targetWorldWidth);
  // Returns whether the world has finished changing the region size if it was previously transitioning
  bool FinishedTransitioning() const;

private:
  // Could use a scale handler for resizing
  sf::IntRect playableRegion;     // The playable region of the game
  sf::RectangleShape renderRect;  // The rectangle to represent the world region

  int targetLeft = 0;             // The new desired left position of the region if a transition is happening
  bool isTransitioning = false;   // Whether or not the world is currently changing sizes
};

#endif