#include "Controls.h"

int Controls::HeldDirection()
{
  return heldDirection;
}

bool Controls::JumpPressed()
{
  return jumpPressed;
}

bool Controls::SuperJumpPressed()
{
  return superJumpPressed;
}

KeyboardControls::KeyboardControls(int playerNumber)
{
  // Get keybindings for player: <playerNumber>
  if (playerNumber == 0)
  {
    leftKey = sf::Keyboard::Key::A;
    rightKey = sf::Keyboard::Key::D;
    jumpKey = sf::Keyboard::Key::Space;
    superJumpKey = sf::Keyboard::Key::LShift;
  }
  else if (playerNumber == 1)
  {
    leftKey = sf::Keyboard::Key::Left;
    rightKey = sf::Keyboard::Key::Right;
    jumpKey = sf::Keyboard::Key::Up;
    superJumpKey = sf::Keyboard::Key::RShift;
  }
}

void KeyboardControls::Update()
{
  heldDirection = Keyboard::IsKeyHeld(rightKey) - Keyboard::IsKeyHeld(leftKey);

  if (heldDirection != 0)
  {
    int i = 0;
    i++;
  }


  jumpPressed = Keyboard::IsKeyOnInitialClick(jumpKey);
  superJumpPressed = Keyboard::IsKeyOnInitialClick(superJumpKey);
}