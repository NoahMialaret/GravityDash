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



StaticInteractable::StaticInteractable(int value)
  :
  Interactable(value)
{
  Utility::InitText(staticText, SMALL_FONT, std::to_string(value), ZERO_VECTOR, {1.0f, 0.5f});
}

bool StaticInteractable::Update()
{
  return true;
}

void StaticInteractable::Render(sf::RenderWindow* win) const
{
  win->draw(staticText);
}

void StaticInteractable::SetPosition(sf::Vector2f& pos)
{
  staticText.setPosition(pos);
}



ToggleInteractable::ToggleInteractable(bool isToggled)
  :
  Interactable(isToggled)
{
  Utility::InitSprite(toggleSprite, "toggle", {2, 1}, {1.0f, 0.5f});

  sf::IntRect rect = toggleSprite.getTextureRect();
  rect.left = isToggled * rect.width;
  toggleSprite.setTextureRect(rect);
}

bool ToggleInteractable::Update()
{
  value = !value;

  sf::IntRect rect = toggleSprite.getTextureRect();
  rect.left = value * rect.width;
  toggleSprite.setTextureRect(rect);

  return true;
}

void ToggleInteractable::Render(sf::RenderWindow* win) const
{
  win->draw(toggleSprite, &WORLD_SHADER);
}

void ToggleInteractable::SetPosition(sf::Vector2f& pos)
{
  toggleSprite.setPosition(pos);
}



RangeInteractable::RangeInteractable(int value, int min, int max)
  :
  Interactable(std::clamp(value, min, max)),
  min(min),
  max(max)
{
  Utility::InitText(rangeText, SMALL_FONT, "{" + std::to_string(std::clamp(value, min, max)) + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool RangeInteractable::Update()
{
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = value;
    return true;
  }
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
  {
    value = backup;
    Utility::UpdateText(rangeText, "{" + std::to_string(value) + "}", {1.0f, 0.5f});
    return true;
  }

  int delta = Settings::GetInstance()->IsActionClicked(Controls::Action::right)
            - Settings::GetInstance()->IsActionClicked(Controls::Action::left);
  
  if (!delta || value + delta > max || value + delta < min)
    return false;
  
  value += delta;
  
  Utility::UpdateText(rangeText, "{" + std::to_string(value) + "}", {1.0f, 0.5f});

  return false;
}

void RangeInteractable::Render(sf::RenderWindow* win) const
{
  win->draw(rangeText);
}

void RangeInteractable::SetPosition(sf::Vector2f& pos)
{
  rangeText.setPosition(pos);
}



SelectionInteractable::SelectionInteractable(int index, std::vector<std::string>& selections)
  :
  Interactable(std::clamp(index, 0, (int)selections.size() - 1)),
  selections(selections)
{
  Utility::InitText(selectionText, SMALL_FONT, "{" + selections[std::clamp(index, 0, (int)selections.size())] + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool SelectionInteractable::Update()
{
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = value;
    return true;
  }
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
  {
    value = backup;
    Utility::UpdateText(selectionText, "{" + selections[value] + "}", {1.0f, 0.5f});
    return true;
  }

  int delta = Settings::GetInstance()->IsActionClicked(Controls::Action::right)
     - Settings::GetInstance()->IsActionClicked(Controls::Action::left);
  
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

void SelectionInteractable::Render(sf::RenderWindow* win) const
{
  win->draw(selectionText);
}

void SelectionInteractable::SetPosition(sf::Vector2f& pos)
{
  selectionText.setPosition(pos);
}



KeybindInteractable::KeybindInteractable(sf::Keyboard::Key keyCode)
  :
  Interactable((int)keyCode)
{
  Utility::InitText(controlText, SMALL_FONT, Keyboard::GetInstance()->GetStringFromKeyCode(keyCode), 
                    ZERO_VECTOR, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = controlText.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(controlText.getPosition() + sf::Vector2f(-width / 2.0f, -FSCALE), 
                      sf::Vector2f(width + 4.0f * FSCALE, 6.0f * FSCALE), 
                      {173, 103, 78});
}

bool KeybindInteractable::Update()
{
  sf::Keyboard::Key keyCode = Keyboard::GetInstance()->GetKeyAtHead();
  if (keyCode == sf::Keyboard::Unknown)
    return false;

  std::string codeString = Keyboard::GetInstance()->GetStringFromKeyCode(keyCode);
  if (codeString == "NULL")
    return false;

  Utility::UpdateText(controlText, codeString, {1.0f, 0.5f});
  float width = controlText.getGlobalBounds().getSize().x;
  keyBg.SetHorizontal(controlText.getPosition().x - width / 2.0f);
  keyBg.SetDim(sf::Vector2f(width + 4.0f * FSCALE, 6.0f * FSCALE));
  
  return true;
}

void KeybindInteractable::Render(sf::RenderWindow* win) const
{
  keyBg.Render(win);
  win->draw(controlText);
}

void KeybindInteractable::SetPosition(sf::Vector2f& pos)
{
  pos.x -= 2 * FSCALE;
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
  Utility::InitText(displayName, SMALL_FONT, name, ZERO_VECTOR, {0.0f, 0.5f});

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

  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
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

void ListItem::SetPosition(sf::Vector2f origin)
{
  origin.y += vertOffset;
  origin.x -= LIST_MARGIN; // Left margin
  displayName.setPosition(origin);

  origin.x += 2 * LIST_MARGIN; // Right margin
  interactable.get()->SetPosition(origin);
}

float ListItem::GetVerticalOffset() const
{
  return vertOffset;
}
