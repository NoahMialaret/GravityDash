#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "assert.h"
#include "Clock.h"
#include "Event.h" 
#include "GameStats.h"
#include "MenuButton.h"
#include "MenuOption.h"
#include "Textures.h"
#include "Utility.h"
#include "ProgramSettings.h"
#include "Keyboard.h"

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
#define GRID_VERT (2.0f * float(SCALED_DIM) - ProgramSettings::gameScale)

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

protected:
  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  int curPos = 0;

  std::vector<std::unique_ptr<MenuButton>> buttons;
};

class GameEndInterface : public ListInterface
{
public:
  GameEndInterface(std::vector<ButtonConfig>& configs, Event menuReturn);

  void Render(sf::RenderWindow* win) const override;

private:
  std::vector<sf::Text> stats;
  sf::Text displayTitle;
  sf::RectangleShape underline;
};

class OptionsSubList
{
public:
  OptionsSubList(std::string& title, std::vector<OptionConfig>& configs, float* origin, float yPos);

  void Update();
  void Render(sf::RenderWindow* win) const;

  void GoTo(int index);
  bool Move(int move);

private:
  int curIndex = 0;

  float* origin;
  float vertOffset = 0.0f;

  sf::Text displayTitle;
  sf::RectangleShape overline;
  sf::RectangleShape underline;

  std::vector<std::unique_ptr<MenuOption>> options;  
};

class OptionsInterface : public MenuInterface // For options and custom games
{
public:
  OptionsInterface(std::vector<std::pair<std::string, std::vector<OptionConfig>>>& configs, Event menuReturn);
  ~OptionsInterface();

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int curIndex = 0;

  Bezier bezier;
  float timer = 0.0f;

  float origin;
  float start = 0.0f;
  float end = 0.0f;

  std::vector<std::unique_ptr<OptionsSubList>> subLists;
};

class PlayerSelectInterface : public MenuInterface
{

};

#endif