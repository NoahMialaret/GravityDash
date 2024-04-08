#ifndef GRIDMENU_H
#define GRIDMENU_H

#define GRID_WIDTH 14
#define GRID_HEIGHT 6

#include <SFML/Graphics.hpp>

#include "SubMenu.h"
#include "Utility.h"

#include <iostream>

// Used for menus that use a grid based button layout, suitable for displaying selectable buttons
class GridMenu : public SubMenu
{
private:
    class Button
    {
    public:
        Button() = default;
        Button(sf::Texture& tex, std::string name, sf::IntRect gridBounds);
        Button(const Button& button);
        bool OccupiesGridPos(sf::Vector2i gridPos) const;
        void ToggleHighlight();
        std::string GetName() const;
        sf::IntRect GetBounds() const;

        void Render(sf::RenderWindow* win) const;
        
    private:
        sf::Texture tex;
        std::vector<sf::Sprite> sprites;
        sf::Text name;

        // The name of the button, used by other classes to determine which button was selected
        //std::string name;
        // A rectangle that determines the button's grid position and size
        sf::IntRect gridBounds;


        bool isHighlighted = false;
    };
public:
    GridMenu(std::string name, int numButtons);
    static void SetTexture(const char* filePath);

    void Update() override;
    void Render(sf::RenderWindow* win) const override;
    void CreateButton(std::string name, sf::IntRect gridBounds);
    std::string Select() const;

    static bool IsInBounds(sf::Vector2i gridPos);
    static sf::Vector2i IsInBoundsAxis(sf::Vector2i gridPos);

private:
    static sf::Texture buttonTex;

    // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
    sf::Vector2i selectorPos;

    std::vector<Button> buttons;
    int curButtonIndex = -1;

    sf::RectangleShape renderRect;
};

#endif