#ifndef ROUNDEDRECT_H
#define ROUNDEDRECT_H

#include "SFML/Graphics.hpp"

#include "Utility.h"

class RoundedRect
{
public:
  RoundedRect() = default;
  RoundedRect(sf::Vector2f centre, sf::Vector2f dim, sf::Color color);

  void SetCentre(sf::Vector2f newCentre);
  void Move(sf::Vector2f offset);
  void SetDim(sf::Vector2f newDim);

  void Render(sf::RenderWindow* win) const;

private:
  sf::Vector2f centre;

  sf::RectangleShape main;
  sf::RectangleShape left;
  sf::RectangleShape right;
};

#endif