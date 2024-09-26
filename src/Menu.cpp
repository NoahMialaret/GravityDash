#include "Menu.h"

Menu::Menu()
{
  ChangeMenu(Menus::main);

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

    buttons.push_back({"score", Event(Event::Type::loadNewMenu, (int)Menus::score), 1});
    buttons.push_back({"opts.", Event(Event::Type::loadNewMenu, (int)Menus::options), 1});
    buttons.push_back({"play", Event(Event::Type::loadNewMenu, (int)Menus::play), 2});
    buttons.push_back({"medal", Event(Event::Type::loadNewMenu, (int)Menus::medal), 1});
    buttons.push_back({"exit", Event(Event::Type::programClose), 1});

    interface = std::make_unique<GridInterface>(buttons, Event(Event::Type::loadNewMenu, (int)Menus::title));
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
