#ifndef ROUNDEDRECT_H
#define ROUNDEDRECT_H

#include "SFML/Graphics.hpp"

#include "Utility.h"
#include "Settings.h"

class RoundedRect
{
public:
  RoundedRect() = default;
  RoundedRect(sf::Vector2f centre, sf::Vector2f dim, sf::Color color);

  void SetCentre(sf::Vector2f newCentre);
  void SetVertical(float vertPos);
  void SetHorizontal(float horiPos);
  void Move(sf::Vector2f offset);
  void SetDim(sf::Vector2f newDim);
  void SetColour(sf::Color col);

  void Render(sf::RenderWindow* win) const;

private:
  sf::Vector2f centre;

  mutable sf::RectangleShape main;
  mutable sf::RectangleShape left;
  mutable sf::RectangleShape right;
};

#endif