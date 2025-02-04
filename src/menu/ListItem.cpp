#include "ListItem.h"

Interactable::Interactable(int value, Event::Type eventType)
  :
  event(eventType, {.value = value}),
  value(&event.data.value),
  backup(*this->value)
{}

Interactable::Interactable(Settings::Setting setting)
  :
  event(Event::Type::updateSettings, {.updateSettings = {(int)setting, GET_SETTING(setting)}}),
  value(&event.data.updateSettings.value),
  backup(*value)
{}

int Interactable::GetValue() const
{
  return *value;
}



StaticInteractable::StaticInteractable(int value, Event::Type eventType)
  :
  Interactable(value, eventType)
{
  event.data.value = value;
  Utility::InitText(staticText, SMALL_FONT, std::to_string(value), ZERO_VECTOR, {1.0f, 0.5f});
}

bool StaticInteractable::Update()
{
  PUSH_EVENT(event);
  return true;
}

void StaticInteractable::Render(sf::RenderWindow* win) const
{
  Utility::RenderTextWithScale(win, staticText, nullptr);
}

void StaticInteractable::SetPosition(sf::Vector2f& pos)
{
  staticText.drawable.setPosition(pos);
}



ToggleInteractable::ToggleInteractable(bool isToggled, Event::Type eventType)
  :
  Interactable(isToggled, eventType)
{
  Utility::InitSprite(toggleSprite, "toggle", {2, 1}, {1.0f, 0.5f});

  UpdateSprite();
}

ToggleInteractable::ToggleInteractable(Settings::Setting setting)
  :
  Interactable(setting)
{
  Utility::InitSprite(toggleSprite, "toggle", {2, 1}, {1.0f, 0.5f});

  UpdateSprite();
}

bool ToggleInteractable::Update()
{
  *value = !(*value);

  UpdateSprite();

  PUSH_EVENT(event);

  return true;
}

void ToggleInteractable::Render(sf::RenderWindow* win) const
{
  Utility::RenderSpriteWithScale(win, toggleSprite, &WORLD_SHADER);
}

void ToggleInteractable::SetPosition(sf::Vector2f& pos)
{
  toggleSprite.setPosition(pos);
}

void ToggleInteractable::UpdateSprite()
{
  sf::IntRect rect = toggleSprite.getTextureRect();
  rect.left = *value * rect.width;
  toggleSprite.setTextureRect(rect);
}



