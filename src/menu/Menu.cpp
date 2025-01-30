#include "Menu.h"

Menu::Menu(Type startMenu)
{
  Push(startMenu);
}

void Menu::Update()
{
  interface.get()->Update();
}

void Menu::Render(sf::RenderWindow* win) const
{
  interface.get()->Render(win);
}

void Menu::ReloadStack(Type menuType)
{
  while(!menuStack.empty())
    menuStack.pop();

  Push(menuType);
}

void Menu::Push(Type menuType)
{
  menuStack.push(menuType);
  LoadMenu(menuType);
}

void Menu::Return()
{
  assert(!menuStack.empty());
  menuStack.pop();
  LoadMenu(menuStack.top());
}

void Menu::LoadMenu(Type menuType)
{
  Event event;
  switch (menuType)
  {
  case Type::main:
  {
    std::vector<StaticButtonInit> buttons;

    event.type = Event::Type::pushMenu;
    event.data.value = (int)Type::score;
    buttons.push_back({"score", event, MEDIUM});
    event.data.value = (int)Type::options;
    buttons.push_back({"opts.", event, MEDIUM});
    event.data.value = (int)Type::play;
    buttons.push_back({"play", event, LARGE});
    event.data.value = (int)Type::stats;
    buttons.push_back({"stats", event, MEDIUM});
    event.type = Event::Type::programClose;
    buttons.push_back({"exit", event, MEDIUM});

    event.type = Event::Type::programClose;
    interface = std::make_unique<GridInterface>(2, buttons, event);
    break;
  }
  case Type::play:
  {
    std::vector<StaticButtonInit> buttons;

    event.type = Event::Type::gameNew;
    event.data.value = (int)GameManager::Preset::minute;
    buttons.push_back({"1min", event, LARGE});
    event.data.value = (int)GameManager::Preset::rush;
    buttons.push_back({"rush", event, LARGE});
    event.data.value = (int)GameManager::Preset::coop;
    buttons.push_back({"co-op", event, MEDIUM});
    event.data.value = (int)GameManager::Preset::vs;
    buttons.push_back({"vs.", event, MEDIUM});

    event.type = Event::Type::menuReturn;
    interface = std::make_unique<GridInterface>(2, buttons, event);
    break;
  }
  case Type::pause:
  {
    std::vector<StaticButtonInit> buttons;

    event.type = Event::Type::resume;
    buttons.push_back({"resume", event, SMALL});

    event.type = Event::Type::gameReset;
    buttons.push_back({"retry", event, SMALL});

    event.type = Event::Type::pushMenu;
    event.data.value = (int)Type::options;
    buttons.push_back({"opts.", event, SMALL});

    event.type = Event::Type::gameExit;
    buttons.push_back({"quit", event, SMALL});

    event.type = Event::Type::resume;
    interface = std::make_unique<VerticalInterface>(buttons, event);

    break;
  }
  case Type::options:
  {
    // std::vector<std::pair<std::string, std::vector<OptionConfig>>> options;

    // event.type = Event::Type::gameExit;

    std::vector<std::string> colours{"green", "orange", "purple"};

    std::vector<std::pair<std::string, Interactable*>> interactables =
    {
      {"Temp", new Static(12390)},
      {"auto-scale", new Toggle(false)},
      {"scale", new Range(6, 1, 32)},
      {"Colour", new Selection(1, colours)},
      {"Select", new Control((int)sf::Keyboard::Key::Space)},
    };

    std::vector<std::pair<int, std::string>> headers =
    {
      {0, "Video"},
      {2, "Other Stuff"}
    };
    
    event.type = Event::Type::menuReturn;
    interface = std::make_unique<ListInterface>(interactables, headers, event);

    // Video
    // options.push_back({"video", {}});
    // OptionConfig option = {"auto-scale", event, OptionConfig::Type::toggle};
    // option.toggle = {true};
    // options[0].second.push_back(option);

    // option = {"scale", event, OptionConfig::Type::range};
    // option.range = {(int)ProgramSettings::gameScale, 1, 32};
    // options[0].second.push_back(option);

    // option = {"fullscreen", event, OptionConfig::Type::toggle};
    // option.toggle = {false};
    // options[0].second.push_back(option);

    // option = {"colour", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[0].second.push_back(option);

    // // Audio
    // options.push_back({"audio", {}});
    // option = {"sfx", event, OptionConfig::Type::range};
    // option.range = {6, 0, 10};
    // options[1].second.push_back(option);

    // option = {"music", event, OptionConfig::Type::range};
    // option.range = {8, 0, 10};
    // options[1].second.push_back(option);
    
    // // Controls
    // options.push_back({"controls", {}});
    // option = {"left", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::A};
    // options[2].second.push_back(option);
    // option = {"right", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::D};
    // options[2].second.push_back(option);
    // option = {"up", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::W};
    // options[2].second.push_back(option);
    // option = {"down", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::S};
    // options[2].second.push_back(option);
    // option = {"jump", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::Space};
    // options[2].second.push_back(option);
    // option = {"select", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::Space};
    // options[2].second.push_back(option);
    // option = {"special", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::LShift};
    // options[2].second.push_back(option);
    // option = {"return", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::Escape};
    // options[2].second.push_back(option);

    // // Multiplayer
    // options.push_back({"multiplayer", {}});
    // option = {"p1 colour", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[3].second.push_back(option);
    // option = {"p2 colour", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[3].second.push_back(option);
    // option = {"p2 left", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::A};
    // options[3].second.push_back(option);
    // option = {"p2 right", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::D};
    // options[3].second.push_back(option);
    // option = {"p2 jump", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::W};
    // options[3].second.push_back(option);
    // option = {"p2 special", event, OptionConfig::Type::control};
    // option.control = {sf::Keyboard::Key::S};
    // options[3].second.push_back(option);
    
    // // Accessibility
    // options.push_back({"accessibility", {}});
    // option = {"colour help", event, OptionConfig::Type::toggle};
    // option.toggle = {false};
    // options[4].second.push_back(option);
    // option = {"world", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[4].second.push_back(option);
    // option = {"player", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[4].second.push_back(option);
    // option = {"targets", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[4].second.push_back(option);
    // option = {"saws", event, OptionConfig::Type::selection};
    // option.selection = {0, &colours};
    // options[4].second.push_back(option);

    // event.type = Event::Type::menuReturn;
    // interface = std::make_unique<ListInterface>(options, event);
    break;
  }
  case Type::stats:
  {
    // std::vector<std::pair<std::string, std::vector<OptionConfig>>> options;

    // event.type = Event::Type::gameExit;

    // // Stats
    // options.push_back({"statistics", {}});
    // OptionConfig option = {"jumps", event, OptionConfig::Type::stat};
    // std::string tmp = std::to_string(56);
    // option.statText = {&tmp};
    // options[0].second.push_back(option);

    // option = {"specials", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[0].second.push_back(option);
    // option = {"hits", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[0].second.push_back(option);
    // option = {"games played", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[0].second.push_back(option);

    // event.type = Event::Type::menuReturn;
    // interface = std::make_unique<ListInterface>(options, event);
    break;
  }
  case Type::score:
  {
    // std::vector<std::pair<std::string, std::vector<OptionConfig>>> options;

    event.type = Event::Type::gameExit;

    // 1-min
    // options.push_back({"1-minute", {}});
    // OptionConfig option = {"1st", event, OptionConfig::Type::stat};
    // std::string tmp = std::to_string(56);
    // option.statText = {&tmp};
    // options[0].second.push_back(option);

    // option = {"2nd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[0].second.push_back(option);
    // option = {"3rd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[0].second.push_back(option);

    // // Rush
    // options.push_back({"Rush", {}});
    // option = {"1st", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[1].second.push_back(option);
    // option = {"2nd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[1].second.push_back(option);
    // option = {"3rd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[1].second.push_back(option);

    // // Co-op
    // options.push_back({"Co-op", {}});
    // option = {"1st", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[2].second.push_back(option);
    // option = {"2nd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[2].second.push_back(option);
    // option = {"3rd", event, OptionConfig::Type::stat};
    // option.statText = {&tmp};
    // options[2].second.push_back(option);

    // event.type = Event::Type::menuReturn;
    // interface = std::make_unique<ListInterface>(options, event);
    break;
  }
  case Type::gameEnd:
  {
    Event event;

    std::vector<StaticButtonInit> buttons;

    event.type = Event::Type::gameReset;
    buttons.push_back({"retry", event, SMALL});

    event.type = Event::Type::gameExit;
    buttons.push_back({"quit", event, SMALL});

    interface = std::make_unique<GameEndInterface>(buttons, event);

    break;
  }
  
  
  default:
    break;
  }
}