#include "Controls.hpp"

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
    leftKey = GLFW_KEY_A;
    rightKey = GLFW_KEY_D;
    jumpKey = GLFW_KEY_SPACE;
  }
  else if (playerNumber == 1)
  {
    leftKey = GLFW_KEY_LEFT;
    rightKey = GLFW_KEY_RIGHT;
    jumpKey = GLFW_KEY_UP;
  }
}

void Keyboard::Update()
{
  // heldDirection = sf::Keyboard::isKeyPressed(rightKey) - sf::Keyboard::isKeyPressed(leftKey);

  if (heldDirection != 0)
  {
    int i = 0;
    i++;
  }

  jumpPressed = Utility::CheckInitialPress(jumpKey);
}