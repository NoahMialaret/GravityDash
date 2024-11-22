#include "World.h"

World::World(sf::Vector2f size)
  :
  bounds(0.5f * SCALED_DIM * size)
{
  float outline = ProgramSettings::gameScale;
  // TODO: fix world rendering
  renderRect = sf::RectangleShape(SCALED_DIM * size);
  renderRect.setOrigin(0.5f * SCALED_DIM * size);
  renderRect.setOutlineThickness(outline);

  attachments[(int)AttachPoint::left]  = Attachment({-bounds.x - outline, 0.0f});
  attachments[(int)AttachPoint::right] = Attachment({bounds.x + outline, 0.0f});

  attachments[(int)AttachPoint::topLeft]     = Attachment({-0.5f * bounds.x, -bounds.y - outline});
  attachments[(int)AttachPoint::topRight]    = Attachment({0.5f * bounds.x, -bounds.y - outline});
  attachments[(int)AttachPoint::bottomLeft]  = Attachment({-0.5f * bounds.x, bounds.y + outline});
  attachments[(int)AttachPoint::bottomRight] = Attachment({0.5f * bounds.x, bounds.y + outline});
}

void World::Update()
{  
  // TODO: do resizing with a handler
}

void World::Render(sf::RenderWindow* win) const
{
  win->draw(renderRect, &Utility::worldShad);
}

sf::Vector2f World::GetBounds() const
{
  return bounds;
}

void World::Attach(AttachPoint point, std::function<void(sf::Vector2f)>& function)
{
  attachments[(int)point].Attach(function);
}