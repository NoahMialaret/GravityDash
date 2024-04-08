#ifndef SUBMENU_H
#define SUBMENU_H

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Utility.h"

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
    virtual void Render(sf::RenderWindow* win) const;
    static void SetBottomLeftTitlePosition(sf::Vector2f bottomLeft); // should use the camera

    GameConfig GetGameConfig() const;
private:
    sf::Text name;
    static sf::Vector2f namePosition;
protected:
    GameConfig gameConfig;
};

#endif