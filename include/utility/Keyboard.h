#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <forward_list>
#include <unordered_map>

#include "Clock.h"

// The number of milliseconds between Key clicks
#define CLICK_TIME 50

// Shorthand for accessing `Keyboard`'s `IsKeyHeld()` function
#define IS_KEY_HELD Keyboard::GetInstance()->IsKeyHeld
// Shorthand for accessing `Keyboard`'s `IsKeyOnInitialClick()` function
#define IS_KEY_INITIAL Keyboard::GetInstance()->IsKeyOnInitialClick
// Shorthand for accessing `Keyboard`'s `IsKeyClicked()` function
#define IS_KEY_CLICKED Keyboard::GetInstance()->IsKeyClicked

// `Keyboard` is a global singleton class used to poll which keys have been 
//    pressed in a frame and converting key codes to and from string representations
class Keyboard
{
private:
  // The `Key` class represents the state of a single active key
  class Key
  {
  public:
    // The press state of the `Key`
    enum class State
    {
      initialClick,
      hold,
      click
    };

  public:
    // Constructs `Key` given its key code
    Key(sf::Keyboard::Key keyCode);

    // Updates the state of the Key based on the time it has been held for
    void Update();

  public:
    sf::Keyboard::Key keyCode; // The key code

    State state = State::initialClick; // The current press state of the Key

    int clickTimer = 250; // The timer used to determine when a 'click' is registered while the Key is held down
  };

private:
  // Private constructor as only one instance should exist in the program
  Keyboard() = default;
  // The global Keyboard instance
  static Keyboard* instance;
  // Deletes the Keyboard instance
  static void Clean();

  // Updates the state of all the actively pressed Keys
  void Update();
  // Adds a Key to the list of active Keys
  void AddKeyPress(sf::Keyboard::Key keyCode);
  // Searches for a given key code in the active list of Keys, returns `nullptr` if not found
  const Key* GetKey(sf::Keyboard::Key keyCode) const;

public:
  // Creates and returns the global instance of Keyboard
  static Keyboard* GetInstance();

  // Checks whether a Key is being held
  bool IsKeyHeld(sf::Keyboard::Key keyCode) const;
  // Checks whether a Key has become active within the last update cycle
  bool IsKeyOnInitialClick(sf::Keyboard::Key keyCode) const;
  // Checks whether a Key 'click' has been registered
  bool IsKeyClicked(sf::Keyboard::Key keyCode) const;

  // Converts a key code to a string, returns `"NULL"` if not found
  std::string GetStringFromKeyCode(sf::Keyboard::Key keyCode) const;
  // Converts a string to a key code, throws `std::invalid_argument` if not found
  sf::Keyboard::Key GetKeyCodeFromString(std::string keyString) const;

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  // The list of currently active keys
  std::forward_list<Key> activeKeys; 

  // Maps key codes to strings for easier lookup when converting between the two
  const std::unordered_map<int, std::string> keyCodeStringMap =
  {
    {(int)sf::Keyboard::Key::Escape,     "esc"},
    {(int)sf::Keyboard::Key::Tab,        "tab"},
    {(int)sf::Keyboard::Key::Enter,      "enter"},
    {(int)sf::Keyboard::Key::Backspace,  "bcksp"},
    {(int)sf::Keyboard::Key::LShift,     "lshft"},
    {(int)sf::Keyboard::Key::RShift,     "rshft"},
    {(int)sf::Keyboard::Key::LControl,   "lctrl"},
    {(int)sf::Keyboard::Key::RControl,   "rctrl"},
    {(int)sf::Keyboard::Key::Space,      "space"},
    {(int)sf::Keyboard::Key::LAlt,       "lalt"},
    {(int)sf::Keyboard::Key::RAlt,       "ralt"},
    {(int)sf::Keyboard::Key::Up,         "up"},
    {(int)sf::Keyboard::Key::Down,       "down"},
    {(int)sf::Keyboard::Key::Left,       "left"},
    {(int)sf::Keyboard::Key::Right,      "right"},
    {(int)sf::Keyboard::Key::PageUp,     "p-up"},
    {(int)sf::Keyboard::Key::PageDown,   "p-down"},
    {(int)sf::Keyboard::Key::Grave,      "`"},
    {(int)sf::Keyboard::Key::Hyphen,     "-"},
    {(int)sf::Keyboard::Key::Equal,      "="},
    {(int)sf::Keyboard::Key::LBracket,   "["},
    {(int)sf::Keyboard::Key::RBracket,   "]"},
    {(int)sf::Keyboard::Key::Backslash,  "\\"},
    {(int)sf::Keyboard::Key::Semicolon,  ";"},
    {(int)sf::Keyboard::Key::Apostrophe, "'"},
    {(int)sf::Keyboard::Key::Comma,      ","},
    {(int)sf::Keyboard::Key::Period,     "."},
    {(int)sf::Keyboard::Key::Slash,      "/"}
  };
};

#endif