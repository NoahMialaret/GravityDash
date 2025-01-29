#include "Bezier.h"

Bezier::Bezier(std::queue<sf::Vector2f>& points)
  :
  points(points)
{
  if (points.empty())
    this->points.push({0.0f, 0.0f});
}

Bezier::Bezier(std::vector<sf::Vector2f> points)
{
  if (points.empty())
    this->points.push({0.0f, 0.0f});

  for (auto& point : points)
    this->points.push(point);
}

float Bezier::GetValue(float t) const
{
  return GetPoint(t).y;
}

sf::Vector2f Bezier::GetPoint(float t) const
{
  if (t <= 0.0f)
    return points.front();
  if (t >= 1.0f)
    return points.back();

  std::queue<sf::Vector2f> subPoints = points;

  int edgeIndex = 0;
  int nextCurveStart = subPoints.size() - 1;

  while (subPoints.size() > 1)
  {
    sf::Vector2f temp = subPoints.front();
    subPoints.pop();
    // For edge (A,B), calculates new point as A + t(B - A)
    subPoints.push(temp + t * (subPoints.front() - temp));

    edgeIndex++;
    if (edgeIndex >= nextCurveStart) // Processed the last edge in the sub curve
    {
      nextCurveStart--;
      edgeIndex = 0;
      subPoints.pop();
    }
  }

  return subPoints.front();
}
