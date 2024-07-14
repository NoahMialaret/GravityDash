#ifndef PLAYERBOOST_H
#define PLAYERBOOST_H

#include <SFML/Graphics.hpp>

#include "Utility.h"
#include "Textures.h"

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
  
  sf::Int32 timer;
  sf::Int32 lastFrameTime;

  int boostAmount = 0;
};

#endif