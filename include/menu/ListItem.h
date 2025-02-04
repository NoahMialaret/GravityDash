#ifndef LISTITEM_H
#define LISTITEM_H

#include "SFML/Graphics.hpp"

#include "AssetManager.h"
#include "Controls.h"
#include "Event.h"
#include "Keyboard.h"
#include "Settings.h"
#include "RoundedRect.h"
#include "Utility.h"

#include <string>
#include <vector>

// The left and right margin offset of the list, essentially dictates the width of each ListItem
#define LIST_MARGIN (5 * SPRITE_DIM)

// `Interactable` is a UI element that provides the user with some form of interaction to perform configurations
class Interactable
{
public:
  // Constructs Interactable given an initial value and the type of event to push
  Interactable(int value, Event::Type eventType = Event::Type::null);
  // Constructs Interactable given the setting that it is tied to
  Interactable(Settings::Setting setting);

  // Allows the user to interact with the Interactable, returns `true` if the interaction has finished
  virtual bool Update() = 0;
  // Renders Interactable elements to the screen
  virtual void Render(sf::RenderWindow* win) const = 0;

  // Returns the value represented by the Interactable
  int GetValue() const;

  // Sets the position of graphical UI elements to the position provided
  virtual void SetPosition(sf::Vector2f& pos) = 0;

protected:
  
  Event event; // The event pushed when the value is changed

  int* value = nullptr;   // A pointer to the value of the Interactable stored in `event`, this is the value that is configured by the user
  int backup = -1;  // A backup of the value, this is used when the user cancels an interaction without saving a new value

};

// `StaticInteractable` is a `Interactable` which does not actually provide any interactablitly, instead only displaying some static text
class StaticInteractable : public Interactable
{
public:
  // Constructs StaticInteractable with some value (this is the static value that is displayed to the user) and an event type
  StaticInteractable(int value, Event::Type eventType = Event::Type::null);

  // Returns `true` if the interaction has finished. This always returns `true` immediately
  bool Update() override;
  // Renders the static text to the screen
  void Render(sf::RenderWindow* win) const override;

  // Sets the position of the static text to some new position
  void SetPosition(sf::Vector2f& pos) override;

private:
  mutable Text staticText;  // The static graphical representation of the StaticInteractable's value
};

// `ToggleInteractable` is an `Interactable` which acts as a boolean toggle, which can either be 'off' or 'on'
class ToggleInteractable : public Interactable
{
public:
  // Constructs ToggleInteractable with an intial toggle state and event type
  ToggleInteractable(bool isToggled, Event::Type eventType = Event::Type::null);
  // Constructs ToggleInteractable given the setting that it is tied to
  ToggleInteractable(Settings::Setting setting);

  // Returns `true` if the interaction has finished. This toggles the toggle and returns `true` immediately after
  bool Update() override;
  // Renders the toggle sprite to the screen
  void Render(sf::RenderWindow* win) const override;

  // Sets the position of the toggle sprite to some new position
  void SetPosition(sf::Vector2f& pos) override;

private:
  void UpdateSprite();

private:
  mutable sf::Sprite toggleSprite;  // The sprite used to represent the toggle status of ToggleInteractable
};

// `RangeInteractable` is an `Interactable` whose value can range from some minimum to some maximum
class RangeInteractable : public Interactable
{
public:
  // Constructs RangeInteractable given some value, the bounds this value can take, and the event type
  RangeInteractable(int value, int min, int max, 
                    Event::Type eventType = Event::Type::null);
  // Constructs RangeInteractable given the setting that it is tied to
  RangeInteractable(Settings::Setting setting, int min, int max);

  // Increments the value based on if the user presses the left or right button. Returns `true` if the interaction has finished
  bool Update() override;
  // Renders the display text to the screen
  void Render(sf::RenderWindow* win) const override;
  
  // Sets the position of the display text to some new position
  void SetPosition(sf::Vector2f& pos) override;

private:
  int min = -1; // The minimum bounds of the value
  int max = -1; // The maximum bounds of the value

  mutable Text rangeText; // The graphical representation of the current value
};

// `SelectionInteractable` is an `Interactable` which allows the selection of some option given a list of available options
class SelectionInteractable : public Interactable
{
public:
  // Constructs SelectionInteractable given an initial index (this is the value used by the Interactable), the list of available selections, and the event type
  SelectionInteractable(int index, std::vector<std::string>& selections, 
                        Event::Type eventType = Event::Type::null);
  // Constructs SelectionInteractable given the setting that it is tied to
  SelectionInteractable(Settings::Setting setting, std::vector<std::string>& selections);

  // Increments the index and subsequent selection based on if the user presses the left or right button. Returns `true` if the interaction has finished
  bool Update() override;
  // Renders the display text to the screen
  void Render(sf::RenderWindow* win) const override;

  // Sets the position of the display text to some new position
  void SetPosition(sf::Vector2f& pos) override;

private:
  std::vector<std::string> selections; // The array of selections that can be chosen from

  mutable Text selectionText; // The graphical representation of the current selection
};

// `KeybindInteractable` is an `Interactable` which allows configuration of keybindings
class KeybindInteractable : public Interactable
{
public:
  // Constructs KeybindInteractable given an intial key code and the event type
  KeybindInteractable(sf::Keyboard::Key keyCode, 
                      Event::Type eventType = Event::Type::null);
  // Constructs KeybindInteractable given the setting that it is tied to
  KeybindInteractable(Settings::Setting setting);

  // Awaits for a key to be pressed, updates the stored keybind when one is pressed and returns `true` to signify the interaction has finished
  bool Update() override;
  // Renders the keybinding to the screen
  void Render(sf::RenderWindow* win) const override;

  // Sets the position of the keybinding text to some new position
  void SetPosition(sf::Vector2f& pos) override;

private:
  mutable RoundedRect keyBg;    // The background rectangle to display behind the text
  mutable Text controlText; // The text used to represent which key is currently binded
};

// `ListItem` represents a UI element apart of a larger list that can be interacted with by the user 
class ListItem
{
public:
  // Constructs ListItem given its display name, the vertical offset from the origin of the list, and its Interactable
  ListItem(std::string name, float vertOffset, Interactable* interactable);

  // Allows the user to interact with the ListItem if it is active
  bool Update();
  // Renders the display text and Interactable to the screen
  void Render(sf::RenderWindow* win) const;

  // Returns whether the ListItem is active or not
  bool IsActive() const;

  // Sets the position of the ListItem given an origin
  void SetPosition(sf::Vector2f origin);
  // Returns the vertical offset of the listItem from the origin
  float GetVerticalOffset() const;

private:
  // The interactable used to allow interactions with the ListItem
  std::unique_ptr<Interactable> interactable = nullptr;

  // The name to display to describe what the ListItem represents to the user
  mutable Text displayName;

  // Whether the ListItem is interactable or not
  bool isActive = false;

  // The vertical positional offset of the ListItem with reference to some origin position 
  float vertOffset = 0.0f;
};

#endif