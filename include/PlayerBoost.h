#ifndef PLAYERBOOST_H
#define PLAYERBOOST_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Entity.h"
#include "Textures.h"
#include "Utility.h"

#include <vector>

class PlayerBoost
{
public:
  PlayerBoost() = default;
  PlayerBoost(sf::Vector2f bottomLeft, int limit = 30000);

  void Update();
  void Render(sf::RenderWindow* win) const;

  void Increment(int amount);

  void Clear();

  bool IsFull() const;

private:
  Entity meter;
  sf::RectangleShape fill;
  
  int timer = 0;
  int limit = 30000;
};

#endif