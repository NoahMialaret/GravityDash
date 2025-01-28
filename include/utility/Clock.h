#ifndef CLOCK_H
#define CLOCK_H

#include <SFML/Graphics.hpp>

#define DELTA_TIME (Clock::GetInstance()->Delta())

// Clock is a global singleton class that tracks the amount of time passed between frames
class Clock
{
private:
  // Private constructor as only one instance should exist in the program
  Clock() = default;
  // The global Clock instance
  static Clock* instance;
  // Delete the Clock instance
  static void Clean();

public:
  // Creates and returns the global instance of Clock
  static Clock* GetInstance();
  
  // Updates the delta time based on when Update was last called
  void Update();

  // Gets the amount of time that has passed between Update calls (i.e. between frames)
  int Delta();
  // Gets the elapsed program time
  sf::Int32 Elapsed();

  // Sets the speed of the clock
  void SetSpeed(float speed);

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  sf::Clock clock;          // The clock used to keep track of the time
  sf::Int32 lastFrameTime;  // The elapsed time from when the Update function was last called

  float clockSpeed = 1.0f;  // How fast the clock runs relative to real time
  int deltaTime = 0;        // The time elapsed between frames
};

#endif