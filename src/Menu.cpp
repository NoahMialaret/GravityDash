#include "Menu.h"

Menu::Menu()
{
  ChangeMenu(Menus::main);
}

void Menu::Update()
{
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
  interface.get()->Render(win);
}

void Menu::ChangeMenu(Menus menuType)
{
  switch (menuType)
  {
  case Menus::main:
  {
    std::vector<std::unique_ptr<Button>> buttons;
    buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{-300, -200}, "score", Event(Event::Type::loadNewMenu, (int)Menus::score)));
    buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{-300, 200}, "opts.", Event(Event::Type::loadNewMenu, (int)Menus::options)));
    buttons.push_back(std::make_unique<LargeButton>(sf::Vector2f{0, 0}, "play", Event(Event::Type::loadNewMenu, (int)Menus::play)));
    buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{300, -200}, "medal", Event(Event::Type::loadNewMenu, (int)Menus::medal)));
    buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{300, 200}, "exit", Event(Event::Type::programClose)));

    std::vector<int> pos = {0, 1, 2, 2, 3, 4};

    interface = std::make_unique<GridInterface>(buttons, pos, Event(Event::Type::loadNewMenu, (int)Menus::title));
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
