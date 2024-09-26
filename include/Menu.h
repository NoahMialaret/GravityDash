#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>

#include "Button.h"
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

  // void LoadLayout(Layout layout);

private:
  std::unique_ptr<MenuInterface> interface = nullptr;
  // std::unique_ptr<Layout> curLayout = nullptr;

  // bool isPauseMenu = false;
  std::unique_ptr<Game> backgroundGame = nullptr;
  bool showGame = true;
};



// // Used by the Menu class to have different update functionality based on what menu is currently open
// class Layout
// {
// public:
//   Layout() = default;
//   virtual void DoButtonAction(MenuInterface* interface, Layout* layout) = 0; // Loads a new layout or flags an event based on what button is clicked
//   virtual void Return(MenuInterface* interface, Layout* layout) = 0;
// };

// class MainLayout : public Layout
// {
// public:
//   MainLayout(Menu* menu);
//   void DoButtonAction(MenuInterface* interface, Layout* layout) override;
//   void Return(MenuInterface* interface, Layout* layout) override;
// };

// class OptionsLayout : public Layout
// {
// public:
//   OptionsLayout(bool isPaused);
// private:
//   bool isPaused = false;
// };

#endif