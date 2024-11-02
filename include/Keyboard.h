#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <SFML/Graphics.hpp>

#include <string>
#include <vector>
#include <forward_list>

#include "Clock.h"

// Provides utility for keyboard related functions and variables, including polling which keys have
//  been pressed in a frame, and converting key codes to and from string representations
class Keyboard
{
public:
  class Key
  {
  public:
    enum class State
    {
      initialClick,
      hold,
      click
    };

  public:
    Key(sf::Keyboard::Key keyCode);
    void Update();

  public:
    sf::Keyboard::Key keyCode;
    State state = State::initialClick;
    int clickTimer = 250;
  };

public:
  // Adds a key to `keysPressed`
  static void AddKeyPress(sf::Keyboard::Key keyCode);
  // Checks whether a key is being held
  static bool IsKeyHeld(sf::Keyboard::Key keyCode);
  // Checks whether a key has just been clicked within the last update cycle
  static bool IsKeyOnInitialClick(sf::Keyboard::Key keyCode);
  // Checks whether a key 'click' has been registered (used for things like menu scrolling)
  static bool IsKeyClicked(sf::Keyboard::Key keyCode);

  // Updates the state of keys in `keysPressed`
  static void Update();

  // Converts a key code to a string, returns `"NULL"` if not found
  static std::string GetStringFromKeyCode(sf::Keyboard::Key keyCode);
  // Converts a string to a key code, returns `sf::keyboard::Unknown` if not found
  static sf::Keyboard::Key GetKeyCodeFromString(std::string keyString);

private:
  // Searches for a given `keyCode` in `keysPressed`, returns `nullptr` if not found
  static Key* GetKey(sf::Keyboard::Key keyCode);

private:
  // Holds keys which were initially pressed in the current frame cycle
  static std::forward_list<Key> keysPressed; 

  // Stores pairs of key codes and strings for easier lookup when converting between the two
  static const std::vector<std::pair<sf::Keyboard::Key, std::string>> keyCodeStringPairs;
};

#endif