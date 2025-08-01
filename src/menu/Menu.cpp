#include "Menu.h"

Menu::Menu(Type startMenu)
{
  Push(startMenu);
  lighter.setFillColor({200, 200, 200, 150});
  lighter.setSize((sf::Vector2f)ASPECT_RATIO);
  lighter.setPosition(-0.5f * (sf::Vector2f)ASPECT_RATIO);
}

void Menu::Update()
{
  if (!menuStack.empty())
  {
    menuStack.top().get()->Update();
    return;
  }
  
  if (!Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
    return;

  // No menu is present and pause button is pressed -> pause game
  PUSH_EVENT({Event::Type::pause});
}

void Menu::Render(sf::RenderWindow* win) const
{
  if (menuStack.empty())
    return;
  Utility::RenderRectWithScale(win, lighter);

  menuStack.top().get()->Render(win);
}

void Menu::ReloadStack(Type menuType)
{
  Clear();
  Push(menuType);
}

void Menu::Clear()
{
  while(!menuStack.empty())
    menuStack.pop();
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
    std::vector<std::string> colours{ "brown", "green", "blue", "purple"};

    std::vector<std::pair<std::string, Interactable*>> interactables =
    {
      // Video - 0
      {"auto-scale",  new ToggleInteractable(Settings::Setting::autoScale)},
      {"scale",       new RangeInteractable(Settings::Setting::scale, 1 ,32)},
      {"fullscreen",  new ToggleInteractable(Settings::Setting::fullscreen)},
      {"colour",      new SelectionInteractable(Settings::Setting::colour, colours)},

      // Audio - 4
      {"music", new RangeInteractable(Settings::Setting::music, 0, 10)},
      {"sfx",   new RangeInteractable(Settings::Setting::sfx, 0, 10)},

      // Controls - 6
      {"left",        new KeybindInteractable(Settings::Setting::left)},
      {"right",       new KeybindInteractable(Settings::Setting::right)},
      {"up",          new KeybindInteractable(Settings::Setting::up)},
      {"down",        new KeybindInteractable(Settings::Setting::down)},
      {"jump",        new KeybindInteractable(Settings::Setting::jump)},
      {"select",      new KeybindInteractable(Settings::Setting::select)},
      {"special",     new KeybindInteractable(Settings::Setting::special)},
      {"pause/exit",  new KeybindInteractable(Settings::Setting::escape)},

      // Multiplayer - 14
      {"p1-colour",   new SelectionInteractable(Settings::Setting::p1Col, colours)},
      {"p2-colour",   new SelectionInteractable(Settings::Setting::p2Col, colours)},
      {"p2-left",     new KeybindInteractable(Settings::Setting::p2Left)},
      {"p2-right",    new KeybindInteractable(Settings::Setting::p2Right)},
      {"p2-jump",     new KeybindInteractable(Settings::Setting::p2Jump)},
      {"p2-special",  new KeybindInteractable(Settings::Setting::p2Special)},

      // Accessibility - 20
      {"distinct-cols", new ToggleInteractable(Settings::Setting::colourHelp)},
      {"world",         new SelectionInteractable(Settings::Setting::accWorldCol, colours)},
      {"player",        new SelectionInteractable(Settings::Setting::accPlayerCol, colours)},
      {"target",        new SelectionInteractable(Settings::Setting::accTargetCol, colours)},
      {"saw",           new SelectionInteractable(Settings::Setting::accSawCol, colours)},
      {"time",          new SelectionInteractable(Settings::Setting::accTimeCol, colours)},
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
      {"games-played", new StaticInteractable(GET_STAT(Stats::StatType::gamesPlayed))},
      {"jumps", new StaticInteractable(GET_STAT(Stats::StatType::jumps))},
      {"specials", new StaticInteractable(GET_STAT(Stats::StatType::specials))},
      {"times-hit", new StaticInteractable(GET_STAT(Stats::StatType::hits))},
    };

    std::vector<std::string> positions{"1st", "2nd", "3rd", "4th", "5th"};
    std::vector<Stats::StatType> scoreStats
    {
      Stats::StatType::minScores, 
      Stats::StatType::rushScores, 
      Stats::StatType::coopScores
    };

    for (auto& scoreStat : scoreStats)
    {
      for (int i = 0; i < positions.size() && i < MAX_HIGHSCORES; i++)
      {
        interactables.push_back
        ({
          positions[i], 
          new StaticInteractable(GET_HIGHSCORE(scoreStat, i))
        });
      }
    }

    std::vector<std::pair<int, std::string>> headers =
    {
      {0,  "statistics"},
      {4,  "1-minute"},
      {4 + MAX_HIGHSCORES,  "rush"},
      {4 + 2 * MAX_HIGHSCORES, "co-op"}
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
      {"retry", {Event::Type::gameReset}, SMALL},
      {"leave", {Event::Type::gameExit},  SMALL}
    };

    event.type = Event::Type::gameExit;
    return new GameEndInterface(buttons, event);
  }
  }
  
  std::cout << "Menu type [" << (int)menuType << "] was not recognised\n";
  assert(false);
  return nullptr;
}