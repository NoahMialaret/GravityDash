#ifndef PLAYERBOOST_H
#define PLAYERBOOST_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Textures.h"
#include "Utility.h"

#include <vector>

class PlayerBoost
{
public:
  PlayerBoost() = default;
  PlayerBoost(int numTabs, sf::Vector2f topRight);

  void Update();
  void Render(sf::RenderWindow* win) const;

  void Increment();
  void Decrement();

  void Clear();

  bool IsFull() const;

private:
  std::vector<sf::Sprite> tabs;
  std::vector<sf::RectangleShape> tabTimer;
  
  int timer;

  int boostAmount = 0;
};

#endif