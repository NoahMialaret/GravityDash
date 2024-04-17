#ifndef CONTROLS_HPP
#define CONTROLS_HPP

#include "Utility.hpp"

#include <GLFW/glfw3.h>

class Controls
{
public:
  Controls() = default;
  virtual ~Controls() = default;

  virtual void Update() = 0;

  int HeldDirection();
  bool JumpPressed();

protected:
  int heldDirection = 0;
  bool jumpPressed = false;
};

class Keyboard : public Controls
{
public:
  Keyboard(int playerNumber); // Uses playerNumber to determine which keybindings to get

  void Update() override;

private:
  int leftKey;
  int rightKey;
  int jumpKey;
};

// class Controller : public Controls
// {
// public:
//   Controller(int playerNumber); // Uses playerNumber to determine which controller to use
// };

#endif