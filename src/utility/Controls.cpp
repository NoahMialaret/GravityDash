#include "Controls.h"

bool KeyboardControls::IsActionHeld(Action action)
{
  return Keyboard::IsKeyHeld(keys[(int)action]);
}

bool KeyboardControls::IsActionOnInitialClick(Action action)
{
  return Keyboard::IsKeyOnInitialClick(keys[(int)action]);
}

bool KeyboardControls::IsActionClicked(Action action)
{
  return Keyboard::IsKeyClicked(keys[(int)action]);
}

sf::Keyboard::Key KeyboardControls::GetKeyAction(Controls::Action action) const
{
  if (action < (Controls::Action)0 || action >= Controls::Action::end)
    return sf::Keyboard::Unknown;

  return keys[(int)action];
}

void KeyboardControls::SetKeyAction(Controls::Action action, sf::Keyboard::Key key)
{
  keys[(int)action] = key;
}
