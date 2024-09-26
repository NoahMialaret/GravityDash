#include "Menu.h"

Menu::Menu(Menus startMenu)
{
  ChangeMenu(startMenu);

  GameConfig config;
  config.numPlayers = 0;
  config.numComputers = 4;
  config.sawFrequency = 0;

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

void Menu::ChangeMenu(Menus menuType)
{
  switch (menuType)
  {
  case Menus::main:
  {
    showGame = true;

    std::vector<ButtonConfig> buttons;

    buttons.push_back({"score", Event(Event::Type::loadNewMenu, (int)Menus::score), MEDIUM});
    buttons.push_back({"opts.", Event(Event::Type::loadNewMenu, (int)Menus::options), MEDIUM});
    buttons.push_back({"play", Event(Event::Type::loadNewMenu, (int)Menus::play), LARGE});
    buttons.push_back({"medal", Event(Event::Type::loadNewMenu, (int)Menus::medal), MEDIUM});
    buttons.push_back({"exit", Event(Event::Type::programClose), MEDIUM});

    interface = std::make_unique<GridInterface>(2, buttons, Event(Event::Type::programClose, (int)Menus::title));
    break;
  }
  case Menus::play:
  {
    showGame = true;

    std::vector<ButtonConfig> buttons;

    buttons.push_back({"1min", Event(Event::Type::loadNewGame), LARGE});
    buttons.push_back({"rush", Event(Event::Type::loadNewGame), LARGE});
    buttons.push_back({"multi", Event(Event::Type::loadNewMenu, (int)Menus::multi), MEDIUM});
    buttons.push_back({"wild", Event(Event::Type::loadNewMenu, (int)Menus::custom), MEDIUM});

    interface = std::make_unique<GridInterface>(2, buttons, Event(Event::Type::loadNewMenu, (int)Menus::main));
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
