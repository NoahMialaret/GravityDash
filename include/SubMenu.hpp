#ifndef SUBMENU_HPP
#define SUBMENU_HPP

#include "Game.hpp"
#include "Utility.hpp"

#include <iostream>
#include <memory>

/* Menu hierachy is as follows ('*'s are not a State class):
//  Title
//      Play
//          Standard*
//          Simple*
//          Multiplayer
//              -
//          Custom Game
//              -
//      Leaderboards/Stats
//      Achievements
//      Config
//      Quit*
*/

// Class representing the various sub menus that the main menu can be in, 
// segregates and simplifies code that would otherwise be included in MainMenu's Update();
class SubMenu
{
public:
  SubMenu(std::string name);
  virtual ~SubMenu() = default;
  virtual void Update() = 0;
  virtual std::unique_ptr<SubMenu> ClickButton() = 0;
  virtual std::unique_ptr<SubMenu> Return() = 0;
  virtual void Render() const;
  static void SetBottomLeftTitlePosition(fVec2 bottomLeft); // should use the camera

  GameConfig GetGameConfig() const;
private:
  //sf::Text name;
  static fVec2 namePosition;
protected:
  GameConfig gameConfig;
};

#endif