#include "MainMenu.h"

MainMenu::MainMenu(bool showIntro)
{
  GameConfig config;
  config.numPlayers = 0;
  config.numComputers = 4;
  config.sawFrequency = 0;

  game = std::make_unique<Game>(config);

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
  sf::IntRect worldRect(-worldSize / 2, worldSize);

  SubMenu::SetBottomLeftTitlePosition({(float)worldRect.left, (float)worldRect.top - 2.0f * Utility::gameScale});
  curSubMenu = std::make_unique<Title>();
}

void MainMenu::Update()
{
  game.get()->Update();

  curSubMenu.get()->Update();

  if (Utility::CheckInitialPress(sf::Keyboard::Space))
  {
    std::unique_ptr<SubMenu> temp = curSubMenu.get()->ClickButton();

    if (temp != nullptr)
    {
      curSubMenu = std::move(temp);
    }
  }
}

void MainMenu::Return()
{
  std::unique_ptr<SubMenu> temp = curSubMenu.get()->Return();

  if (temp != nullptr)
  {
    curSubMenu = std::move(temp);
  }
}

void MainMenu::Render(sf::RenderWindow* win) const
{
  game.get()->Render(win);

  curSubMenu.get()->Render(win);
}

GameConfig MainMenu::GetGameConfig() const
{
  return curSubMenu.get()->GetGameConfig();
}
