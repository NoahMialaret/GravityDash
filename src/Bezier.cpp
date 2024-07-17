#include "Bezier.h"

Bezier::Bezier(std::vector<sf::Vector2f> points)
  :
  points(points)
{
  if (points.empty())
  {
    std::cout << "Warning, attempted to make a Bezier with no points!\n";
    this->points.push_back({0,0});
  }
}

float Bezier::GetValue(float t, bool invert)
{
  if (t <= 0.0f)
  {
    return invert ? points.back().y : points.front().y;
  }
  else if (t >= 1.0f)
  {
    return invert ? points.front().y : points.back().y;
  }

  std::queue<sf::Vector2f> subPoints;
  for (int i = (invert ? points.size() - 1 : 0); 
      (invert ? i >= 0 : i < points.size()); 
      (invert ? i-- : i++))
  {
    subPoints.push(points[i]);
  }

  int i = 0;
  int iMax = subPoints.size() - 1;

  while (subPoints.size() > 1)
  {
    sf::Vector2f temp = subPoints.front();
    subPoints.pop();
    // A + t(B - A)
    subPoints.push(temp + t * (subPoints.front() - temp));

    i++;
    if (i >= iMax)
    {
      iMax--;
      i = 0;
      subPoints.pop();
    }
  }

  return subPoints.front().y;
}