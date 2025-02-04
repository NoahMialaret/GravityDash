#include "Settings.h"

Settings* Settings::instance = nullptr;

void Settings::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

void Settings::Load()
{
  settings = 
  {
    true,                       // autoScale
    6,                          // scale
    false,                      // fullscreen
    (int)Colours::brown,        // colour

    10,                         // music
    10,                         // sfx

    (int)sf::Keyboard::A,       // left
    (int)sf::Keyboard::D,       // right
    (int)sf::Keyboard::W,       // up
    (int)sf::Keyboard::S,       // down
    (int)sf::Keyboard::Space,   // jump
    (int)sf::Keyboard::Space,   // select
    (int)sf::Keyboard::LShift,  // special
    (int)sf::Keyboard::Escape,  // escape

    (int)Colours::blue,         // p1Col
    (int)Colours::green,        // p2Col
    (int)sf::Keyboard::Left,    // p2Left
    (int)sf::Keyboard::Right,   // p2Right
    (int)sf::Keyboard::Up,      // p2Jump
    (int)sf::Keyboard::RShift,  // p2Special

    false,                      // colourHelp
    (int)Colours::brown,        // accWorldCol
    (int)Colours::blue,         // accPlayerCol
    (int)Colours::green,        // accTargetCol
    (int)Colours::purple,       // accSawCol
    (int)Colours::purple,       // accTimeCol
  };

  InitControls();
}

void Settings::Load(nlohmann::json& json)
{
  try
  {
    settings = 
    {
      json["video"]["autoScale"],
      json["video"]["scale"],
      json["video"]["fullscreen"],
      json["video"]["colour"],

      json["audio"]["music"],
      json["audio"]["sfx"],

      json["multiplayer"]["p1Controls"]["left"],
      json["multiplayer"]["p1Controls"]["right"],
      json["multiplayer"]["p1Controls"]["up"],
      json["multiplayer"]["p1Controls"]["down"],
      json["multiplayer"]["p1Controls"]["jump"],
      json["multiplayer"]["p1Controls"]["select"],
      json["multiplayer"]["p1Controls"]["special"],
      json["multiplayer"]["p1Controls"]["escape"],

      json["multiplayer"]["p1Colour"],
      json["multiplayer"]["p2Colour"],
      json["multiplayer"]["p2Controls"]["left"],
      json["multiplayer"]["p2Controls"]["right"],
      json["multiplayer"]["p2Controls"]["jump"],
      json["multiplayer"]["p2Controls"]["special"],
      
      json["accessibility"]["colourHelp"],
      json["accessibility"]["world"],
      json["accessibility"]["player"],
      json["accessibility"]["target"],
      json["accessibility"]["saw"],
      json["accessibility"]["time"],
    };

    InitControls();
  }
  catch (...)
  {
    std::cout << "Error loading json...\n";
    throw;
  }
}

Settings* Settings::GetInstance()
{
  if (instance == nullptr)
    instance = new Settings();
  return instance;
}

void Settings::Save(nlohmann::json &save)
{
  std::string className = "settings";
  save[className] = {
    {"video", {
      {"autoScale",   settings[(int)Setting::autoScale]},
      {"scale",       settings[(int)Setting::scale]},
      {"fullscreen",  settings[(int)Setting::fullscreen]},
      {"colour",      settings[(int)Setting::colour]}
    }},
    {"audio", {
      {"music", settings[(int)Setting::music]},
      {"sfx",   settings[(int)Setting::sfx]}
    }},
    {"multiplayer", {
      {"p1Colour", settings[(int)Setting::p1Col]},
      {"p2Colour", settings[(int)Setting::p2Col]},
      {"p1Controls", {
        {"left",    settings[(int)Setting::left]},
        {"right",   settings[(int)Setting::right]},
        {"up",      settings[(int)Setting::up]},
        {"down",    settings[(int)Setting::down]},
        {"jump",    settings[(int)Setting::jump]},
        {"special", settings[(int)Setting::special]},
        {"select",  settings[(int)Setting::select]},
        {"escape",  settings[(int)Setting::escape]},
      }},
      {"p2Controls", {
        {"left",    settings[(int)Setting::p2Left]},
        {"right",   settings[(int)Setting::p2Right]},
        {"jump",    settings[(int)Setting::p2Jump]},
        {"special", settings[(int)Setting::p2Special]},
      }},
    }},
    {"accessibility", {
      {"colourHelp",  settings[(int)Setting::colourHelp]},
      {"world",       settings[(int)Setting::accWorldCol]},
      {"player",      settings[(int)Setting::accPlayerCol]},
      {"target",      settings[(int)Setting::accTargetCol]},
      {"saw",         settings[(int)Setting::accSawCol]},
      {"time",        settings[(int)Setting::accTimeCol]}
    }}
  };
}

