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

  Utility::InitSprite(button, texName, {2, 1});
  button.setPosition(pos);
}

void StaticButton::Render(sf::RenderWindow *win) const
{
  Utility::RenderSpriteWithScale(win, button, &STATIC_SHADER);
  Utility::RenderTextWithScale(win, displayName, &STATIC_SHADER);
}

void StaticButton::Move(sf::Vector2f offset)
{
  button.move(offset);
  displayName.drawable.move(offset);
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
  return button.getTextureRect().width;
}
float StaticButton::GetHeight() const
{
  return button.getTextureRect().height;
}
