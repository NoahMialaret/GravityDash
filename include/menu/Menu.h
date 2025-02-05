#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "GameManager.h"
#include "ListItem.h"
#include "MenuInterface.h"
#include "Settings.h"
#include "StaticButton.h"
#include "Utility.h"

#include <memory>
#include <stack>

// Returns the keybinding associated with the `action` for a given player `p`
// #define KEYBIND_ACTION(p, action) (sf::Keyboard::Key)Settings::GetInstance()->GetSetting()->GetAction(action)

// `Menu` provides a simple interface to load and interface with the menu objects of the program,  
class Menu
{
public:
  // The different menus interfaces, determines layout and functionality
  enum class Type
  {
    gameEnd,
    main,
    options,
    pause,
    play,
    tutor,
    stats,
  };

public:
  // Constructs Menu given an initial interface to load
  Menu(Type startMenu = Type::main);
  
  // Updates the underlying menu interface
  void Update();
  // Renders UI components to the screen
  void Render(sf::RenderWindow* win) const;

  // Resets the interface stack and loads a new interface
  void ReloadStack(Type menuType); 
  // Pushes an interface to the stack
  void Push(Type menuType); 
  // Pops an interface from the stack and returns to the previous interface
  void Return();

private:
  // Loads a new menu
  MenuInterface* LoadMenu(Type menuType);

private:
  // The stack of interfaces representing the path the user took to arrive at the interface at `top()`
  std::stack<std::unique_ptr<MenuInterface>> menuStack;
};

#endif