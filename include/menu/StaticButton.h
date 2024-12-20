#ifndef STATICBUTTON_H
#define STATICBUTTON_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Textures.h"
#include "Utility.h"
#include "ProgramSettings.h"

#include <string>
#include <vector>

#define SMALL 0
#define MEDIUM 1
#define LARGE 2

#define SMALL_TEXT_OFFSET ProgramSettings::gameScale * sf::Vector2f(0.0f, -0.5f)
#define MEDIUM_TEXT_OFFSET ProgramSettings::gameScale * sf::Vector2f(-2.5f, 6.0f)
#define LARGE_TEXT_OFFSET ProgramSettings::gameScale * sf::Vector2f(-2.5f, 5.0f)

// `ButtonConfig` stores the confguration variables used when initialising a `StaticButton`
struct StaticButtonInit
{
  std::string name;
  Event clickEvent;
  int size;
};

// `StaticButton` represents a button seen in a menu which has no other functionality except to be clicked
class StaticButton
{
public:
  // Constructs `StaticButton` of a specified position, name, size, and click event
  StaticButton(sf::Vector2f pos, StaticButtonInit& config);

  // Renders the button to the screen
  void Render(sf::RenderWindow* win) const;

  // Moves the position of the button by some offset
  void Move(sf::Vector2f offset);

  // Pushes `clickEvent` to the event queue
  void Click() const;

  // Toggles whether or not the button should be rendered in its highlighted state
  void ToggleHighlight();
  
protected:
  sf::Sprite button;    // The sprite used to render the button's background
  sf::Text displayName; // The drawable used to render the button's name

  Event clickEvent; // The event associated with clicking the button

  bool isHighlighted = false; // Whether the button is currently being highlighted
};

#endif