#ifndef MENUINTERFACE_H
#define MENUINTERFACE_H

#include <SFML/Graphics.hpp>

#include "AssetManager.h"
#include "Attachment.h"
#include "Bezier.h"
#include "BezierTransition.h"
#include "Controls.h"
#include "Event.h" 
#include "ListItem.h"
#include "Settings.h"
#include "StaticButton.h"
#include "Stats.h"
#include "Utility.h"

#include <assert.h>
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
  // mutable Text Title;
  Event menuReturn; // The event that is pushed when the menu is escaped from
};

// The vertical offset of the top and bottom row from the centre of the grid
#define GRID_VERT_POS (2.0f * float(SPRITE_DIM) - 1.0f)

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
  mutable Text displayTitle;        // The title to be displayed above the stats
  mutable sf::RectangleShape underline; // The underline for the title
  mutable std::vector<Text> stats;  // A list of text drawables representing the different stats to display
};

// The `Header` class is a graphical class used by `ListInterface` to display header text along with an under/over-line
class Header
{
public:
  // Constructs Header given its text and the vertical offset from some origin
  Header(std::string text, float vertOffset);

  // Render the Header to the screen
  void Render(sf::RenderWindow* win) const;

  // Sets the position the Header's text and lines
  void SetPosition(sf::Vector2f pos);

private:
  mutable Text displayTitle;        // The graphical text portion of the Header
  mutable sf::RectangleShape overline;  // The Header's overline, which appears above the text
  mutable sf::RectangleShape underline; // The Header's underline, which appears below the text

  float vertOffset; // The vertical offset of the Header from some origin
};

// `ListInterface` represents a menu layout composed of a list of `ListItem`s with different sections defined by a list of `Header`s
class ListInterface : public MenuInterface
{
public:
  // Constructs `ListInterface` given a vector of `{name, Interactable}` pairs used to initialise ListItems, a vector of `{index, name}` pairs for the headers, and the event to be called when returning from the menu
  ListInterface(std::vector<std::pair<std::string, Interactable*>>& inters, std::vector<std::pair<int, std::string>>& headers, Event menuReturn);

  // Updates which ListItem is highlighted or interacts with the current ListItem based on user inputs
  void Update() override;
  // Renders the ListItems and Headers to the screen
  void Render(sf::RenderWindow* win) const override;

private:
  // Updates the position of all ListItems and Headers
  void UpdateAllPositions();

private:
  std::vector<ListItem> list;   // The vector of ListItems
  std::vector<Header> headers;  // The vector of Headers

  int curIndex = 0; // The index of the current ListItem in `list`

  sf::Vector2f origin; // The origin that all ListItems and Headers base their position off of

  BezierTransition<sf::Vector2f> translation; // Used to allow smooth scrolling between ListItems

  RoundedRect highlight;  // The highlight used to display which ListItem is active
};

#endif