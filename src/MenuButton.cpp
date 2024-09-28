#include "MenuButton.h"

MenuButton::MenuButton(std::string name, Event action, std::string textureName, sf::Vector2f pos)
  :
  action(action)
{
  button = sf::Sprite(Textures::textures.at(textureName));
  button.setScale(DEFAULT_SCALE);
  button.setPosition(pos);
  button.setTextureRect(sf::IntRect(0, 0, button.getTextureRect().width / 2, button.getTextureRect().height));
  button.setOrigin({button.getTextureRect().width / 2.0f, button.getTextureRect().height / 2.0f});


  displayName.setPosition(pos);
  displayName.setString(name);
  displayName.setCharacterSize(SCALED_DIM);
  displayName.setFillColor(sf::Color(173, 103, 78));
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
  MenuButton(name, action, "small_button", pos)
{
  displayName.setFont(Textures::small);
  displayName.setOrigin(sf::Vector2f(displayName.getGlobalBounds().width / 2, displayName.getGlobalBounds().height / 2));
  displayName.move(SMALL_TEXT_OFFSET);
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
  MenuButton(name, action, "medium_button", pos)
{
  displayName.setFont(Textures::small);
  displayName.setOrigin(sf::Vector2f(displayName.getGlobalBounds().width / 2, displayName.getGlobalBounds().height / 2));
  displayName.move(MEDIUM_TEXT_OFFSET);
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
  MenuButton(name, action, "large_button", pos)
{
  displayName.setFont(Textures::large);
  displayName.setOrigin(sf::Vector2f(displayName.getGlobalBounds().width / 2, displayName.getGlobalBounds().height / 2));
  displayName.move(LARGE_TEXT_OFFSET);
}

void LargeButton::Update()
{
}

void LargeButton::Render(sf::RenderWindow* win) const
{
  MenuButton::Render(win);
}