int Settings::GetSetting(Setting setting) const
{
  return settings[(int)setting];
}

void Settings::SetSetting(Setting setting, int val)
{
  if (settings[(int)setting] == val)
    return;

  settings[(int)setting] = val;

  if ((setting >= Setting::left && setting <= Setting::escape) 
      || (setting >= Setting::p2Left && setting <= Setting::p2Special))
    InitControls();

  else if (setting == Setting::autoScale && autoScaleVal != settings[(int)Setting::scale])
    PUSH_EVENT(Event::Type::updateScale);
  else if (setting == Setting::scale && !settings[(int)Setting::autoScale])
    PUSH_EVENT(Event::Type::updateScale);

  else if (setting == Setting::fullscreen)
    PUSH_EVENT(Event::Type::fullscreen);

}

int Settings::GetScale() const
{
  return settings[(int)Setting::autoScale] ? autoScaleVal : settings[(int)Setting::scale];
}

bool Settings::IsActionOnInitialClick(Controls::Action action, int player)
{
  return (player == 0 ? p1Controls : p2Controls).get()->IsActionOnInitialClick(action);
}

bool Settings::IsActionHeld(Controls::Action action, int player)
{
  return (player == 0 ? p1Controls : p2Controls).get()->IsActionHeld(action);
}

bool Settings::IsActionClicked(Controls::Action action, int player)
{
  return (player == 0 ? p1Controls : p2Controls).get()->IsActionClicked(action);
}

Settings::Colours Settings::GetWorldColour()
{
  return (Settings::Colours)(settings[(int)Setting::colourHelp] 
          ? settings[(int)Setting::accWorldCol] 
          : settings[(int)Setting::colour]);
}

Settings::Colours Settings::GetPlayerColour(int id)
{
  if (id == 0)
    return (Settings::Colours)(settings[(int)Setting::colourHelp] 
            ? settings[(int)Setting::accPlayerCol] 
            : settings[(int)Setting::colour]);

  return (Settings::Colours)(id == 1 
          ? settings[(int)Setting::p1Col]
          : settings[(int)Setting::p2Col]);
}

Settings::Colours Settings::GetTargetColour()
{
  return (Settings::Colours)(settings[(int)Setting::colourHelp] 
          ? settings[(int)Setting::accTargetCol] 
          : settings[(int)Setting::colour]);
}

Settings::Colours Settings::GetSawColour()
{
  return (Settings::Colours)(settings[(int)Setting::colourHelp] 
          ? settings[(int)Setting::accSawCol] 
          : settings[(int)Setting::colour]);
}

Settings::Colours Settings::GetTimeColour()
{
  return (Settings::Colours)(settings[(int)Setting::colourHelp] 
          ? settings[(int)Setting::accTimeCol] 
          : settings[(int)Setting::colour]);
}

void Settings::InitControls()
{
  KeyboardControls* p1Cont = new KeyboardControls();

  p1Cont->SetAction(Controls::Action::left,     (sf::Keyboard::Key)settings[(int)Setting::left]);
  p1Cont->SetAction(Controls::Action::right,    (sf::Keyboard::Key)settings[(int)Setting::right]);
  p1Cont->SetAction(Controls::Action::up,       (sf::Keyboard::Key)settings[(int)Setting::up]);
  p1Cont->SetAction(Controls::Action::down,     (sf::Keyboard::Key)settings[(int)Setting::down]);
  p1Cont->SetAction(Controls::Action::select,   (sf::Keyboard::Key)settings[(int)Setting::select]);
  p1Cont->SetAction(Controls::Action::escape,   (sf::Keyboard::Key)settings[(int)Setting::escape]);
  p1Cont->SetAction(Controls::Action::jump,     (sf::Keyboard::Key)settings[(int)Setting::jump]);
  p1Cont->SetAction(Controls::Action::special,  (sf::Keyboard::Key)settings[(int)Setting::special]);

  p1Controls = std::unique_ptr<Controls>(p1Cont);

  KeyboardControls* p2Cont = new KeyboardControls();

  p2Cont->SetAction(Controls::Action::left,     (sf::Keyboard::Key)settings[(int)Setting::p2Left]);
  p2Cont->SetAction(Controls::Action::right,    (sf::Keyboard::Key)settings[(int)Setting::p2Right]);
  p2Cont->SetAction(Controls::Action::jump,       (sf::Keyboard::Key)settings[(int)Setting::p2Jump]);
  p2Cont->SetAction(Controls::Action::special,     (sf::Keyboard::Key)settings[(int)Setting::p2Special]);

  p2Controls = std::unique_ptr<Controls>(p2Cont);
}