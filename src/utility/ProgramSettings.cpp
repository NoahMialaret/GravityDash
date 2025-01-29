#include "ProgramSettings.h"
#include <iostream>

sf::Vector2f ProgramSettings::windowDim;

bool ProgramSettings::autoScale = true; 
float ProgramSettings::gameScale = 6.0f;
bool ProgramSettings::fullScreen = false;
ProgramSettings::Colours ProgramSettings::gameColour = ProgramSettings::Colours::brown;

int ProgramSettings::musicVolume = 10;
int ProgramSettings::SFXVolume = 10;

ProgramSettings::Colours ProgramSettings::p1Colour = ProgramSettings::Colours::brown;
ProgramSettings::Colours ProgramSettings::p2Colour = ProgramSettings::Colours::brown;
KeyboardControls ProgramSettings::p1Controls;
KeyboardControls ProgramSettings::p2Controls;

bool ProgramSettings::useAccessibilityColours = false;
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

  p1Colour = Colours::brown;
  p2Colour = Colours::brown;
  
  p1Controls.SetKeyAction(Controls::Action::left,     sf::Keyboard::A);
  p1Controls.SetKeyAction(Controls::Action::right,    sf::Keyboard::D);
  p1Controls.SetKeyAction(Controls::Action::up,       sf::Keyboard::W);
  p1Controls.SetKeyAction(Controls::Action::down,     sf::Keyboard::S);
  p1Controls.SetKeyAction(Controls::Action::jump,     sf::Keyboard::Space);
  p1Controls.SetKeyAction(Controls::Action::select,   sf::Keyboard::Space);
  p1Controls.SetKeyAction(Controls::Action::special,  sf::Keyboard::LShift);
  p1Controls.SetKeyAction(Controls::Action::escape,   sf::Keyboard::Escape);
  
  p2Controls.SetKeyAction(Controls::Action::left,     sf::Keyboard::Left);
  p2Controls.SetKeyAction(Controls::Action::right,    sf::Keyboard::Right);
  p2Controls.SetKeyAction(Controls::Action::up,       sf::Keyboard::Up);
  p2Controls.SetKeyAction(Controls::Action::down,     sf::Keyboard::Down);
  p2Controls.SetKeyAction(Controls::Action::jump,     sf::Keyboard::Up);
  p2Controls.SetKeyAction(Controls::Action::select,   sf::Keyboard::PageUp);
  p2Controls.SetKeyAction(Controls::Action::special,  sf::Keyboard::RShift);
  p2Controls.SetKeyAction(Controls::Action::escape,   sf::Keyboard::PageDown);

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

    p1Colour = settings["multiplayer"]["p1Colour"];
    p2Colour = settings["multiplayer"]["p2Colour"];
    LoadControls(settings["multiplayer"]["p1Controls"], 1);
    LoadControls(settings["multiplayer"]["p2Controls"], 2);
    
    useAccessibilityColours =   settings["accessibility"]["colourHelp"];
    accessibilityWorldColour =  (Colours)settings["accessibility"]["world"];
    accessibilityPlayerColour = (Colours)settings["accessibility"]["player"];
    accessibilityTargetColour = (Colours)settings["accessibility"]["target"];
    accessibilitySawColour =    (Colours)settings["accessibility"]["saw"];
    accessibilityTimeColour =   (Colours)settings["accessibility"]["time"];
  }
  catch (...)
  {
    std::cout << "Error loading settings...\n";
    throw;
  }
}

void ProgramSettings::Save(nlohmann::json& save)
{
  std::string className = "settings";
  save[className] = {
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
      {"p1Colour", p1Colour},
      {"p2Colour", p2Colour}
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

  SaveControls(save[className], 1);
  SaveControls(save[className], 2);
}

Controls* ProgramSettings::GetControls(int playerNum)
{
  return playerNum == 2 ? &p2Controls : &p1Controls;
}

ProgramSettings::Colours ProgramSettings::GetWorldColour()
{
  return (useAccessibilityColours ? accessibilityWorldColour : gameColour);
}

ProgramSettings::Colours ProgramSettings::GetPlayerColour(int id)
{
  if (id == 0)
    return (useAccessibilityColours ? accessibilityPlayerColour : gameColour);

  return (id == 1 ? p1Colour : p2Colour);
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

void ProgramSettings::LoadControls(nlohmann::json& controls, int player)
{
  KeyboardControls* pControls = player == 1 ? &p1Controls : &p2Controls;

  pControls->SetKeyAction(Controls::Action::left,     Keyboard::GetInstance()->GetKeyCodeFromString(controls["left"]));
  pControls->SetKeyAction(Controls::Action::right,    Keyboard::GetInstance()->GetKeyCodeFromString(controls["right"]));
  pControls->SetKeyAction(Controls::Action::up,       Keyboard::GetInstance()->GetKeyCodeFromString(controls["up"]));
  pControls->SetKeyAction(Controls::Action::down,     Keyboard::GetInstance()->GetKeyCodeFromString(controls["down"]));
  pControls->SetKeyAction(Controls::Action::select,   Keyboard::GetInstance()->GetKeyCodeFromString(controls["select"]));
  pControls->SetKeyAction(Controls::Action::escape,   Keyboard::GetInstance()->GetKeyCodeFromString(controls["escape"]));
  pControls->SetKeyAction(Controls::Action::jump,     Keyboard::GetInstance()->GetKeyCodeFromString(controls["jump"]));
  pControls->SetKeyAction(Controls::Action::special,  Keyboard::GetInstance()->GetKeyCodeFromString(controls["special"]));
}

void ProgramSettings::SaveControls(nlohmann::json& save, int player)
{
  KeyboardControls* pControls = player == 1 ? &p1Controls : &p2Controls;

  save["multiplayer"]["p" + std::to_string(player) + "Controls"] = {
    {"left",    Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::left))},
    {"right",   Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::right))},
    {"up",      Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::up))},
    {"down",    Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::down))},
    {"select",  Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::select))},
    {"escape",  Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::escape))},
    {"jump",    Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::jump))},
    {"special", Keyboard::GetInstance()->GetStringFromKeyCode(pControls->GetKeyAction(Controls::Action::special))},
  };
}
