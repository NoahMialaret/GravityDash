#ifndef WORLD_H
#define WORLD_H

#include <SFML/Graphics.hpp>

#include "Attachment.h"
#include "Settings.h"
#include "Utility.h"

#include <functional>
#include <iostream>

// The default size to initialise the world at
#define DEFAULT_WORLD_SIZE sf::Vector2f(16.0f, 8.0f)

// A class representing the playable region of the game, centred at the origin (0,0)
class World
{
public:
  // The positions on the world border that components can be attached to
  enum class AttachPoint
  {
    left,
    right,
    top,
    bottom,
    topLeft,
    topRight,
    bottomLeft,
    bottomRight,
    null
  };

public:
  // Constructs World based on the given size (in tiles)
  World(sf::Vector2f size = DEFAULT_WORLD_SIZE);
  // Updates the world size if it is currently changing size
  void Update();
  // Renders the playable region as a rectangle
  void Render(sf::RenderWindow* win) const;

  // Gets the playable bounds of the game
  const sf::Vector2f& GetBounds() const;
  // Gets a pointer to a specified attachment point
  void Attach(AttachPoint point, std::function<void(sf::Vector2f)>& function);
  // Forces a specified world attachment to update
  void UpdateAttachment(World::AttachPoint point);

private:
  sf::Vector2f bounds;            // The bounds of the world
  // mutable sf::RectangleShape renderRect;  // The rectangle used to render the world

  std::vector<Attachment> attachments{(int)AttachPoint::null}; // Points on the world border that components can attach to
};

#endif