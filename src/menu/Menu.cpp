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
      {"auto-scale",  new ToggleInteractable(true)},
      {"scale",       new RangeInteractable(6, 1 ,32)},
      {"fullscreen",  new ToggleInteractable(false)},
      {"colour",      new SelectionInteractable(0, colours)},

      // Audio - 4
      {"music", new RangeInteractable(5, 0, 10)},
      {"sfx",   new RangeInteractable(3, 0, 10)},

      // Controls - 6
      {"left",        new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::left))},
      {"right",       new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::right))},
      {"up",          new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::up))},
      {"down",        new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::down))},
      {"jump",        new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::jump))},
      {"select",      new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::select))},
      {"special",     new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::special))},
      {"pause/exit",  new KeybindInteractable(KEYBIND_ACTION(1, Controls::Action::escape))},

      // Multiplayer - 14
      {"p1-colour",   new SelectionInteractable(0, colours)},
      {"p2-colour",   new SelectionInteractable(1, colours)},
      {"p2-left",     new KeybindInteractable(KEYBIND_ACTION(2, Controls::Action::left))},
      {"p2-right",    new KeybindInteractable(KEYBIND_ACTION(2, Controls::Action::right))},
      {"p2-jump",     new KeybindInteractable(KEYBIND_ACTION(2, Controls::Action::jump))},
      {"p2-special",  new KeybindInteractable(KEYBIND_ACTION(2, Controls::Action::special))},

      // Accessibility - 20
      {"colour-help", new ToggleInteractable(false)},
      {"world",       new SelectionInteractable(0, colours)},
      {"player",      new SelectionInteractable(1, colours)},
      {"target",      new SelectionInteractable(2, colours)},
      {"saw",         new SelectionInteractable(3, colours)}
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