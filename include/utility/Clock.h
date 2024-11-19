#ifndef CLOCK_H
#define CLOCK_H

#include <SFML/Graphics.hpp>

// The clock used by the program to track the amount of time between frames
class Clock
{
public:
  // Initialises the lastFrameTime variable
  static void Init();
  // Updates the deltaTime variable
  static void Update();
  // Gets the amount of time that has passed between frames
  static int Delta();
  // Gets the elapsed program time
  static sf::Int32 Elapsed();

  // Sets the speed of the clock
  static void SetSpeed(float speed);

private:
  static sf::Clock clock; // The clock used to keep track of the time
  static sf::Int32 lastFrameTime; // The clock value on the last program frame

  static float clockSpeed; // How fast the clock runs relative to real time
  static int deltaTime; // The time elapsed between frames
};

#endif