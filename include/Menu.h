#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "MenuButton.h"
#include "MenuOption.h"
#include "Game.h"
#include "MenuInterface.h"
#include "Utility.h"
#include "Event.h"

#include <memory>

// Handles switching between menu interfaces, and the transitions between them
class Menu
{
public:
  Menu(Event::MenuType startMenu = Event::MenuType::title);
  
  void Update();
  void Render(sf::RenderWindow* win) const;

  void ChangeMenu(Event::MenuType menuType);
  void LoadGameEndMenu(Event::GameStats stats);

private:
  std::unique_ptr<MenuInterface> interface = nullptr;
};

#endif