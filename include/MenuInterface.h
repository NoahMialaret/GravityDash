#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "Button.h"
#include "Event.h" 
#include "Textures.h"
#include "Utility.h"
#include "assert.h"

#include <iostream>

enum class Menus
{
  title,
  main,
  play,
  options,
  score,
  medal,
  custom,
  multi,
  pause
};

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

protected:
  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  int curPos = 2;

  std::vector<std::unique_ptr<Button>> buttons;
  std::vector<int> buttonPos; // Represents which button occupies which grid space, from top to bottom, left to right
};

class ConfigurationInterface : public MenuInterface // For options and custom games
{

};

class PlayerSelectInterface : public MenuInterface
{

};

class PauseInterface : public MenuInterface
{
public:
  PauseInterface();

private:
  std::vector<std::unique_ptr<Button>> buttons;

  int curButtonIndex = -1;
};

#endif