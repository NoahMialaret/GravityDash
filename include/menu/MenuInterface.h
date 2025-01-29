#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "assert.h"
#include "Clock.h"
#include "Controls.h"
#include "Event.h" 
#include "GameStats.h"
#include "StaticButton.h"
#include "ListItem.h"
#include "AssetManager.h"
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
  Event menuReturn; // The event that is pushed when the menu is escaped from
};


#define GRID_VERT_POS (2.0f * float(SCALED_DIM) - ProgramSettings::gameScale)

// `GridInterface` specifies a layout where buttons are arranged on a 2 x 3 grid
class GridInterface : public MenuInterface
{
public:
  // Constructs `GridInterface` given a list of buttons to include in the layout
  GridInterface(int initialHighlight, std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre = ZERO_VECTOR);

  // Updates the currently highlighted button if a direction is pressed by the user
  void Update() override;
  // Redners the buttons to the screen
  void Render(sf::RenderWindow* win) const override;

private:
  int curButton; // Index that maps to the grid position of the currently highlighted button

  std::vector<int> grid;             // Maps each grid position to an index in `buttons`
  std::vector<StaticButton> buttons; // The buttons that make up the interface
};

// `VerticalInterface` specifies a layout where buttons are arranged vertically, such as the pause menu
class VerticalInterface : public MenuInterface
{
public:
  // Constructs `VerticalInterface` given a list of buttons to include in the layout
  VerticalInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre = ZERO_VECTOR);

  // Updates the currently highlighted button if a direction is pressed by the user
  void Update() override;
  // Redners the buttons to the screen
  void Render(sf::RenderWindow* win) const override;

protected:
  int curButton = 0; // Index to the currently highlighted button

  std::vector<StaticButton> buttons; // The buttons that make up the interface
};

// `GameEndInterface` is a type of `VerticalInterface` shown at the end of a game, and includes game statistics
class GameEndInterface : public VerticalInterface
{
public:
  // Constructs `GameEndInterface` given a list of buttons to include in the layout
  GameEndInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre = ZERO_VECTOR);

  // Redners the buttons and stats to the screen
  void Render(sf::RenderWindow* win) const override;

private:
  sf::Text displayTitle;        // The title to be displayed above the stats
  sf::RectangleShape underline; // The underline for the title
  std::vector<sf::Text> stats;  // A list of text drawables representing the different stats to display
};

// `SubList` specifies a layout where buttons are arranged as a list
class SubList
{
public:
  SubList(std::string& title, std::vector<OptionConfig>& configs, float* origin, float yPos);

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

  std::vector<std::unique_ptr<ListItem>> options;  
};

// `ListInterface` specifies a list of ListItems seperated into different sublists based on category
class ListInterface : public MenuInterface
{
public:
  ListInterface(std::vector<std::pair<std::string, std::vector<OptionConfig>>>& configs, Event menuReturn);
  ~ListInterface();

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int curIndex = 0;

  Bezier bezier;
  float timer = 0.0f;

  float origin;
  float start = 0.0f;
  float end = 0.0f;

  std::vector<std::unique_ptr<SubList>> subLists;
};

#endif