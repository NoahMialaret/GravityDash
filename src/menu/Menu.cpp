#include "Menu.h"

Menu::Menu(Type startMenu)
{
  Push(startMenu);
}

void Menu::Update()
{
  if (!menuStack.empty())
    menuStack.top().get()->Update();
}

void Menu::Render(sf::RenderWindow* win) const
{
  if (!menuStack.empty())
    menuStack.top().get()->Render(win);
}

void Menu::ReloadStack(Type menuType)
{
  while(!menuStack.empty())
    menuStack.pop();

  Push(menuType);
}

void Menu::Push(Type menuType)
{
  menuStack.push(std::unique_ptr<MenuInterface>(LoadMenu(menuType)));
}

void Menu::Return()
{
  assert(!menuStack.empty());
  menuStack.pop();
}

MenuInterface* Menu::LoadMenu(Type menuType)
{
  Event event {Event::Type::menuReturn};
  switch (menuType)
  {
  case Type::main:
  {
    std::vector<StaticButtonInit> buttons =
    {
      {"stats", {Event::Type::pushMenu, {.value = (int)Type::stats}},   MEDIUM},
      {"opts.", {Event::Type::pushMenu, {.value = (int)Type::options}}, MEDIUM},
      {"play",  {Event::Type::pushMenu, {.value = (int)Type::play}},    LARGE},
      {"tutor", {Event::Type::pushMenu, {.value = (int)Type::stats}},   MEDIUM},
      {"exit",  {Event::Type::programClose},                            MEDIUM}
    };

    event.type = Event::Type::programClose;
    return new GridInterface(2, buttons, event);
  }
  case Type::play:
  {
    std::vector<StaticButtonInit> buttons = 
    {
      {"1min",  {Event::Type::gameNew, {.value = (int)GameManager::Preset::minute}}, LARGE},
      {"rush",  {Event::Type::gameNew, {.value = (int)GameManager::Preset::rush}},   LARGE},
      {"co-op", {Event::Type::gameNew, {.value = (int)GameManager::Preset::coop}},   MEDIUM},
      {"vs.",   {Event::Type::gameNew, {.value = (int)GameManager::Preset::vs}},     MEDIUM}
    };

    return new GridInterface(2, buttons, event);
  }
  case Type::pause:
  {
    std::vector<StaticButtonInit> buttons =
    {
      {"resume",  {Event::Type::resume},                                  SMALL},
      {"retry",   {Event::Type::gameReset},                               SMALL},
      {"opts.",   {Event::Type::pushMenu, {.value = (int)Type::options}}, SMALL},
      {"quit",    {Event::Type::gameExit},                                SMALL}
    };

    event.type = Event::Type::resume;
    return new VerticalInterface(buttons, event);
  }
  case Type::options:
  {
    std::vector<std::string> colours{ "orange", "green", "purple", "blue"};

    std::vector<std::pair<std::string, Interactable*>> interactables =
    {
      // Video - 0
      {"auto-scale",  new ToggleInteractable(GET_SETTING(Settings::Setting::autoScale))},
      {"scale",       new RangeInteractable(GET_SETTING(Settings::Setting::scale), 1 ,32)},
      {"fullscreen",  new ToggleInteractable(GET_SETTING(Settings::Setting::fullscreen))},
      {"colour",      new SelectionInteractable(GET_SETTING(Settings::Setting::colour), colours)},

      // Audio - 4
      {"music", new RangeInteractable(GET_SETTING(Settings::Setting::music), 0, 10)},
      {"sfx",   new RangeInteractable(GET_SETTING(Settings::Setting::sfx), 0, 10)},

      // Controls - 6
      {"left",        new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::left))},
      {"right",       new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::right))},
      {"up",          new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::up))},
      {"down",        new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::down))},
      {"jump",        new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::jump))},
      {"select",      new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::select))},
      {"special",     new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::special))},
      {"pause/exit",  new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::escape))},

      // Multiplayer - 14
      {"p1-colour",   new SelectionInteractable(GET_SETTING(Settings::Setting::p1Col), colours)},
      {"p2-colour",   new SelectionInteractable(GET_SETTING(Settings::Setting::p2Col), colours)},
      {"p2-left",     new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::p2Left))},
      {"p2-right",    new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::p2Left))},
      {"p2-jump",     new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::p2Left))},
      {"p2-special",  new KeybindInteractable((sf::Keyboard::Key)GET_SETTING(Settings::Setting::p2Left))},

      // Accessibility - 20
      {"colour-help", new ToggleInteractable(GET_SETTING(Settings::Setting::p1Col))},
      {"world",       new SelectionInteractable(GET_SETTING(Settings::Setting::accWorldCol), colours)},
      {"player",      new SelectionInteractable(GET_SETTING(Settings::Setting::accPlayerCol), colours)},
      {"target",      new SelectionInteractable(GET_SETTING(Settings::Setting::accTargetCol), colours)},
      {"saw",         new SelectionInteractable(GET_SETTING(Settings::Setting::accSawCol), colours)},
      {"time",         new SelectionInteractable(GET_SETTING(Settings::Setting::accTimeCol), colours)},
    };

    std::vector<std::pair<int, std::string>> headers =
    {
      {0,  "video"},
      {4,  "audio"},
      {6,  "controls"},
      {14, "multiplayer"},
      {20, "accessibility"}
    };
    
    return new ListInterface(interactables, headers, event);
  }
  case Type::stats:
  {
    std::vector<std::pair<std::string, Interactable*>> interactables =
    {
      // Stats - 0
      {"games-played", new StaticInteractable(86)},
      {"jumps", new StaticInteractable(1421)},
      {"specials", new StaticInteractable(142)},
      {"hits", new StaticInteractable(64)},

      // 1-Min - 4
      {"1st", new StaticInteractable(100)},
      {"2nd", new StaticInteractable(50)},
      {"3rd", new StaticInteractable(25)},

      // Rush - 7
      {"1st", new StaticInteractable(100)},
      {"2nd", new StaticInteractable(50)},
      {"3rd", new StaticInteractable(25)},

      // Co-op - 10
      {"1st", new StaticInteractable(100)},
      {"2nd", new StaticInteractable(50)},
      {"3rd", new StaticInteractable(25)}
    };

    std::vector<std::pair<int, std::string>> headers =
    {
      {0,  "statistics"},
      {4,  "1-minute"},
      {7,  "rush"},
      {10, "co-op"}
    };

    return new ListInterface(interactables, headers, event);
  }
  case Type::tutor:
  {
    return nullptr;
  }
  case Type::gameEnd:
  {
    std::vector<StaticButtonInit> buttons =
    {
      {"retry",       {Event::Type::gameReset}, SMALL},
      {"main menu",   {Event::Type::gameExit},  SMALL}
    };

    event.type = Event::Type::gameExit;
    return new GameEndInterface(buttons, event);
  }
  }
  
  std::cout << "Menu type [" << (int)menuType << "] was not recognised\n";
  assert(false);
  return nullptr;
}