RangeInteractable::RangeInteractable(int value, int min, int max, Event::Type eventType)
  :
  Interactable(std::clamp(value, min, max), eventType),
  min(min),
  max(max)
{
  Utility::InitText(rangeText, SMALL_FONT, "{" + std::to_string(std::clamp(value, min, max)) + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

RangeInteractable::RangeInteractable(Settings::Setting setting, int min, int max)
  :
  Interactable(setting),
  min(min),
  max(max)
{
  Utility::InitText(rangeText, SMALL_FONT, "{" + std::to_string(*value) + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool RangeInteractable::Update()
{
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = *value;
    PUSH_EVENT(event);
    return true;
  }
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
  {
    *value = backup;
    rangeText.drawable.setString("{" + std::to_string(*value) + "}");
    return true;
  }

  int delta = Settings::GetInstance()->IsActionClicked(Controls::Action::right)
            - Settings::GetInstance()->IsActionClicked(Controls::Action::left);
  
  if (!delta || *value + delta > max || *value + delta < min)
    return false;
  
  *value += delta;
  
  rangeText.drawable.setString("{" + std::to_string(*value) + "}");

  PUSH_EVENT(event);

  return false;
}

void RangeInteractable::Render(sf::RenderWindow* win) const
{
  Utility::RenderTextWithScale(win, rangeText, nullptr);
}

void RangeInteractable::SetPosition(sf::Vector2f& pos)
{
  rangeText.drawable.setPosition(pos);
}



SelectionInteractable::SelectionInteractable(int index, std::vector<std::string>& selections, Event::Type eventType)
  :
  Interactable(std::clamp(index, 0, (int)selections.size() - 1), eventType),
  selections(selections)
{
  Utility::InitText(selectionText, SMALL_FONT, "{" + selections[std::clamp(index, 0, (int)selections.size())] + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

SelectionInteractable::SelectionInteractable(Settings::Setting setting, std::vector<std::string> &selections)
  :
  Interactable(setting),
  selections(selections)
{
  Utility::InitText(selectionText, SMALL_FONT, "{" + selections[*value] + "}", ZERO_VECTOR, {1.0f, 0.5f});
}

bool SelectionInteractable::Update()
{
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    backup = *value;
    PUSH_EVENT(event);
    return true;
  }
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
  {
    *value = backup;
    selectionText.drawable.setString("{" + selections[*value] + "}");
    return true;
  }

  int delta = Settings::GetInstance()->IsActionClicked(Controls::Action::right)
     - Settings::GetInstance()->IsActionClicked(Controls::Action::left);
  
  if (!delta)
    return false;

  *value += delta;
  if (*value < 0)
    *value = selections.size() - 1;
  else if (*value >= (int)selections.size())
    *value = 0;
  
  selectionText.drawable.setString("{" + selections[*value] + "}");

  PUSH_EVENT(event);

  return false;
}

void SelectionInteractable::Render(sf::RenderWindow* win) const
{
  Utility::RenderTextWithScale(win, selectionText, nullptr);
}

void SelectionInteractable::SetPosition(sf::Vector2f& pos)
{
  selectionText.drawable.setPosition(pos);
}



KeybindInteractable::KeybindInteractable(sf::Keyboard::Key keyCode, Event::Type eventType)
  :
  Interactable((int)keyCode, eventType)
{
  Utility::InitText(controlText, SMALL_FONT, Keyboard::GetInstance()->GetStringFromKeyCode(keyCode), 
                    ZERO_VECTOR, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = controlText.drawable.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(controlText.drawable.getPosition() + sf::Vector2f(-width / 2.0f, -1.0f), 
                      sf::Vector2f(width + 4.0f, 6.0f), 
                      {173, 103, 78});
}

KeybindInteractable::KeybindInteractable(Settings::Setting setting)
  :
  Interactable(setting)
{
  Utility::InitText(controlText, SMALL_FONT, Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)(*value)), 
                    ZERO_VECTOR, {1.0f, 0.5f}, {255, 229, 181});
  
  float width = controlText.drawable.getGlobalBounds().getSize().x;
  keyBg = RoundedRect(controlText.drawable.getPosition() + sf::Vector2f(-width / 2.0f, -1.0f), 
                      sf::Vector2f(width + 4.0f, 6.0f), 
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

  *value = (int)keyCode;

  controlText.drawable.setString(codeString);
  float width = controlText.drawable.getGlobalBounds().getSize().x;
  keyBg.SetHorizontal(controlText.drawable.getPosition().x - width / 2.0f);
  keyBg.SetDim(sf::Vector2f(width + 4.0f, 6.0f));

  PUSH_EVENT(event);
  
  return true;
}

void KeybindInteractable::Render(sf::RenderWindow* win) const
{
  keyBg.Render(win);
  Utility::RenderTextWithScale(win, controlText, nullptr);
}

void KeybindInteractable::SetPosition(sf::Vector2f& pos)
{
  pos.x -= 2.0f;
  float width = controlText.drawable.getGlobalBounds().getSize().x;
  keyBg.SetHorizontal(controlText.drawable.getPosition().x - width / 2.0f);
  keyBg.SetVertical(pos.y);
  controlText.drawable.setPosition(pos);
}



ListItem::ListItem(std::string name, float vertOffset, Interactable* interactable)
  :
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
      isActive = false;
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
  Utility::RenderTextWithScale(win, displayName, nullptr);
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
  displayName.drawable.setPosition(origin);

  origin.x += 2 * LIST_MARGIN; // Right margin
  interactable.get()->SetPosition(origin);
}

float ListItem::GetVerticalOffset() const
{
  return vertOffset;
}
