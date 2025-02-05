#include "World.h"

World::World(sf::Vector2f size)
  :
  bounds(0.5f * SPRITE_DIM * size)
{
  float outline = 1.0f;
  // TODO: fix world rendering
  // renderRect = sf::RectangleShape((float)SPRITE_DIM * size);
  // renderRect.setOrigin(0.5f * SPRITE_DIM * size);
  // renderRect.setOutlineThickness(outline);

  attachments[(int)AttachPoint::left]  = Attachment({-bounds.x - outline, 0.0f});
  attachments[(int)AttachPoint::right] = Attachment({bounds.x + outline, 0.0f});

  attachments[(int)AttachPoint::top]    = Attachment({0.0f, -bounds.y - outline});
  attachments[(int)AttachPoint::bottom] = Attachment({0.0f, bounds.y + outline});

  attachments[(int)AttachPoint::topLeft]     = Attachment({-0.65f * bounds.x, -bounds.y - outline});
  attachments[(int)AttachPoint::topRight]    = Attachment({0.65f * bounds.x, -bounds.y - outline});
  attachments[(int)AttachPoint::bottomLeft]  = Attachment({-0.65f * bounds.x, bounds.y + outline});
  attachments[(int)AttachPoint::bottomRight] = Attachment({0.65f * bounds.x, bounds.y + outline});

  ENTITY_SHADER.setUniform("worldDim", sf::Glsl::Vec2(bounds));
  WORLD_SHADER.setUniform("worldDim", sf::Glsl::Vec2(bounds));
}

void World::Update()
{  
  // TODO: do resizing with a handler
}

void World::Render(sf::RenderWindow* win) const
{
  // Utility::RenderRectWithScale(win, renderRect, &WORLD_SHADER);
  sf::Vector2f winDim = (sf::Vector2f)Settings::GetInstance()->GetWindowDim();
  sf::RectangleShape rect(winDim);
  rect.setPosition(-0.5f * winDim);
  win->draw(rect, &WORLD_SHADER);
}

const sf::Vector2f& World::GetBounds() const
{
  return bounds;
}

void World::Attach(AttachPoint point, std::function<void(sf::Vector2f)>& function)
{
  attachments[(int)point].Attach(function);
}

void World::UpdateAttachment(World::AttachPoint point)
{
  attachments[(int)point].ForceUpdate();
}
