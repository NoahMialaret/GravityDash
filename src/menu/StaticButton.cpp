#include "StaticButton.h"

StaticButton::StaticButton(sf::Vector2f pos, StaticButtonInit& config)
  :
  clickEvent(config.clickEvent)
{
  assert(config.size >= SMALL && config.size <= LARGE);

  std::string texName;
  switch (config.size)
  {
  case SMALL:
    Utility::InitText(displayName, Textures::small, config.name, pos + SMALL_TEXT_OFFSET);
    texName = "small_button";
    break;

  case MEDIUM:
    Utility::InitText(displayName, Textures::small, config.name, pos + MEDIUM_TEXT_OFFSET);
    texName = "medium_button";
    break;

  case LARGE:
    Utility::InitText(displayName, Textures::large, config.name, pos + LARGE_TEXT_OFFSET);
    texName = "large_button";
    break;
  }

  Utility::InitSprite(button, texName, pos, {2, 1});
}

void StaticButton::Render(sf::RenderWindow *win) const
{
  win->draw(button, &Utility::worldShad);
  win->draw(displayName);
}

void StaticButton::Move(sf::Vector2f offset)
{
  button.move(offset);
  displayName.move(offset);
}

void StaticButton::Click() const
{
  Event::events.push(clickEvent);
}

void StaticButton::ToggleHighlight()
{
  sf::IntRect curRect = button.getTextureRect();

  curRect.left = isHighlighted ? 0 : curRect.width;

  isHighlighted = !isHighlighted;

  button.setTextureRect(curRect);
}