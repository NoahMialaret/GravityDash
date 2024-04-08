#ifndef MAINMENU_H
#define MAINMENU_H

#include "Entity.h"
#include "Particle.h"
#include "Character.h"
#include "Game.h"
#include "SortedDblyLnkdList.h"
#include "SubMenu.h"
#include "tempMenu.h"
#include "Utility.h"
#include "World.h"

#include <iostream>
#include <memory>

// struct CustomGame
// {
//      speed
/*  
*/
// };

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
    std::vector<std::unique_ptr<Character>> cpus;
    std::unique_ptr<World> world = nullptr;

  SortedDblyLnkdList<Entity> entities; // A sorted (by vertical position) linked list of entites
  sf::Texture entityTex; // The sprite sheet used by all entities

    // std::unique_ptr<GridMenu> gridMenu = nullptr;   // A menu used to represent grid portions of the main menu (such as the title screen)
    // A menu used to represent listed menus (such as for the config or custom game menus)

};

#endif