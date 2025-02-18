#ifndef SETTINGS_H
#define SETTINGS_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "AssetManager.h"
#include "Controls.h"
#include "Event.h"
#include "Keyboard.h"

#include <iostream>
#include <memory>

// Shortcut to obtain the program rendering scale
#define SCALE (Settings::GetInstance()->GetScale())
// Shortcut to obtain the program rendering scale cast to float
#define FSCALE ((float)SCALE)
// Shortcut to access a given `setting`
#define GET_SETTING(setting) (Settings::GetInstance()->GetSetting(setting))

// `Settings` is a global singleton used to store and access the user-configurable program settings
class Settings
{
public:
  // Available colours that can be used for rendering
  enum class Colours
  {
    brown,
    green,
    blue,
    purple
  };

  // The various settings of the program
  enum class Setting
  {
    autoScale,
    scale,
    fullscreen,
    colour,

    music,
    sfx,

    left,
    right,
    up,
    down,
    jump,
    select,
    special,
    escape,

    p1Col,
    p2Col,
    p2Left,
    p2Right,
    p2Jump,
    p2Special,

    colourHelp,
    accWorldCol,
    accPlayerCol,
    accTargetCol,
    accSawCol,
    accTimeCol,
    end
  };

private:
  // Private constructor as only one instance should exist in the program
  Settings() = default;
  // The global Settings instance
  static Settings* instance;
  // Deletes the Settings instance
  static void Clean();

  // Initialises varaibles with default values
  void Load();
  // Initialises variables using data stored in a `JSON` object
  void Load(nlohmann::json& save);

  // Helper function for `Init()` to initialise the keybinds for the controls
  void InitControls();

public:
  // Creates and returns the global instance of Settings
  static Settings* GetInstance();

  // Writes stored data to a `JSON` object
  void Save(nlohmann::json& save);

  // Returns the value of an associated setting
  int GetSetting(Setting setting) const;

  // Sets the value of an associated setting
  void SetSetting(Setting setting, int val);

  // Returns the scale of the program given the value of the fullscreen and auto-scale settings
  int GetScale() const;
  // Returns the dimensions of the window
  sf::Vector2u GetWindowDim() const;

  // Returns whether the key associated with an Action is being held for a given player
  bool IsActionOnInitialClick(Controls::Action action, int player = 0);
  // Returns whether the key associated with an Action was the start of an input for a given player
  bool IsActionHeld(Controls::Action action, int player = 0);
  // Returns whether the key associated with an Action is registered as a click for a given player
  bool IsActionClicked(Controls::Action action, int player = 0);

  // Sets the render colour used in the world and static shader
  void SetWorldColour();
  // Gets the render colour of the specified player
  Colours GetPlayerColour(int id);
  // Gets the render colour of target entities
  Colours GetTargetColour();
  // Gets the render colour of saw obstacles
  Colours GetSawColour();
  // Gets the render colour of time extension entites
  Colours GetTimeColour();

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  // The stores the values for the various program settings, it maps from the `Setting` enum class
  std::vector<int> settings;

  // The scale associated with the auto-scaling
  int autoScaleVal = -1;
  // The maximum number of program cycles that can happen per second 
  static constexpr unsigned int targetFrameRate = 60;
  // The dimensions of the program window
  sf::Vector2u windowDim;

  // The controls associated with the first player, used by default for menuing
  std::unique_ptr<Controls> p1Controls;
  // The controls associated with the second player
  std::unique_ptr<Controls> p2Controls;
};

#endif