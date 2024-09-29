#include "MenuOption.h"

RoundedRect MenuOption::highlightBg = RoundedRect({0,0}, {2.0f * CONFIG_MENU_MARGIN + 2.0f * Utility::gameScale, 6 * Utility::gameScale}, sf::Color(245, 204, 164));

MenuOption::MenuOption(std::string name, Event action, float yPos)
  :
  action(action)
{
  Utility::InitText(displayName, Textures::small, name, {-CONFIG_MENU_MARGIN, yPos}, {0, 0.5f});
}

void MenuOption::Update()
{
  // bezier
}

void MenuOption::Render(sf::RenderWindow* win) const
{
  if (isHighlighted)
    highlightBg.Render(win);

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

  // if (isHighlighted)
  //   highlightBg.SetCentre({0, })
}

bool MenuOption::IsActive()
{
  return isActive;
}

ToggleOption::ToggleOption(std::string name, Event action, float yPos, OptionConfig::Toggle config)
  :
  MenuOption(name, action, yPos),
  toggle(config.init)
{
  Utility::InitSprite(toggleSprite, "toggle", {CONFIG_MENU_MARGIN, yPos}, {2, 1}, {1.0f, 0.5f});

  if (config.init)
  {
    sf::IntRect texRect = toggleSprite.getTextureRect();
    texRect.left = texRect.width;
    toggleSprite.setTextureRect(texRect);
  }
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
  Utility::InitText(displayRange, Textures::small, "{" + std::to_string(value) + "}", {CONFIG_MENU_MARGIN, yPos}, {1.0f, 0.5f});
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
  Utility::InitText(displaySelection, Textures::small, "{" + (*config.selections)[config.initIndex] + "}", {CONFIG_MENU_MARGIN, yPos}, {1.0f, 0.5f});
}
void SelectionOption::Update()
{
  if (!isHighlighted)
    return;

  int delta = Utility::CheckInitialPress(sf::Keyboard::D) - Utility::CheckInitialPress(sf::Keyboard::A);
  
  if (!delta)
    return;
  
  index += delta;
  if (index < 0)
    index = selections.size() - 1;
  else if (index >= selections.size())
    index = 0;

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

ControlOption::ControlOption(std::string name, Event action, float yPos, OptionConfig::Control config)
  :
  MenuOption(name, action, yPos)
{  
  Utility::InitText(curKey, Textures::small, Utility::GetStringFromKeyCode(config.init), {CONFIG_MENU_MARGIN - 2 * Utility::gameScale, yPos}, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = curKey.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(curKey.getPosition() + sf::Vector2f(-width / 2.0f, -Utility::gameScale), sf::Vector2f(width + 4.0f * Utility::gameScale, 6.0f * Utility::gameScale), {173, 103, 78});
}
 
void ControlOption::Update()
{
  if (!Utility::initialKeyPresses.empty())
  {
    Utility::UpdateText(curKey, Utility::GetStringFromKeyCode((sf::Keyboard::Key)Utility::initialKeyPresses[0]), {1.0f, 0.5f});
    float width = curKey.getGlobalBounds().getSize().x;
    keyBg.SetCentre(curKey.getPosition() + sf::Vector2f(-width / 2.0f, -Utility::gameScale));
    keyBg.SetDim(sf::Vector2f(width + 4.0f * Utility::gameScale, 6.0f * Utility::gameScale));
  }
}

void ControlOption::Render(sf::RenderWindow* win) const
{
  MenuOption::Render(win);
  keyBg.Render(win);
  win->draw(curKey);
}

void ControlOption::SetY(float newY)
{
  MenuOption::SetY(newY);
  sf::Vector2f newPos(curKey.getPosition().x, newY);
  keyBg.Move({0.0f, newY - curKey.getPosition().y});
  curKey.setPosition(newPos);
}

void ControlOption::Move(float offsetY)
{
  MenuOption::Move(offsetY);
  curKey.move({0, offsetY});
  keyBg.Move({0, offsetY});
}
