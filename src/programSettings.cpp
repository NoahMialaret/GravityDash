#include "programSettings.h"

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
