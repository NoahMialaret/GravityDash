#include "Button.h"

Button::Button(std::string name, Event action, std::string textureName, sf::Vector2f pos)
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

void Button::Render(sf::RenderWindow *win) const
{
  win->draw(button, &Utility::worldShad);
  win->draw(displayName);
}

void Button::Move(sf::Vector2f offset)
{
  button.move(offset);
  displayName.move(offset);
}

void Button::Click() const
{
  Utility::events.push_back(action);
}

void Button::ToggleHighlight()
{
  sf::IntRect curRect = button.getTextureRect();
  if (!isHighlighted)
    curRect.left = curRect.width;
  else
    curRect.left = 0;

  isHighlighted = !isHighlighted;
  button.setTextureRect(curRect);
}

MediumButton::MediumButton(sf::Vector2f pos, std::string name, Event action)
  :
  Button(name, action, "medium_button", pos)
{
  displayName.setFont(Textures::small);
  displayName.setOrigin(sf::Vector2f(displayName.getGlobalBounds().width / 2, displayName.getGlobalBounds().height / 2));
  displayName.move(MEDIUM_TEXT_OFFSET);
}

void MediumButton::Update()
{
}

void MediumButton::Render(sf::RenderWindow *win) const
{
  Button::Render(win);
}

LargeButton::LargeButton(sf::Vector2f pos, std::string name, Event action)
  :
  Button(name, action, "large_button", pos)
{
  displayName.setFont(Textures::large);
  displayName.setOrigin(sf::Vector2f(displayName.getGlobalBounds().width / 2, displayName.getGlobalBounds().height / 2));
  displayName.move(LARGE_TEXT_OFFSET);
}

void LargeButton::Update()
{
}

void LargeButton::Render(sf::RenderWindow *win) const
{
  Button::Render(win);
}
