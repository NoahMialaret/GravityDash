#include "MenuOption.h"

MenuOption::MenuOption(std::string name, Event action, float yPos)
  :
  action(action)
{
  displayName.setPosition(sf::Vector2f(-CONFIG_MENU_MARGIN, yPos));
  displayName.setString(name);
  displayName.setCharacterSize(SCALED_DIM);
  displayName.setFillColor(sf::Color(173, 103, 78));
  displayName.setFont(Textures::small);
  displayName.setOrigin(sf::Vector2f(0, displayName.getGlobalBounds().height / 2));
}

void MenuOption::Update()
{
  // bezier
}

void MenuOption::Render(sf::RenderWindow* win) const
{
  if (isHighlighted)
    int i = 0; // draw background

  win->draw(displayName);
}

void MenuOption::SetY(float newY)
{
  sf::Vector2f newPos(displayName.getPosition().x, newY);
  displayName.setPosition(newPos);
}

void MenuOption::Move(float offsetY)
{
  displayName.move({0, offsetY});
}

void MenuOption::ToggleHighlight()
{
  isHighlighted = !isHighlighted;
}

ToggleOption::ToggleOption(std::string name, Event action, float yPos, bool init)
  :
  MenuOption(name, action, yPos),
  toggle(init)
{
  toggleSprite = sf::Sprite(Textures::textures.at("toggle"));
  toggleSprite.setScale(DEFAULT_SCALE);
  toggleSprite.setPosition(sf::Vector2f(CONFIG_MENU_MARGIN, yPos));

  sf::IntRect texRect(0, 0, toggleSprite.getTextureRect().width / 2, toggleSprite.getTextureRect().height);
  if (init)
    texRect.left = texRect.width;
  toggleSprite.setTextureRect(texRect);
  toggleSprite.setOrigin({(float)texRect.width, texRect.height / 2.0f});
}

void ToggleOption::Update()
{
  if (!isHighlighted || !Utility::CheckInitialPress(sf::Keyboard::Space))
    return;

  toggle = !toggle;

  sf::IntRect texRect = toggleSprite.getTextureRect();
  texRect.left = toggle ? texRect.width : 0;
  toggleSprite.setTextureRect(texRect);
}

void ToggleOption::Render(sf::RenderWindow* win) const
{
  MenuOption::Render(win);
  win->draw(toggleSprite, &Utility::worldShad);
}

void ToggleOption::SetY(float newY)
{
  MenuOption::SetY(newY);

  sf::Vector2f newPos(toggleSprite.getPosition().x, newY);
  toggleSprite.setPosition(newPos);
}

void ToggleOption::Move(float offsetY)
{
  MenuOption::Move(offsetY);
  toggleSprite.move({0, offsetY});
}
