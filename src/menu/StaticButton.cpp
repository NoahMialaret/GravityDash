#include "StaticButton.h"

StaticButton::StaticButton(StaticButtonInit &config, sf::Vector2f pos)
    : clickEvent(config.clickEvent)
{
  assert(config.size >= SMALL && config.size <= LARGE);

  std::string texName;
  switch (config.size)
  {
  case SMALL:
    Utility::InitText(displayName, SMALL_FONT, config.name, pos + SMALL_TEXT_OFFSET);
    texName = "small_button";
    break;

  case MEDIUM:
    Utility::InitText(displayName, SMALL_FONT, config.name, pos + MEDIUM_TEXT_OFFSET);
    texName = "medium_button";
    break;

  case LARGE:
    Utility::InitText(displayName, LARGE_FONT, config.name, pos + LARGE_TEXT_OFFSET);
    texName = "large_button";
    break;
  }

  Utility::InitSprite(button, texName, pos, {2, 1});
}

void StaticButton::Render(sf::RenderWindow *win) const
{
  win->draw(button, &WORLD_SHADER);
  win->draw(displayName);
}

void StaticButton::Move(sf::Vector2f offset)
{
  button.move(offset);
  displayName.move(offset);
}

void StaticButton::Click() const
{
  PUSH_EVENT(clickEvent);
}

void StaticButton::ToggleHighlight()
{
  sf::IntRect curRect = button.getTextureRect();

  curRect.left = isHighlighted ? 0 : curRect.width;

  isHighlighted = !isHighlighted;

  button.setTextureRect(curRect);
}

float StaticButton::GetWidth() const
{
  return button.getTextureRect().width * ProgramSettings::gameScale;
}
float StaticButton::GetHeight() const
{
  return button.getTextureRect().height * ProgramSettings::gameScale;
}
