#ifndef CONTROLS_H
#define CONTROLS_H

#include <SFML/Graphics.hpp>

#include "Utility.h"

class Controls
{
public:
  Controls() = default;
  virtual ~Controls() = default;

  virtual void Update() = 0;

  int HeldDirection();
  bool JumpPressed();
  bool SuperJumpPressed();

protected:
  int heldDirection = 0;
  bool jumpPressed = false;
  bool superJumpPressed = false;
};

class Keyboard : public Controls
{
public:
  Keyboard(int playerNumber); // Uses playerNumber to determine which keybindings to get

  void Update() override;

private:
  sf::Keyboard::Key leftKey;
  sf::Keyboard::Key rightKey;
  sf::Keyboard::Key jumpKey;
  sf::Keyboard::Key superJumpKey;
};

// class Controller : public Controls
// {
// public:
//   Controller(int playerNumber); // Uses playerNumber to determine which controller to use
// };

#endif