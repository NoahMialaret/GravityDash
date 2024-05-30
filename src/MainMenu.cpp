#include "MainMenu.h"

MainMenu::MainMenu(bool showIntro)
{
  for (int i = 0; i < 4; i++)
  {
    cpus.push_back(std::make_unique<ComputerCharacter>("assets/charBW.png", i));
    cpus[i].get()->StartJump();
  }

  if (!entityTex.loadFromFile("assets/GravWEs.png")) 
  {
    std::cout << "\tEntity textures could not be loaded!\n";
  }

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
  sf::IntRect worldRect(-worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  SubMenu::SetBottomLeftTitlePosition({(float)worldRect.left, (float)worldRect.top - 2.0f * Utility::gameScale});
  curSubMenu = std::make_unique<Title>();
}

void MainMenu::Update()
{
  for (auto &cpu : cpus)
  {
    cpu.get()->Update();
  }

  std::vector<Character*> pls;
  for (auto& p : cpus)
  {
    pls.push_back(p.get());
  }
  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Update(pls);
    if (node->GetData()->EndOfLife())
    {
      node = entities.Delete(node);
      continue;
    }
    node = node->GetNextNode();
  }

  for (auto& cpu : cpus)
  {
    CorrectCharacterPos(cpu.get());
  }

  std::uniform_int_distribution spawnChance(0, 99);
  int randomInt = spawnChance(Utility::rng);

  if (randomInt > 90)
  {
    sf::IntRect playableRegion = world.get()->GetRegion();
    Entity* temp = new MovingTarget(&entityTex, playableRegion, cpus.size());
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();
    entities.InsertData(temp, searchFrom);
  }

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
  world.get()->Render(win);

  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Render(win);
    node = node->GetNextNode();
  }

  Particle::RenderParticles(win);

  for (auto& cpu : cpus)
  {
    cpu.get()->Render(win);
  }

  curSubMenu.get()->Render(win);
}

void MainMenu::CorrectCharacterPos(Character* cpu)
{
  sf::Vector2f CharacterPos = cpu->GetPosition();
  float posBuffer = 0.5f * SCALED_DIM;

  sf::IntRect playableRegion = world.get()->GetRegion();

  if (CharacterPos.x - posBuffer < playableRegion.left)
  {
    CharacterPos.x = playableRegion.left + posBuffer;
    cpu->SetPosition(CharacterPos);
  }
  else if (CharacterPos.x + posBuffer > playableRegion.left + playableRegion.width)
  {
    CharacterPos.x = playableRegion.left + playableRegion.width - posBuffer;
    cpu->SetPosition(CharacterPos);
  }

  if (cpu->GetCurState() == Character::State::airborne)
  {
    if (CharacterPos.y - posBuffer < playableRegion.top)
    {
      CharacterPos.y = playableRegion.top + posBuffer;
      cpu->SetPosition(CharacterPos);

      cpu->Land();
    }
    else if (CharacterPos.y + posBuffer > playableRegion.top + playableRegion.height)
    {
      CharacterPos.y = playableRegion.top + playableRegion.height - posBuffer;
      cpu->SetPosition(CharacterPos);

      cpu->Land();
    }
  }
}

GameConfig MainMenu::GetGameConfig() const
{
  return curSubMenu.get()->GetGameConfig();
}
