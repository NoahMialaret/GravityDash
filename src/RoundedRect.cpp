#include "RoundedRect.h"

RoundedRect::RoundedRect(sf::Vector2f centre, sf::Vector2f dim, sf::Color color)
  :
  centre(centre)
{
  main.setSize(dim - Utility::gameScale * sf::Vector2f(2.0f, 0.0f));
  main.setPosition(centre);
  main.setFillColor(color);
  main.setOrigin(main.getGlobalBounds().width / 2, main.getGlobalBounds().height / 2);

  left.setSize({Utility::gameScale, dim.y - 2.0f * Utility::gameScale});
  left.setPosition({centre.x - dim.x / 2.0f + Utility::gameScale, centre.y});
  left.setFillColor(color);
  left.setOrigin(left.getGlobalBounds().width, left.getGlobalBounds().height / 2);

  right.setSize({Utility::gameScale, dim.y - 2 * Utility::gameScale});
  right.setPosition({centre.x + dim.x / 2.0f - Utility::gameScale, centre.y});
  right.setFillColor(color);
  right.setOrigin(0, right.getGlobalBounds().height / 2);
}

void RoundedRect::SetCentre(sf::Vector2f newCentre)
{
  Move(newCentre - centre);
}

void RoundedRect::Move(sf::Vector2f offset)
{  
  main.move(offset);
  left.move(offset);
  right.move(offset);
  centre += offset;
}

void RoundedRect::SetDim(sf::Vector2f newDim)
{
  main.setSize(newDim - Utility::gameScale * sf::Vector2f(2.0f, 0.0f));
  main.setOrigin(main.getGlobalBounds().width / 2, main.getGlobalBounds().height / 2);

  left.setSize({Utility::gameScale, newDim.y - 2.0f * Utility::gameScale});
  left.setPosition({centre.x - newDim.x / 2.0f + Utility::gameScale, centre.y});
  left.setOrigin(left.getGlobalBounds().width, left.getGlobalBounds().height / 2);

  right.setSize({Utility::gameScale, newDim.y - 2 * Utility::gameScale});
  right.setPosition({centre.x + newDim.x / 2.0f - Utility::gameScale, centre.y});
  right.setOrigin(0, right.getGlobalBounds().height / 2);
}

void RoundedRect::Render(sf::RenderWindow* win) const
{
  win->draw(main);
  win->draw(left);
  win->draw(right);
}
