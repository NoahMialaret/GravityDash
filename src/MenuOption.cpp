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

ToggleOption::ToggleOption(std::string name, Event action, float yPos, OptionConfig::Toggle config)
  :
  MenuOption(name, action, yPos),
  toggle(config.init)
{
  toggleSprite = sf::Sprite(Textures::textures.at("toggle"));
  toggleSprite.setScale(DEFAULT_SCALE);
  toggleSprite.setPosition(sf::Vector2f(CONFIG_MENU_MARGIN, yPos));

  sf::IntRect texRect(0, 0, toggleSprite.getTextureRect().width / 2, toggleSprite.getTextureRect().height);
  if (config.init)
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

RangeOption::RangeOption(std::string name, Event action, float yPos, OptionConfig::Range config)
  :
  MenuOption(name, action, yPos),
  value(config.init),
  min(config.min),
  max(config.max)
{
  displayRange.setPosition(sf::Vector2f(CONFIG_MENU_MARGIN, yPos));
  displayRange.setString("{" + std::to_string(value) + "}");
  displayRange.setCharacterSize(SCALED_DIM);
  displayRange.setFillColor(sf::Color(173, 103, 78));
  displayRange.setFont(Textures::small);
  displayRange.setOrigin(sf::Vector2f(displayRange.getGlobalBounds().width, displayName.getGlobalBounds().height / 2));
}

void RangeOption::Update()
{
  if (!isHighlighted)
    return;

  int delta = Utility::CheckInitialPress(sf::Keyboard::D) - Utility::CheckInitialPress(sf::Keyboard::A);
  
  if (!delta || value + delta > max || value + delta < min)
    return;
  
  value += delta;
  displayRange.setString("{" + std::to_string(value) + "}");
  displayRange.setOrigin(sf::Vector2f(displayRange.getGlobalBounds().width, displayName.getGlobalBounds().height / 2));

  // event.data = value
}

void RangeOption::Render(sf::RenderWindow *win) const
{
  MenuOption::Render(win);
  win->draw(displayRange);
}

void RangeOption::SetY(float newY)
{
  MenuOption::SetY(newY);
  sf::Vector2f newPos(displayRange.getPosition().x, newY);
  displayRange.setPosition(newPos);
}

void RangeOption::Move(float offsetY)
{
  MenuOption::Move(offsetY);
  displayRange.move({0, offsetY});
}

SelectionOption::SelectionOption(std::string name, Event action, float yPos, OptionConfig::Selection config)
  :
  MenuOption(name, action, yPos),
  index(config.initIndex),
  selections(*config.selections)
{
  displaySelection.setPosition(sf::Vector2f(CONFIG_MENU_MARGIN, yPos));
  displaySelection.setString("{" + (*config.selections)[config.initIndex] + "}");
  displaySelection.setCharacterSize(SCALED_DIM);
  displaySelection.setFillColor(sf::Color(173, 103, 78));
  displaySelection.setFont(Textures::small);
  displaySelection.setOrigin(sf::Vector2f(displaySelection.getGlobalBounds().width, displaySelection.getGlobalBounds().height / 2));
}
void SelectionOption::Update()
{
  if (!isHighlighted)
    return;

  int delta = Utility::CheckInitialPress(sf::Keyboard::D) - Utility::CheckInitialPress(sf::Keyboard::A);
  
  if (!delta || index + delta < 0 || index + delta >= selections.size())
    return;
  
  index += delta;
  displaySelection.setString("{" + selections[index] + "}");
  displaySelection.setOrigin(sf::Vector2f(displaySelection.getGlobalBounds().width, displayName.getGlobalBounds().height / 2));

  // event.data = value
}

void SelectionOption::Render(sf::RenderWindow *win) const
{
  MenuOption::Render(win);
  win->draw(displaySelection);
}

void SelectionOption::SetY(float newY)
{
  MenuOption::SetY(newY);
  sf::Vector2f newPos(displaySelection.getPosition().x, newY);
  displaySelection.setPosition(newPos);
}

void SelectionOption::Move(float offsetY)
{
  MenuOption::Move(offsetY);
  displaySelection.move({0, offsetY});
}