#include "Controls.h"

int Controls::HeldDirection()
{
  return heldDirection;
}

bool Controls::JumpPressed()
{
  return jumpPressed;
}

Keyboard::Keyboard(int playerNumber)
{
  // Get keybindings for player: <playerNumber>
  if (playerNumber == 0)
  {
    leftKey = sf::Keyboard::Key::A;
    rightKey = sf::Keyboard::Key::D;
    jumpKey = sf::Keyboard::Key::Space;
  }
  else if (playerNumber == 1)
  {
    leftKey = sf::Keyboard::Key::Left;
    rightKey = sf::Keyboard::Key::Right;
    jumpKey = sf::Keyboard::Key::Up;
  }
}

void Keyboard::Update()
{
  heldDirection = sf::Keyboard::isKeyPressed(rightKey) - sf::Keyboard::isKeyPressed(leftKey);

  if (heldDirection != 0)
  {
    int i = 0;
    i++;
  }

  jumpPressed = Utility::CheckInitialPress(jumpKey);
}