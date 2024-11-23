#ifndef TIMERCOMPONENT_H
#define TIMERCOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Event.h"
#include "Game.h"
#include "GameComponent.h"
#include "ProgramSettings.h"
#include "Utility.h"
#include "World.h"

#include <functional>

// A GameComponent providing timer functionality, which can be refilled under certain conditions
class TimerComponent : public GameComponent
{
public:
  // Create a timer with the desired time duration (in milliseconds)
  TimerComponent(Game* game, int maxTime);
  // Update the amount of time remaining, sends out events if the timer is refilled or runs out
  void Update() override;
  // Renders the timer gauge
  void Render(sf::RenderWindow* win) const override;

private:
  // Add or subtracts time from the timer
  void AddTime(int addition);

private:
  sf::Sprite gauge;             // The sprite for the timer gauge
  sf::RectangleShape timeRect;  // The rectangle used to display how much time is left

  const int maxTime = 0;  // The maximum amount of time the timer can hold (in milliseconds)
  int timeRemaining = 0;  // The amount of time left in the timer (in milliseconds)

  bool done = false;  // Whether the timer is done
};

#endif