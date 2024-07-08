#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "SubMenu.h"
#include "tempMenu.h"
#include "Utility.h"

#include <iostream>
#include <memory>

class MainMenu
{

public:
    MainMenu(bool showIntro = false);

    void Update();
    void Return();
    void Render(sf::RenderWindow* win) const;
    void CorrectCharacterPos(Character* cpu);

    GameConfig GetGameConfig() const;
private:
    std::unique_ptr<SubMenu> curSubMenu = nullptr;
    std::unique_ptr<Game> game = nullptr;

  //   std::vector<std::unique_ptr<Character>> cpus;
  //   std::unique_ptr<World> world = nullptr;

  // SortedDblyLnkdList<Entity> entities; // A sorted (by vertical position) linked list of entites
  // sf::Texture entityTex; // The sprite sheet used by all entities

    // std::unique_ptr<GridMenu> gridMenu = nullptr;   // A menu used to represent grid portions of the main menu (such as the title screen)
    // A menu used to represent listed menus (such as for the config or custom game menus)

};

#endif