#include "Controls.h"

bool KeyboardControls::IsActionHeld(Action action)
{
  return IS_KEY_HELD(keys[(int)action]);
}

bool KeyboardControls::IsActionOnInitialClick(Action action)
{
  return IS_KEY_INITIAL(keys[(int)action]);
}

bool KeyboardControls::IsActionClicked(Action action)
{
  return IS_KEY_CLICKED(keys[(int)action]);
}

int KeyboardControls::GetAction(Controls::Action action) const
{
  if (action < (Controls::Action)0 || action >= Controls::Action::end)
    return sf::Keyboard::Unknown;

  return (int)keys[(int)action];
}

void KeyboardControls::SetAction(Controls::Action action, int key)
{
  keys[(int)action] = (sf::Keyboard::Key)key;
}
