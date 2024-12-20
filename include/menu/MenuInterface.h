#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "assert.h"
#include "Clock.h"
#include "Controls.h"
#include "Event.h" 
#include "GameStats.h"
#include "StaticButton.h"
#include "MenuOption.h"
#include "Textures.h"
#include "Utility.h"
#include "ProgramSettings.h"
#include "Keyboard.h"

#include <iostream>

// Represents an interface layout that can be interacted with by the user
class MenuInterface
{
public:
  // Constructs `MenuInterface` with a given event to be called when the menu is escaped from
  MenuInterface(Event menuReturn);

  // Pushes the `menuReturn` event if the approriate key is pressed
  virtual void Update();
  // Renders interface elements to the screen
  virtual void Render(sf::RenderWindow* win) const = 0;

protected:
  // sf::Text Title;
  Event menuReturn; // The event that is pushed when 
};

class TitleInterface : public MenuInterface
{

};


#define GRID_HORI 4.5f * float(SCALED_DIM)
#define GRID_VERT (2.0f * float(SCALED_DIM) - ProgramSettings::gameScale)

// `GridInterface` specifies a layout where buttons are arranged on a 2 x 3 grid
class GridInterface : public MenuInterface
{
public:
  // Constructs `GridInterface` given the button to 
  GridInterface(int initialHighlight, std::vector<StaticButtonInit>& configs, Event menuReturn);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int curButton = 2;  // Index to the current as to the specific grid position that 
                      //  is highlighted, used to determine button selection

  std::vector<std::unique_ptr<StaticButton>> buttons; // The buttons that make up the interface
  std::vector<int> buttonPos;                       // Maps each grid position to a button
};

class ListInterface : public MenuInterface // Represents a list of small buttons, like the pause menu
{
public:
  ListInterface(std::vector<StaticButtonInit>& configs, Event menuReturn);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

protected:
  // An invisible indicator as to the specific grid position that is highlighted, used to determine button selection
  int curButton = 0;

  std::vector<std::unique_ptr<StaticButton>> buttons;
};

class GameEndInterface : public ListInterface
{
public:
  GameEndInterface(std::vector<StaticButtonInit>& configs, Event menuReturn);

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

#endif