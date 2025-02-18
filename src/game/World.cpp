#include "World.h"

World::World(sf::Vector2f size)
  :
  bounds(0.5f * SPRITE_DIM * size)
{
  float outline = 1.0f;
  worldRect.setSize((sf::Vector2f)ASPECT_RATIO);
  worldRect.setPosition(-0.5f * (sf::Vector2f)ASPECT_RATIO);

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

void World::Render(sf::RenderWindow* win) const
{
  Utility::RenderRectWithScale(win, worldRect, &WORLD_SHADER);
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
