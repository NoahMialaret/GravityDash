#include "MenuButton.h"

MenuButton::MenuButton(Event action, std::string textureName, sf::Vector2f pos)
  :
  action(action)
{
  Utility::InitSprite(button, textureName, pos, {2, 1});
}

void MenuButton::Render(sf::RenderWindow *win) const
{
  win->draw(button, &Utility::worldShad);
  win->draw(displayName);
}

void MenuButton::Move(sf::Vector2f offset)
{
  button.move(offset);
  displayName.move(offset);
}

void MenuButton::Click() const
{
  Event::events.push_back(action);
}

void MenuButton::ToggleHighlight()
{
  sf::IntRect curRect = button.getTextureRect();
  if (!isHighlighted)
    curRect.left = curRect.width;
  else
    curRect.left = 0;

  isHighlighted = !isHighlighted;
  button.setTextureRect(curRect);
}

SmallButton::SmallButton(sf::Vector2f pos, std::string name, Event action)
  :
  MenuButton(action, "small_button", pos)
{
  Utility::InitText(displayName, Textures::small, name, pos + SMALL_TEXT_OFFSET);
}

void SmallButton::Update()
{
}

void SmallButton::Render(sf::RenderWindow* win) const
{
  MenuButton::Render(win);
}

MediumButton::MediumButton(sf::Vector2f pos, std::string name, Event action)
  :
  MenuButton(action, "medium_button", pos)
{
  Utility::InitText(displayName, Textures::small, name, pos + MEDIUM_TEXT_OFFSET);
}

void MediumButton::Update()
{
}

void MediumButton::Render(sf::RenderWindow* win) const
{
  MenuButton::Render(win);
}

LargeButton::LargeButton(sf::Vector2f pos, std::string name, Event action)
  :
  MenuButton(action, "large_button", pos)
{
  Utility::InitText(displayName, Textures::large, name, pos + LARGE_TEXT_OFFSET);
}

void LargeButton::Update()
{
}

void LargeButton::Render(sf::RenderWindow* win) const
{
  MenuButton::Render(win);
}