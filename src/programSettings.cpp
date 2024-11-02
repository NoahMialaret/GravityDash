#include "programSettings.h"
#include <iostream>

sf::Vector2f ProgramSettings::windowDim;

bool ProgramSettings::autoScale = true; 
float ProgramSettings::gameScale = 6.0f;
bool ProgramSettings::fullScreen = false;
ProgramSettings::Colours ProgramSettings::gameColour = ProgramSettings::Colours::brown;

int ProgramSettings::musicVolume = 10;
int ProgramSettings::SFXVolume = 10;

ProgramSettings::Colours ProgramSettings::player1Colour = ProgramSettings::Colours::brown;
ProgramSettings::Colours ProgramSettings::player2Colour = ProgramSettings::Colours::brown;

bool ProgramSettings::useAccessibilityColours = false;;
ProgramSettings::Colours ProgramSettings::accessibilityWorldColour = ProgramSettings::Colours::brown;
ProgramSettings::Colours ProgramSettings::accessibilityPlayerColour = ProgramSettings::Colours::blue;
ProgramSettings::Colours ProgramSettings::accessibilityTargetColour = ProgramSettings::Colours::green;
ProgramSettings::Colours ProgramSettings::accessibilitySawColour = ProgramSettings::Colours::purple;
ProgramSettings::Colours ProgramSettings::accessibilityTimeColour = ProgramSettings::Colours::purple;

void ProgramSettings::Init()
{
  autoScale = true; 
  gameScale = 6.0f;
  fullScreen = false;
  gameColour = Colours::brown;

  musicVolume = 10;
  SFXVolume = 10;

  player1Colour = Colours::brown;
  player2Colour = Colours::brown;

  useAccessibilityColours = false;;
  accessibilityWorldColour = Colours::brown;
  accessibilityPlayerColour = Colours::blue;
  accessibilityTargetColour = Colours::green;
  accessibilitySawColour = Colours::purple;
  accessibilityTimeColour = Colours::purple;
}

void ProgramSettings::Init(nlohmann::json& settings)
{
  try
  {
    autoScale =   settings["video"]["autoScale"];
    gameScale =   settings["video"]["scale"];
    fullScreen =  settings["video"]["fullscreen"];
    gameColour =  (Colours)settings["video"]["colour"];

    musicVolume = settings["audio"]["music"];
    SFXVolume =   settings["audio"]["sfx"];

    player1Colour = settings["multiplayer"]["p1Colour"];
    player2Colour = settings["multiplayer"]["p2Colour"];
    
    useAccessibilityColours =   settings["accessibility"]["colourHelp"];
    accessibilityWorldColour =  (Colours)settings["accessibility"]["world"];
    accessibilityPlayerColour = (Colours)settings["accessibility"]["player"];
    accessibilityTargetColour = (Colours)settings["accessibility"]["target"];
    accessibilitySawColour =    (Colours)settings["accessibility"]["saw"];
    accessibilityTimeColour =   (Colours)settings["accessibility"]["time"];
  }
  catch (const nlohmann::json::exception& e)
  {
    std::cout << "Error loading settings...\n";
    throw e;
  }
}

void ProgramSettings::Save(nlohmann::json& save)
{
  save["settings"] = {
    {"video", {
      {"autoScale", autoScale},
      {"scale", gameScale},
      {"fullscreen", fullScreen},
      {"colour", (int)gameColour}
    }},
    {"audio", {
      {"music", musicVolume},
      {"sfx", SFXVolume}
    }},
    {"multiplayer", {
      {"p1Colour", player1Colour},
      {"p2Colour", player2Colour}
    }},
    {"accessibility", {
      {"colourHelp", useAccessibilityColours},
      {"world", (int)accessibilityWorldColour},
      {"player", (int)accessibilityPlayerColour},
      {"target", (int)accessibilityTargetColour},
      {"saw", (int)accessibilitySawColour},
      {"time", (int)accessibilityTimeColour}
    }}
  };
}

ProgramSettings::Colours ProgramSettings::GetWorldColour()
{
  return (useAccessibilityColours ? accessibilityWorldColour : gameColour);
}

ProgramSettings::Colours ProgramSettings::GetPlayerColour(int id)
{
  if (id == 0)
    return (useAccessibilityColours ? accessibilityPlayerColour : gameColour);

  return (id == 1 ? player1Colour : player2Colour);
}

ProgramSettings::Colours ProgramSettings::GetTargetColour()
{
  return (useAccessibilityColours ? accessibilityTargetColour : gameColour);
}

ProgramSettings::Colours ProgramSettings::GetSawColour()
{
  return (useAccessibilityColours ? accessibilitySawColour : gameColour);
}

ProgramSettings::Colours ProgramSettings::GetTimeColour()
{
  return (useAccessibilityColours ? accessibilitySawColour : gameColour);
}
