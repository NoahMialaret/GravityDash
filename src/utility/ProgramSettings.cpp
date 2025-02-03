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
  
  p1Controls.SetAction(Controls::Action::left,     (int)sf::Keyboard::A);
  p1Controls.SetAction(Controls::Action::right,    (int)sf::Keyboard::D);
  p1Controls.SetAction(Controls::Action::up,       (int)sf::Keyboard::W);
  p1Controls.SetAction(Controls::Action::down,     (int)sf::Keyboard::S);
  p1Controls.SetAction(Controls::Action::jump,     (int)sf::Keyboard::Space);
  p1Controls.SetAction(Controls::Action::select,   (int)sf::Keyboard::Space);
  p1Controls.SetAction(Controls::Action::special,  (int)sf::Keyboard::LShift);
  p1Controls.SetAction(Controls::Action::escape,   (int)sf::Keyboard::Escape);
  
  p2Controls.SetAction(Controls::Action::left,     (int)sf::Keyboard::Left);
  p2Controls.SetAction(Controls::Action::right,    (int)sf::Keyboard::Right);
  p2Controls.SetAction(Controls::Action::up,       (int)sf::Keyboard::Up);
  p2Controls.SetAction(Controls::Action::down,     (int)sf::Keyboard::Down);
  p2Controls.SetAction(Controls::Action::jump,     (int)sf::Keyboard::Up);
  p2Controls.SetAction(Controls::Action::select,   (int)sf::Keyboard::PageUp);
  p2Controls.SetAction(Controls::Action::special,  (int)sf::Keyboard::RShift);
  p2Controls.SetAction(Controls::Action::escape,   (int)sf::Keyboard::PageDown);

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

  pControls->SetAction(Controls::Action::left,     (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["left"]));
  pControls->SetAction(Controls::Action::right,    (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["right"]));
  pControls->SetAction(Controls::Action::up,       (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["up"]));
  pControls->SetAction(Controls::Action::down,     (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["down"]));
  pControls->SetAction(Controls::Action::select,   (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["select"]));
  pControls->SetAction(Controls::Action::escape,   (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["escape"]));
  pControls->SetAction(Controls::Action::jump,     (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["jump"]));
  pControls->SetAction(Controls::Action::special,  (int)Keyboard::GetInstance()->GetKeyCodeFromString(controls["special"]));
}

void ProgramSettings::SaveControls(nlohmann::json& save, int player)
{
  KeyboardControls* pControls = player == 1 ? &p1Controls : &p2Controls;

  save["multiplayer"]["p" + std::to_string(player) + "Controls"] = {
    {"left",    Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::left))},
    {"right",   Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::right))},
    {"up",      Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::up))},
    {"down",    Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::down))},
    {"select",  Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::select))},
    {"escape",  Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::escape))},
    {"jump",    Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::jump))},
    {"special", Keyboard::GetInstance()->GetStringFromKeyCode((sf::Keyboard::Key)pControls->GetAction(Controls::Action::special))},
  };
}
