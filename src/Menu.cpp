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


  // if (Utility::CheckInitialPress(sf::Keyboard::Space))
  // {
  //   curLayout.get()->DoButtonAction(interface.get(), curLayout.get());
  // }
  // else if (Utility::CheckInitialPress(sf::Keyboard::Escape))
  // {
  //   curLayout.get()->Return(interface.get(), curLayout.get());
  // }
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
    event.gameConfig = Event::GameConfig{Event::GameConfig::Type::min, 1, 0, 90, 2};
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
  case Event::MenuType::pause:
  {
    break;
  }
  
  default:
    break;
  }
}

// MainLayout::MainLayout(MenuInterface* interface, Layout* layout)
// {
//   menu->curLayout;
// }

// void MainLayout::DoButtonAction(MenuInterface* interface, Layout* layout)
// {
//   int clickedButton = interface.
// }

// void MainLayout::Return(MenuInterface* interface, Layout* layout)
// {
// }
