#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "MenuButton.h"
#include "MenuOption.h"
#include "Event.h" 
#include "Textures.h"
#include "Utility.h"
#include "assert.h"

#include <iostream>

class MenuInterface
{
public:
  MenuInterface(Event menuReturn);

  virtual void Update() = 0;
  virtual void Render(sf::RenderWindow* win) const = 0;

protected:
  // int colourCode = 0;
  // sf::Text Title;
  Event menuReturn;
};

class TitleInterface : public MenuInterface
{

};


#define GRID_HORI 4.5f * float(SCALED_DIM)
#define GRID_VERT (2.0f * float(SCALED_DIM) - Utility::gameScale)

class GridInterface : public MenuInterface // Represents a 2 x 3 grid of tiles that can be filled with buttons
{
public:
  GridInterface(int startPos, std::vector<ButtonConfig>& configs, Event menuReturn);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  int curPos = 2;

  std::vector<std::unique_ptr<MenuButton>> buttons;
  std::vector<int> buttonPos; // Represents which button occupies which grid space, from top to bottom, left to right
};

class ListInterface : public MenuInterface // Represents a list of small buttons, like the pause menu
{
public:
  ListInterface(std::vector<ButtonConfig>& configs, Event menuReturn);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  int curPos = 0;

  std::vector<std::unique_ptr<MenuButton>> buttons;
};

class OptionsInterface : public MenuInterface // For options and custom games
{
public:
  OptionsInterface(std::vector<OptionConfig> configs, Event menuReturn);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int curIndex = 0;

  std::vector<std::unique_ptr<MenuOption>> options;
};

class PlayerSelectInterface : public MenuInterface
{

};

#endif