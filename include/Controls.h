#ifndef CONTROLS_H
#define CONTROLS_H

#include <SFML/Graphics.hpp>

#include <Keyboard.h>

class Controls
{
public:
  enum class Binding
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
  virtual ~Controls() = default;
 
  virtual bool IsBindingHeld(Binding binding) = 0;  
  virtual bool IsBindingOnInitialClick(Binding binding) = 0; 
  virtual bool IsBindingClicked(Binding binding) = 0;  
};

class KeyboardControls : public Controls
{
public:
  KeyboardControls() = default;

  bool IsBindingHeld(Binding binding) override;  
  bool IsBindingOnInitialClick(Binding binding) override; 
  bool IsBindingClicked(Binding binding) override; 

  sf::Keyboard::Key GetKeyBinding(Controls::Binding binding) const;
  void SetKeyBinding(Controls::Binding binding, sf::Keyboard::Key key);

private:
  std::vector<sf::Keyboard::Key> keys = std::vector<sf::Keyboard::Key>((int)Controls::Binding::end, sf::Keyboard::Unknown);
};

#endif