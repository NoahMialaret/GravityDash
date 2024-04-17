#ifndef TEMPMEN_HPP
#define TEMPMEN_HPP

#include "SubMenu.hpp"
#include "Game.hpp"
#include "GridMenu.hpp"
#include "ConfigMenuType.hpp"

class Title : public GridMenu
{
public:
    Title();
    std::unique_ptr<SubMenu> ClickButton() override;
    std::unique_ptr<SubMenu> Return() override;
};

class GameSelection : public GridMenu
{
public:
    GameSelection();
    std::unique_ptr<SubMenu> ClickButton() override;
    std::unique_ptr<SubMenu> Return() override;
};

class MultiplayerMenu : public GridMenu
{
public:
    MultiplayerMenu();
    std::unique_ptr<SubMenu> ClickButton() override;
    std::unique_ptr<SubMenu> Return() override;
};

class ScoresMenu : public GridMenu
{
public:
    ScoresMenu();
    std::unique_ptr<SubMenu> ClickButton() override;
    std::unique_ptr<SubMenu> Return() override;
};

// Might make SubMenu triple inherited (i.e. SubMenu -> Settings -> Options)
class Options : public ConfigMenuType
{
public:
    Options();
    std::unique_ptr<SubMenu> ClickButton() override;
    std::unique_ptr<SubMenu> Return() override;
 
    // tooltips
    //std::funtion prevMenu;
};

// class Config : public SubMenu
// {
// public:
//     Config();
//     SubMenu Update() override;
//     void Render(sf::RenderWindow* win) const override;

// private:
//     std::unique_ptr<GridMenu> gridMenu = nullptr;   // 
// };

#endif