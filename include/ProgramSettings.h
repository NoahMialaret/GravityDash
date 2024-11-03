#ifndef PROGRAMSETTINGS_H
#define PROGRAMSETTINGS_H

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include <memory>

#include "Controls.h"
#include "Keyboard.h"

// Static class which holds various user defined settings about the game
class ProgramSettings
{
public:
  enum class Colours
  {
    brown,
    green,
    blue,
    purple
  };

public:
  static void Init();
  static void Init(nlohmann::json& save);
  static void Save(nlohmann::json& save);

  static Controls* GetControls(int playerNum = 1);

  static Colours GetWorldColour();
  static Colours GetPlayerColour(int id);
  static Colours GetTargetColour();
  static Colours GetSawColour();
  static Colours GetTimeColour();

private:
  static void LoadControls(nlohmann::json& controls, int player);
  static void SaveControls(nlohmann::json& save, int player);

public:
  // General Program Settings
  static constexpr float targetFrameRate = 60.0f;   // The max framerate that the game runs at

  static sf::Vector2f windowDim;

  // Video Settings
  static bool autoScale; // Whether the scale of the program is determined automatically on init, or manually by the user
  static float gameScale; // The scale that the game is rendered at compared to the original sprites
  static bool fullScreen;
  static Colours gameColour;

  // Audio Settings
  static int musicVolume;
  static int SFXVolume;

  // Multiplayer Settings
  static Colours p1Colour;
  static Colours p2Colour;
  static KeyboardControls p1Controls;
  static KeyboardControls p2Controls;

  // Accessibility Settings
  static bool useAccessibilityColours;
  static Colours accessibilityWorldColour;
  static Colours accessibilityPlayerColour;
  static Colours accessibilityTargetColour;
  static Colours accessibilitySawColour;
  static Colours accessibilityTimeColour;

};

#endif