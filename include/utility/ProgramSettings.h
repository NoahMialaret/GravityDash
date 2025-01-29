#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <memory>

#include "Controls.h"
#include "Keyboard.h"

// Static class used to store user-configurable settings and other program related options
class ProgramSettings
{
public:
  // Colours used by shaders when rendering
  enum class Colours
  {
    brown,
    green,
    blue,
    purple
  };

public:
  // Initialises varaibles with default values
  static void Init();
  // Initialises variables using data stored in a `JSON` object
  static void Init(nlohmann::json& save);
  // Writes stored data to a `JSON` object
  static void Save(nlohmann::json& save);

  // Gets the controls for a specified player
  static Controls* GetControls(int playerNum = 1);

  // Gets the render colour for the game world and background
  static Colours GetWorldColour();
  // Gets the render colour of the specified player
  static Colours GetPlayerColour(int id);
  // Gets the render colour of target entities
  static Colours GetTargetColour();
  // Gets the render colour of saw obstacles
  static Colours GetSawColour();
  // Gets the render colour of time extension entites
  static Colours GetTimeColour();

private:
  // Helper function for `Init()` to load in control related data from a `JSON` object
  static void LoadControls(nlohmann::json& controls, int player);
  // Helper function for `Save()` to save control related data to a `JSON` object
  static void SaveControls(nlohmann::json& save, int player);

public:
  // UNUSED: The maximum number of program cycles that can happen per second 
  static constexpr float targetFrameRate = 60.0f;
  // The dimensions of the program window
  static sf::Vector2f windowDim;

  // Whether the scale of the program is determined automatically when opened, or manually by the user
  static bool autoScale; 
  // The scale that the game is rendered at compared to the original sprites
  static float gameScale; 
  // Whether the program is rendered in fullscreen
  static bool fullScreen;
  // The global colour used when rendering
  static Colours gameColour;

  // The volume of the music used in-game, range of 0 to 10
  static int musicVolume;
  // The volume of the sound effects used in-game, range of 0 to 10
  static int SFXVolume;

  // The render colour of the first player when playing multiplayer
  static Colours p1Colour;
  // The render colour of the second player
  static Colours p2Colour;
  // The controls associated with the first player, also used for menus
  static KeyboardControls p1Controls;
  // The controls associated with the second player
  static KeyboardControls p2Controls;

  // An override used to manually set the colours of different game entities and the background
  static bool useAccessibilityColours;
  static Colours accessibilityWorldColour;
  static Colours accessibilityPlayerColour;
  static Colours accessibilityTargetColour;
  static Colours accessibilitySawColour;
  static Colours accessibilityTimeColour;

};

#endif