#ifndef CONTROLS_H
#define CONTROLS_H

#include <SFML/Graphics.hpp>

#include <Keyboard.h>

// The `Controls` class acts as an abstraction of user control by mapping certain bindings to Actions  
class Controls
{
public:
  // The different Actions that can be binded to
  enum class Action
  {
    left,
    right,
    up,
    down,
    select,
    escape,
    jump,
    special,
    end
  };

public:
  Controls() = default;
 
  // Returns whether the binding associated with an Action is being held
  virtual bool IsActionHeld(Action action) = 0;  
  // Returns whether the binding associated with an Action was the start of an input
  virtual bool IsActionOnInitialClick(Action action) = 0; 
  // Returns whether the binding associated with an Action is registered as a click
  virtual bool IsActionClicked(Action action) = 0;  

  // Gets the binding associated with an Action
  virtual int GetAction(Action action) const = 0;
  // Sets the binding of a certain Action
  virtual void SetAction(Action action, int binding) = 0;
};

// `KeyboardControls` maps a keyboards key-bindings to actions
class KeyboardControls : public Controls
{
public:
  KeyboardControls() = default;

  // Returns whether the key associated with an Action is being held
  bool IsActionHeld(Action action) override;  
  // Returns whether the key associated with an Action was the start of an input
  bool IsActionOnInitialClick(Action action) override; 
  // Returns whether the key associated with an Action is registered as a click
  bool IsActionClicked(Action action) override; 

  // Gets the key associated with an Action
  int GetAction(Action action) const override;
  // Sets the key binding of a certain Action
  void SetAction(Action action, int key) override;

private:
  // A vector which defines the mapping from keys to actions, where `keys[Action] = Binding`
  std::vector<sf::Keyboard::Key> keys = std::vector<sf::Keyboard::Key>((int)Controls::Action::end, sf::Keyboard::Unknown);
};

#endif