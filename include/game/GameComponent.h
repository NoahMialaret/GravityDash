#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Event.h"

// `GameComponent` extends the functionality of `Game` by providing '
// modular game features such as scores and timers 
class GameComponent
{
public:
  // Constructs GameComponent with a reference to Game
  GameComponent(Game* game);

  // Processes relevent events
  virtual void ProcessEvent(Event& event){}
  // Updates functionality
  virtual void Update() = 0;
  // Renders drawables
  virtual void Render(sf::RenderWindow* win) const{};
  // Returns user relevent game statistics
  virtual void GetGameStats(){}

protected:
  Game* game; // A reference to a `Game` instance to allow access to important game information
};

#endif