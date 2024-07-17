#ifndef MOTIONHANDLER_H
#define MOTIONHANDLER_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Utility.h"

#include <queue>

struct Motion
{
  Curve curve = Curve::linear;
  float duration = 0;
  // Could be a bezier curve itself
  Bezier points;
};

// Handles a sprite's motion throughout the game world
class MotionHandler
{  
public:
  MotionHandler() = default;
  // Constructor that takes a sprite pointer so that it can modify it's position
  MotionHandler(sf::Sprite* sprite);

  // Updates the sprites position
  void Update();

  // Clears the queue of all motions
  void Clear();
  // Pushes a motion to the current motion queue
  void Queue(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end);
  void Queue(Curve curve, float duration, Bezier points);
  void Queue(Motion& motion);

  // Returns the position the sprite will be after all current motions finished
  sf::Vector2f GetEndPoint() const;

private:
  // Pops the current motion so that the next motion is the active one
  void NextMotion();

private:
  sf::Sprite* sprite;

  Motion* cur = nullptr;
  std::queue<Motion> motions; // A queue of motions, a motion is dequed when the timer has reached 0

  float timer = 0.0f; // The amount of time left in the current motion
};

#endif