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
}

void Button::Render(sf::RenderWindow *win) const
{
  win->draw(button, &Utility::worldShad);
}

void Button::Move(sf::Vector2f offset)
{
  button.move(offset);
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
{}

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
{}

void LargeButton::Update()
{
}

void LargeButton::Render(sf::RenderWindow *win) const
{
  Button::Render(win);
}
