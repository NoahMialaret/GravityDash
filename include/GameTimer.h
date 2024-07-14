#ifndef GAMETIMER_H
#define GAMETIMER_H

#include <SFML/Graphics.hpp>

#include "Textures.h"
#include "Utility.h"

#include <iostream>

// A class to display and keep track of the time remaining in a game
class GameTimer
{
public:
  // Create a timer with the desired time duration (in ms), and the bottom left position of the time
  GameTimer(sf::Int32 time, sf::Vector2f bottomLeft);
  // Update the amount of time remaining, returns true if the timer has finished
  bool Update();
  // Renders the timer gauge and the time remaining
  void Render(sf::RenderWindow* win) const;

  // Add or subtracts time from the timer
  void AddTime(sf::Int32 addition);

  // Pauses the timer so that it no longer ticks when Update() is called
  void Pause();
  // Updauses the timer so that it can resume ticking
  void Unpause();
  // Returns whether or not the timer is paused
  bool IsPaused() const;

private:
  sf::Sprite sprite;   // The sprite for the timer gauge

  bool paused = false; // Whether the timer is paused

  sf::RectangleShape timeRect; // The rectangle used to display how much time is left

  sf::Int32 maxTime = 0;       // The maximum amount of time the timer can hold
  sf::Int32 timeRemaining = 0; // The amount of time left in the timer
  sf::Int32 lastFrameTime = 0; // The clock time of the previous frame, used to determine how much time has passed
};

#endif