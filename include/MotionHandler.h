#ifndef MOTIONHANDLER_H
#define MOTIONHANDLER_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Utility.h"

#include <queue>

struct Motion;

// Handles a sprite's motion throughout the game world
class MotionHandler
{
public:
  enum class Type
  {
    linear,
    easeIn,
    easeOut
  };
  
public:
  MotionHandler() = default;
  // Constructor that takes a sprite pointer so that it can modify it's position
  MotionHandler(sf::Sprite* sprite);

  // Updates the sprites position
  void Update();
  // void Update(sf::Sprite* sprite);

  // Clears the queue of all motions
  void Clear();
  // Pushes a motion to the current motion queue
  void QueueMotion(Motion& motion);
  void QueueMotion(Type type, float duration, sf::Vector2f start, sf::Vector2f end);

private:
  // Pops the current motion so that the next motion is the active one
  void NextMotion();

private:
  sf::Sprite* sprite;

  Motion* cur = nullptr;
  std::queue<Motion> motions; // A queue of motions, a motion is dequed when the timer has reached 0
  static std::vector<Bezier> curves; // The bezier curves used to determine the timing of the motion

  float timer = 0.0f; // The amount of time left in the current motion
};

struct Motion
{
  MotionHandler::Type type = MotionHandler::Type::linear;
  float duration = 0;
  // Could be a bezier curve itself
  sf::Vector2f startPoint = ZERO_VECTOR;
  sf::Vector2f endPoint = ZERO_VECTOR;
};

#endif