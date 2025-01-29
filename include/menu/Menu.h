#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Game.h"
#include "GameManager.h"
#include "MenuInterface.h"
#include "ListItem.h"
#include "ProgramSettings.h"
#include "StaticButton.h"
#include "Utility.h"

#include <memory>
#include <stack>

// Handles switching between menu interfaces, and the transitions between them
class Menu
{
public:
  // The different menus that can be interfaced by the user, determines layout and functionality
  enum class Type
  {
    gameEnd,
    main,
    options,
    pause,
    play,
    score,
    stats,
    title
  };

public:
  Menu(Type startMenu = Type::title);
  
  void Update();
  void Render(sf::RenderWindow* win) const;

  void ReloadStack(Type menuType); 
  void Push(Type menuType); 
  void Return();

private:
  void LoadMenu(Type menuType);

private:
  std::unique_ptr<MenuInterface> interface = nullptr;
  std::stack<Type> menuStack;
};

#endif