#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Event.h"
#include "Textures.h"
#include "Utility.h"
#include "ProgramSettings.h"
#include "Game.h"
#include "GameComponent.h"

#include <functional>
#include <iostream>

// A class to display and keep track of the time remaining in a game
class TimerComponent : public GameComponent
{
public:
  // Create a timer with the desired time duration (in ms), and the bottom left position of the time
  TimerComponent(Game* game, int maxTime);
  // Update the amount of time remaining, sends out events if the timer is refilled or runs out
  void Update() override;
  // Renders the timer gauge
  void Render(sf::RenderWindow* win) const override;

private:
  // Add or subtracts time from the timer
  void AddTime(int addition);

private:
  sf::Sprite sprite;  // The sprite for the timer gauge

  bool done = false;  // Whether the timer is done

  sf::RectangleShape timeRect; // The rectangle used to display how much time is left

  int maxTime = 0;       // The maximum amount of time the timer can hold
  int timeRemaining = 0; // The amount of time left in the timer
};

#endif