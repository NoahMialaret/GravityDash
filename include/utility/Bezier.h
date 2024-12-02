#ifndef BEZIER_H
#define BEZIER_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <queue>
#include <vector>

// A class representing a Bezier curve as a queue of points
class Bezier
{
public:
  Bezier() = default;
  // Constructs `Bezier` with a queue of control points 
  Bezier(std::queue<sf::Vector2f>& points);
  // Constructs `Bezier` using a vector of points
  Bezier(std::vector<sf::Vector2f> points);

  // Returns the float value that is t * 100% along the curve
  float GetValue(float t) const;
  // Returns the point that is t * 100% along the curve
  sf::Vector2f GetPoint(float t) const;

private:
  std::queue<sf::Vector2f> points; // The control points that define the curve
};

#endif