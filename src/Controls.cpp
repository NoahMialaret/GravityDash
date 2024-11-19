#include "Controls.h"

bool KeyboardControls::IsBindingHeld(Binding binding)
{
  return Keyboard::IsKeyHeld(keys[(int)binding]);
}

bool KeyboardControls::IsBindingOnInitialClick(Binding binding)
{
  return Keyboard::IsKeyOnInitialClick(keys[(int)binding]);
}

bool KeyboardControls::IsBindingClicked(Binding binding)
{
  return Keyboard::IsKeyClicked(keys[(int)binding]);
}

sf::Keyboard::Key KeyboardControls::GetKeyBinding(Controls::Binding binding) const
{
  if (binding < (Controls::Binding)0 || binding >= Controls::Binding::end)
    return sf::Keyboard::Unknown;

  return keys[(int)binding];
}

void KeyboardControls::SetKeyBinding(Controls::Binding binding, sf::Keyboard::Key key)
{
  keys[(int)binding] = key;
}
