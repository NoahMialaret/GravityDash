#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "StaticButton.h"
#include "MenuOption.h"
#include "Game.h"
#include "MenuInterface.h"
#include "Utility.h"
#include "Event.h"
#include "ProgramSettings.h"

#include <memory>
#include <stack>

// Handles switching between menu interfaces, and the transitions between them
class Menu
{
public:
  Menu(Event::MenuType startMenu = Event::MenuType::title);
  
  void Update();
  void Render(sf::RenderWindow* win) const;

  void ReloadStack(Event::MenuType menuType); 
  void Push(Event::MenuType menuType); 
  void Return();

private:
  void LoadMenu(Event::MenuType menuType);

private:
  std::unique_ptr<MenuInterface> interface = nullptr;
  std::stack<Event::MenuType> menuStack;
};

#endif