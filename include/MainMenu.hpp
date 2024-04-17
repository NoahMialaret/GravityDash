#ifndef MAINMENU_HPP
#define MAINMENU_HPP

#include "Entity.hpp"
#include "Particle.hpp"
#include "Character.hpp"
#include "Game.hpp"
#include "SortedDblyLnkdList.hpp"
#include "SubMenu.hpp"
#include "tempMenu.hpp"
#include "Texture.hpp"
#include "Utility.hpp"
#include "World.hpp"

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
  void Render() const;
  void CorrectCharacterPos(Character* cpu);

  GameConfig GetGameConfig() const;

private:
  std::unique_ptr<SubMenu> curSubMenu = nullptr;
  std::vector<std::unique_ptr<Character>> cpus;
  std::unique_ptr<World> world = nullptr;

  SortedDblyLnkdList<Entity> entities;  // A sorted (by vertical position) linked list of entites
  Texture entityTex;                    // The sprite sheet used by all entities
  Texture buttonTex;
  Texture charTex;

  // std::unique_ptr<GridMenu> gridMenu = nullptr;   // A menu used to represent grid portions of the main menu (such as the title screen)
  // A menu used to represent listed menus (such as for the config or custom game menus)
};

#endif