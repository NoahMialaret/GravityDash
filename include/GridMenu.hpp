#ifndef GRIDMENU_HPP
#define GRIDMENU_HPP

#define GRID_WIDTH 14
#define GRID_HEIGHT 6

#include "Rect.hpp"
#include "Sprite.hpp"
#include "SubMenu.hpp"
#include "Texture.hpp"
#include "Utility.hpp"
#include "Vec2.hpp"

#include <iostream>

// Used for menus that use a grid based button layout, suitable for displaying selectable buttons
class GridMenu : public SubMenu
{
private:
  class Button
  {
  public:
    Button() = default;
    Button(Texture& tex, std::string name, IRect gridBounds);
    Button(const Button& button);
    bool OccupiesGridPos(iVec2 gridPos) const;
    void ToggleHighlight();
    std::string GetName() const;
    IRect GetBounds() const;

    void Render() const;

  private:
    std::vector<Sprite> sprites;
    // sf::Text name;

    // The name of the button, used by other classes to determine which button was selected
    std::string name;

    // A rectangle that determines the button's grid position and size
    IRect gridBounds;

    bool isHighlighted = false;
  };

public:
  GridMenu(std::string name, int numButtons);
  static void SetTexture(const char* filePath);

  void Update() override;
  void Render() const override;
  void CreateButton(std::string name, IRect gridBounds);
  std::string Select() const;

  static bool IsInBounds(iVec2 gridPos);
  static iVec2 IsInBoundsAxis(iVec2 gridPos);

  static Texture buttonTex;
private:

  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  iVec2 selectorPos;

  std::vector<Button> buttons;
  int curButtonIndex = -1;

  // sf::RectangleShape renderRect;
};

#endif