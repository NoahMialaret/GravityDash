#ifndef ROTATIONHANDLER_H
#define ROTATIONHANDLER_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Utility.h"

#include <queue>

struct Rotation
{
  Curve curve = Curve::linear;
  float duration = 0;
  float start = 0.0f;
  float end = 0.0f;
};

// Handles a sprite's rotation throughout the game world
class RotationHandler
{  
public:
  RotationHandler() = default;
  // Constructor that takes a sprite pointer so that it can modify it's rotation
  RotationHandler(sf::Sprite* sprite);

  // Updates the sprites rotation
  void Update();

  // Clears the queue of all rotations
  void Clear();
  // Pushes a rotation to the current rotation queue
  void Queue(Rotation& rotation);
  void Queue(Curve curve, float duration, float start, float end);

  // Returns the rotation amount of the sprite after all rotations have been applied
  float GetEndRotation() const;

private:
  // Pops the current rotation so that the next rotation is the active one
  void NextRotation();

private:
  sf::Sprite* sprite;

  Rotation* cur = nullptr;
  std::queue<Rotation> rotations; // A queue of rotations, a rotation is dequed when the timer has reached 0

  float timer = 0.0f; // The amount of time left in the current rotation
};

#endif