#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Event.h"

class GameComponent
{
public:
  GameComponent(Game* game);

  virtual void Update() = 0;
  virtual void Render(sf::RenderWindow* win) const = 0;
  virtual void ProcessEvents(Event& event){}
  // Returns game relevant stats specific to this component (e.g. score)
  virtual void GetGameStats(){}

protected:
  Game* game;
};

#endif