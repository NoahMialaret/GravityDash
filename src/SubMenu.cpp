#include "SubMenu.hpp"

fVec2 SubMenu::namePosition;

SubMenu::SubMenu(std::string menuName)
{
    // name.setString(menuName);
    // name.setFont(Utility::programFont);
    // name.setCharacterSize(SCALED_DIM);
    // name.setFillColor(sf::Color(173, 103, 78));
    // name.setOrigin(name.getLocalBounds().left, name.getLocalBounds().height + name.getLocalBounds().top);
    // name.setPosition(namePosition);
}

void SubMenu::Render() const
{
    // win->draw(name);
}

void SubMenu::SetBottomLeftTitlePosition(fVec2 bottomLeft)
{
    namePosition = bottomLeft;
}

GameConfig SubMenu::GetGameConfig() const
{
  return gameConfig;
}
