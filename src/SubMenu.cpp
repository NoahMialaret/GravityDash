#include "SubMenu.h"

sf::Vector2f SubMenu::namePosition;

SubMenu::SubMenu(std::string menuName)
{
    name.setString(menuName);
    name.setFont(Textures::font);
    name.setCharacterSize(SCALED_DIM);
    name.setFillColor(sf::Color(173, 103, 78));
    name.setOrigin(name.getLocalBounds().left, name.getLocalBounds().height + name.getLocalBounds().top);
    name.setPosition(namePosition);
}

void SubMenu::Render(sf::RenderWindow *win) const
{
    win->draw(name);
}

void SubMenu::SetBottomLeftTitlePosition(sf::Vector2f bottomLeft)
{
    namePosition = bottomLeft;
}

GameConfig SubMenu::GetGameConfig() const
{
  return gameConfig;
}
