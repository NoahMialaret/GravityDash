#include "MenuOption.h"

RoundedRect MenuOption::highlightBg = RoundedRect({0,0}, {2.0f * CONFIG_MENU_MARGIN + 2.0f * ProgramSettings::gameScale, 6 * ProgramSettings::gameScale}, sf::Color(245, 204, 164));
MenuOption* MenuOption::curHighlight = nullptr;

MenuOption::MenuOption(std::string name, Event action, float* origin, float offset)
  :
  action(action),
  origin(origin),
  vertOffset(offset)
{
  Utility::InitText(displayName, SMALL_FONT, name, {-CONFIG_MENU_MARGIN, *origin + offset}, {0, 0.5f});
}

void MenuOption::Update()
{
  displayName.setPosition(displayName.getPosition().x, *origin + vertOffset);

  if (isHighlighted)
    highlightBg.SetVertical(*origin + vertOffset);
  // bezier
}

void MenuOption::Render(sf::RenderWindow* win) const
{
  if (isHighlighted)
    highlightBg.Render(win);

  win->draw(displayName);
}

void MenuOption::SetHighlight()
{
  if (curHighlight != nullptr)
    curHighlight->isHighlighted = false;

  curHighlight = this;
  isHighlighted = true;
}

bool MenuOption::IsActive()
{
  return isActive;
}

float MenuOption::GetOffset() const
{
  return vertOffset;
}

StaticText::StaticText(std::string name, Event action, float *origin, float offset, OptionConfig::StaticText config)
  :
  MenuOption(name, action, origin, offset)
{
  Utility::InitText(staticText, SMALL_FONT, *config.text, {CONFIG_MENU_MARGIN, *origin + offset}, {1.0f, 0.5f});
}

void StaticText::Update()
{
  staticText.setPosition(staticText.getPosition().x, *origin + vertOffset);
  MenuOption::Update();
}

void StaticText::Render(sf::RenderWindow *win) const
{
  MenuOption::Render(win);
  win->draw(staticText);
}

ToggleOption::ToggleOption(std::string name, Event action, float* origin, float offset, OptionConfig::Toggle config)
  :
  MenuOption(name, action, origin, offset),
  toggle(config.init)
{
  Utility::InitSprite(toggleSprite, "toggle", {2, 1}, {1.0f, 0.5f});
  toggleSprite.setPosition({CONFIG_MENU_MARGIN, *origin + offset});

  if (config.init)
  {
    sf::IntRect texRect = toggleSprite.getTextureRect();
    texRect.left = texRect.width;
    toggleSprite.setTextureRect(texRect);
  }
}

void ToggleOption::Update()
{
  MenuOption::Update();
  toggleSprite.setPosition(toggleSprite.getPosition().x, *origin + vertOffset);
  if (!isHighlighted || !ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::select))
    return;

  toggle = !toggle;

  sf::IntRect texRect = toggleSprite.getTextureRect();
  texRect.left = toggle ? texRect.width : 0;
  toggleSprite.setTextureRect(texRect);
}

void ToggleOption::Render(sf::RenderWindow* win) const
{
  MenuOption::Render(win);
  win->draw(toggleSprite, &WORLD_SHADER);
}

RangeOption::RangeOption(std::string name, Event action, float* origin, float offset, OptionConfig::Range config)
  :
  MenuOption(name, action, origin, offset),
  value(config.init),
  min(config.min),
  max(config.max)
{
  Utility::InitText(displayRange, SMALL_FONT, "{" + std::to_string(value) + "}", {CONFIG_MENU_MARGIN, *origin + offset}, {1.0f, 0.5f});
}

void RangeOption::Update()
{
  MenuOption::Update();
  displayRange.setPosition(displayRange.getPosition().x, *origin + vertOffset);

  if (!isHighlighted)
    return;

  int delta = ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::right)
     - ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::left);
  
  if (!delta || value + delta > max || value + delta < min)
    return;
  
  value += delta;
  
  Utility::UpdateText(displayRange, "{" + std::to_string(value) + "}", {1.0f, 0.5f});

  // event.data = value
}

void RangeOption::Render(sf::RenderWindow *win) const
{
  MenuOption::Render(win);
  win->draw(displayRange);
}

SelectionOption::SelectionOption(std::string name, Event action, float* origin, float offset, OptionConfig::Selection config)
  :
  MenuOption(name, action, origin, offset),
  index(config.initIndex),
  selections(*config.selections)
{
  Utility::InitText(displaySelection, SMALL_FONT, "{" + (*config.selections)[config.initIndex] + "}", {CONFIG_MENU_MARGIN, *origin + offset}, {1.0f, 0.5f});
}
void SelectionOption::Update()
{
  MenuOption::Update();
  displaySelection.setPosition(displaySelection.getPosition().x, *origin + vertOffset);

  if (!isHighlighted)
    return;

  int delta = ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::right)
     - ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::left);
  
  if (!delta)
    return;
  
  index += delta;
  if (index < 0)
    index = selections.size() - 1;
  else if (index >= selections.size())
    index = 0;

  Utility::UpdateText(displaySelection, "{" + selections[index] + "}", {1.0f, 0.5f});

  // event.data = value
}

void SelectionOption::Render(sf::RenderWindow *win) const
{
  MenuOption::Render(win);
  win->draw(displaySelection);
}

ControlOption::ControlOption(std::string name, Event action, float* origin, float offset, OptionConfig::Control config)
  :
  MenuOption(name, action, origin, offset)
{  
  Utility::InitText(curKey, SMALL_FONT, Keyboard::GetInstance()->GetStringFromKeyCode(config.init), {CONFIG_MENU_MARGIN - 2 * ProgramSettings::gameScale, *origin + offset}, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = curKey.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(curKey.getPosition() + sf::Vector2f(-width / 2.0f, -ProgramSettings::gameScale), sf::Vector2f(width + 4.0f * ProgramSettings::gameScale, 6.0f * ProgramSettings::gameScale), {173, 103, 78});
}
 
void ControlOption::Update()
{
  MenuOption::Update();
  curKey.setPosition(curKey.getPosition().x, *origin + vertOffset);
  keyBg.SetVertical(*origin + vertOffset);
  // if (!Keyboard::keysPressed.empty())
  // {
  //   Utility::UpdateText(curKey, Keyboard::GetStringFromKeyCode(Keyboard::keysPressed.front()), {1.0f, 0.5f});
  //   float width = curKey.getGlobalBounds().getSize().x;
  //   keyBg.SetHorizontal(curKey.getPosition().x - width / 2.0f);
  //   keyBg.SetDim(sf::Vector2f(width + 4.0f * ProgramSettings::gameScale, 6.0f * ProgramSettings::gameScale));
  // }
}

void ControlOption::Render(sf::RenderWindow* win) const
{
  MenuOption::Render(win);
  keyBg.Render(win);
  win->draw(curKey);
}