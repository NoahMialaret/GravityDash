#ifndef SCALEHANDLER_H
#define SCALEHANDLER_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Utility.h"

#include <queue>

struct Scale
{
  Curve curve = Curve::linear;
  float duration = 0;
  // Could be a bezier curve itself
  sf::Vector2f start = DEFAULT_SCALE;
  sf::Vector2f end = DEFAULT_SCALE;
};

// Handles a sprite's scale changes
class ScaleHandler
{  
public:
  ScaleHandler() = default;
  // Constructor that takes a sprite pointer so that it can modify it's position
  ScaleHandler(sf::Sprite* sprite);

  // Updates the sprites position
  void Update();

  // Clears the queue of all scales
  void Clear();
  // Pushes a scale to the current scale queue
  void Queue(Scale& scale);
  void Queue(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end);

  // Returns the scale the sprite will be at after all current scale changes have finished
  sf::Vector2f GetEndScale() const;

private:
  // Pops the current scale so that the next scale is the active one
  void NextScale();

private:
  sf::Sprite* sprite;

  Scale* cur = nullptr;
  std::queue<Scale> scales; // A queue of scales, a scale is dequed when the timer has reached 0

  float timer = 0.0f; // The amount of time left in the current scale
};

#endif