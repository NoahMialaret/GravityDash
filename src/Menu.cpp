#include "Menu.h"

Menu::Menu(Event::MenuType startMenu)
{
  ChangeMenu(startMenu);

  Event::GameConfig config;
  config.numPlayers = 0;
  config.numComputers = 4;
  config.sawFrequency = 0;
  config.targetSpawnChance = 90;

  backgroundGame = std::make_unique<Game>(config);
}

void Menu::Update()
{
  if (showGame)
    backgroundGame.get()->Update();

  interface.get()->Update();
}

void Menu::Render(sf::RenderWindow* win) const
{
  if (showGame)
    backgroundGame.get()->Render(win);
    
  interface.get()->Render(win);
}

void Menu::ChangeMenu(Event::MenuType menuType)
{
  Event event;
  switch (menuType)
  {
  case Event::MenuType::main:
  {
    showGame = true;

    std::vector<ButtonConfig> buttons;

    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::score;
    buttons.push_back({"score", event, MEDIUM});
    event.menuType = Event::MenuType::options;
    buttons.push_back({"opts.", event, MEDIUM});
    event.menuType = Event::MenuType::play;
    buttons.push_back({"play", event, LARGE});
    event.menuType = Event::MenuType::medal;
    buttons.push_back({"medal", event, MEDIUM});
    event.type = Event::Type::programClose;
    buttons.push_back({"exit", event, MEDIUM});

    // event.type = Event::Type::loadNewMenu;
    // event.menuType = Event::MenuType::title;
    interface = std::make_unique<GridInterface>(2, buttons, event);
    break;
  }
  case Event::MenuType::play:
  {
    showGame = true;

    std::vector<ButtonConfig> buttons;

    event.type = Event::Type::loadNewGame;
    event.gameConfig = Event::GameConfig{Event::GameConfig::Type::min, 1, 0, 90, 23};
    buttons.push_back({"1min", event, LARGE});
    event.gameConfig.type = Event::GameConfig::Type::rush;
    buttons.push_back({"rush", event, LARGE});
    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::multi;
    buttons.push_back({"multi", event, MEDIUM});
    event.menuType = Event::MenuType::custom;
    buttons.push_back({"wild", event, MEDIUM});

    event.menuType = Event::MenuType::main;
    interface = std::make_unique<GridInterface>(2, buttons, event);
    break;
  }
  case Event::MenuType::multi:
  {
    showGame = true;
    // color = blue;

    std::vector<ButtonConfig> buttons;

    event.type = Event::Type::loadNewGame;
    event.gameConfig = Event::GameConfig{Event::GameConfig::Type::min, 1, 1, 90, 2};
    buttons.push_back({"ally", event, LARGE});
    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::addPlayers;
    buttons.push_back({"add", event, MEDIUM});
    event.menuType = Event::MenuType::custom;
    buttons.push_back({"wild", event, MEDIUM});
    event.type = Event::Type::loadNewGame;
    event.gameConfig = Event::GameConfig{Event::GameConfig::Type::min, 1, 1, 90, 2};
    buttons.push_back({"vs.", event, LARGE});

    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::play;
    interface = std::make_unique<GridInterface>(2, buttons, event);
    break;    
  }
  case Event::MenuType::pause:
  {
    showGame = false;

    std::vector<ButtonConfig> buttons;

    event.type = Event::Type::resumePlay;
    buttons.push_back({"resume", event, SMALL});

    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::options;
    buttons.push_back({"opts.", event, SMALL});

    event.type = Event::Type::exitGame;
    buttons.push_back({"quit", event, SMALL});

    event.type = Event::Type::resumePlay;
    interface = std::make_unique<ListInterface>(buttons, event);

    break;
  }
  case Event::MenuType::options:
  {
    showGame = true;

    std::vector<OptionConfig> options;

    event.type = Event::Type::exitGame;

    OptionConfig option = {"scale", event, OptionConfig::Type::range};
    option.range = {2, 0, 10};
    options.push_back(option);

    option = {"fullscreen", event, OptionConfig::Type::toggle};
    option.toggle = {true};
    options.push_back(option);

    option = {"colour", event, OptionConfig::Type::selection};
    std::vector<std::string> colours{"green", "orange", "purple"};
    option.selection = {0, &colours};
    options.push_back(option);

    option = {"mode", event, OptionConfig::Type::selection};
    std::vector<std::string> modes{"1min", "rush", "wild"};
    option.selection = {0, &modes};
    options.push_back(option);

    event.type = Event::Type::loadNewMenu;
    event.menuType = Event::MenuType::main;
    interface = std::make_unique<OptionsInterface>(options, event);
  }
  
  default:
    break;
  }
}