#ifndef BEZIER_H
#define BEZIER_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <queue>
#include <vector>

class Bezier
{
public:
  Bezier() = default;
  Bezier(std::vector<sf::Vector2f> points);
  float GetValue(float t, bool invert = false) const;
  sf::Vector2f GetPoint(float t, bool invert = false) const;
private:
  std::vector<sf::Vector2f> points;
};

#endif