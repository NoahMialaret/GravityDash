#include "ListItem.h"

Interactable::Interactable(int value)
  :
  value(value),
  backup(value)
{}

int Interactable::GetValue() const
{
  return value;
}



Static::Static(int value)
  :
  Interactable(value)
{
  Utility::InitText(staticText, SMALL_FONT, std::to_string(value), ZERO_VECTOR, {1.0f, 0.5f});
}

bool Static::Update()
{
  return true;
}

void Static::Render(sf::RenderWindow* win) const
{
  win->draw(staticText);
}

void Static::SetPosition(sf::Vector2f& pos)
{
  staticText.setPosition(pos);
}



Toggle::Toggle(int isToggled)
  :
  Interactable(isToggled)
{
  Utility::InitSprite(toggleSprite, "toggle", {2, 1}, {1.0f, 0.5f});

  sf::IntRect rect = toggleSprite.getTextureRect();
  rect.left = isToggled * rect.width;
  toggleSprite.setTextureRect(rect);
}

bool Toggle::Update()
{
  value = !value;

  sf::IntRect rect = toggleSprite.getTextureRect();
  rect.left = value * rect.width;
  toggleSprite.setTextureRect(rect);

  return true;
}

void Toggle::Render(sf::RenderWindow* win) const
{
  win->draw(toggleSprite, &WORLD_SHADER);
}

void Toggle::SetPosition(sf::Vector2f& pos)
{
  toggleSprite.setPosition(pos);
}



Range::Range(int value, int min, int max)
  :
  Interactable(std::clamp(value, min, max)),
  min(min),
  max(max)
{
  Utility::InitText(rangeText, SMALL_FONT, "{" + std::to_string(std::clamp(value, min, max)) + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool Range::Update()
{
  if (ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = value;
    return true;
  }
  if (ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::escape))
  {
    value = backup;
    Utility::UpdateText(rangeText, "{" + std::to_string(value) + "}", {1.0f, 0.5f});
    return true;
  }

  int delta = ProgramSettings::GetControls()->IsActionClicked(Controls::Action::right)
     - ProgramSettings::GetControls()->IsActionClicked(Controls::Action::left);
  
  if (!delta || value + delta > max || value + delta < min)
    return false;
  
  value += delta;
  
  Utility::UpdateText(rangeText, "{" + std::to_string(value) + "}", {1.0f, 0.5f});

  return false;
}

void Range::Render(sf::RenderWindow* win) const
{
  win->draw(rangeText);
}

void Range::SetPosition(sf::Vector2f& pos)
{
  rangeText.setPosition(pos);
}



Selection::Selection(int index, std::vector<std::string>& selections)
  :
  Interactable(std::clamp(index, 0, (int)selections.size() - 1)),
  selections(selections)
{
  Utility::InitText(selectionText, SMALL_FONT, "{" + selections[std::clamp(index, 0, (int)selections.size())] + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool Selection::Update()
{
  if (ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = value;
    return true;
  }
  if (ProgramSettings::GetControls()->IsActionOnInitialClick(Controls::Action::escape))
  {
    value = backup;
    Utility::UpdateText(selectionText, "{" + selections[value] + "}", {1.0f, 0.5f});
    return true;
  }

  int delta = ProgramSettings::GetControls()->IsActionClicked(Controls::Action::right)
     - ProgramSettings::GetControls()->IsActionClicked(Controls::Action::left);
  
  if (!delta)
    return false;

  value += delta;
  if (value < 0)
    value = selections.size() - 1;
  else if (value >= (int)selections.size())
    value = 0;
  
  Utility::UpdateText(selectionText, "{" + selections[value] + "}", {1.0f, 0.5f});

  return false;
}

void Selection::Render(sf::RenderWindow* win) const
{
  win->draw(selectionText);
}

void Selection::SetPosition(sf::Vector2f& pos)
{
  selectionText.setPosition(pos);
}



Control::Control(int keyCode)
  :
  Interactable(keyCode)
{
  Utility::InitText(controlText, SMALL_FONT, Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)keyCode), 
                    ZERO_VECTOR, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = controlText.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(controlText.getPosition() + sf::Vector2f(-width / 2.0f, -ProgramSettings::gameScale), sf::Vector2f(width + 4.0f * ProgramSettings::gameScale, 6.0f * ProgramSettings::gameScale), {173, 103, 78});
  // keyBg.SetHorizontal(controlText.getPosition().x - width / 2.0f);
}

bool Control::Update()
{
  int keyCode = Keyboard::GetInstance()->GetKeyCodeAtHead();
  if (keyCode == -1)
    return false;

  std::string codeString = Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)keyCode);
  if (codeString == "NULL")
    return false;

  Utility::UpdateText(controlText, codeString, {1.0f, 0.5f});
  float width = controlText.getGlobalBounds().getSize().x;
  keyBg.SetHorizontal(controlText.getPosition().x - width / 2.0f);
  keyBg.SetDim(sf::Vector2f(width + 4.0f * ProgramSettings::gameScale, 6.0f * ProgramSettings::gameScale));
  
  return true;
}

void Control::Render(sf::RenderWindow* win) const
{
  keyBg.Render(win);
  win->draw(controlText);
}

void Control::SetPosition(sf::Vector2f& pos)
{
  pos.x -= 2 * ProgramSettings::gameScale;
  float width = controlText.getGlobalBounds().getSize().x;
  keyBg.SetHorizontal(controlText.getPosition().x - width / 2.0f);
  keyBg.SetVertical(pos.y);
  controlText.setPosition(pos);
}



ListItem::ListItem(std::string name, float vertOffset, Interactable* interactable)
  :
  event({Event::Type::updateSettings, Event::Data{.updateSettings = {name.c_str(), -1}}}),
  vertOffset(vertOffset)
{
  Utility::InitText(displayName, SMALL_FONT, name, ZERO_VECTOR, {0, 0.5f});

  this->interactable = std::unique_ptr<Interactable>(interactable);
}

bool ListItem::Update()
{
  if (isActive)
  {
    if (interactable.get()->Update())
    {
      event.data.updateSettings.value = interactable.get()->GetValue();
      PUSH_EVENT(event);
      isActive = false;
    }
    return true;
  }

  if (ProgramSettings::p1Controls.IsActionOnInitialClick(Controls::Action::select))
  {
    isActive = true;
    return true;
  }

  return false;
}

void ListItem::Render(sf::RenderWindow *win) const
{
  win->draw(displayName);
  interactable.get()->Render(win);
}

bool ListItem::IsActive() const
{
  return isActive;
}

void ListItem::SetPosition(sf::Vector2f pos)
{
  pos.y += vertOffset;
  pos.x -= LIST_MARGIN;
  displayName.setPosition(pos);
  pos.x += 2 * LIST_MARGIN;
  interactable.get()->SetPosition(pos);
}

float ListItem::GetVerticalOffset() const
{
  return vertOffset;
}
