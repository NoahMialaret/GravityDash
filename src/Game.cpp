#include "Game.h"

Game::Game(GameConfig& config)
  :
  config(config)
{
	for (int i = 0; i < config.numPlayers; i++)
	{
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(i);
		characters.push_back(std::make_unique<PlayableCharacter>("assets/charBW.png", i, control));
		characters[i].get()->StartJump();
	}
  
  for (int i = 0; i < config.numComputers; i++)
  {
    characters.push_back(std::make_unique<ComputerCharacter>("assets/charBW.png", characters.size()));
    characters[characters.size() - 1].get()->StartJump();
  }

  if (characters.size() <= 0)
  {    
    std::cout << "WARNING! Game was started with no characters!\n";
    characters.push_back(std::make_unique<ComputerCharacter>("assets/charBW.png", 0));
    characters[0].get()->StartJump();
  }

  if (!entityTex.loadFromFile("assets/GravWEs.png")) 
  {
    std::cout << "\tEntity textures could not be loaded!\n";
  }

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  if (config.mode == Mode::title)
  {
    return;
  }

	sf::Vector2f topRightPos(worldRect.left + worldRect.width, worldRect.top - (SCORE_TEX_HEIGHT + 1) * Utility::gameScale);
	score = std::make_unique<Score>("assets/BigNums.png", topRightPos);

  nextSpikeSpawnTimeMin = 1000 + CUR_TIME;
  // BeginNextPhase = 5000 + CUR_TIME;

  timer = CUR_TIME + 60000;

  timerRect.setFillColor(sf::Color(173, 103, 78));
  timerRect.setPosition((- (sf::Vector2f)worldSize / 2.0f) - sf::Vector2f(0.0f, 32.0f));
}

Game::~Game()
{
  std::cout << "Deleting the game and it's entities...\n";

  entities.DeleteAll();
}

void Game::Update() // Should have different update and render functions based on the mode
{

  if (Utility::CheckInitialPress(sf::Keyboard::H))
  {
    auto node = entities.Start();
    while(node != nullptr)
    {
      std::cout << node->GetData()->GetPosition().y << '\n';
      node = node->GetNextNode();
    }
  }

	if (gameOver)
	{
		return;
	}

	for (auto& p : characters)
	{
		p.get()->Update();
	}

  if (config.mode == Mode::time)
  {
    timerRect.setSize(sf::Vector2f(((float)timer - CUR_TIME) * 400.0f / 60000.0f, 32.0f));
  }

	if ((characters.size() == 1 && characters[0].get()->GetCurState() == Character::State::dead) || (timer < CUR_TIME && config.mode == Mode::time))
	{
		std::cout << "\nThe game is over! Well played!\n";
		std::cout << "\tYou scored: " << score.get()->GetAsString() << " points!\n\n";
		gameOver = true;
		return;
	}

	std::vector<Character*> pls;
	for (auto& p : characters)
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

	for (auto& p : characters)
	{
    CorrectCharacterPos(p.get());
	}

	if (characters.size() == 1 && characters[0].get()->IsLastStand())
	{
    node = entities.Start();
    while(node != nullptr)
    {
      node->GetData()->Update(pls);
      node = node->GetNextNode();
    }
		return;
	}
    
    // if (BeginNextPhase < Utility::clock.getElapsedTime().asMilliseconds() && curPhase != Phase::transition)
    // {
    //     if (curPhase == Phase::standard)
    //     {
    //         nextPhase = Phase::skinny;
	// 		world.get()->SetTargetLeft(32);
    //     }
    //     if (curPhase == Phase::skinny)
    //     {
    //         nextPhase = Phase::standard;
	// 		world.get()->SetTargetLeft(400);
    //     }
    //     std::cout << "Beginning world transition into phase: " << (int)nextPhase << '\n';
    //     curPhase = Phase::transition;
    // }


    // if (curPhase == Phase::transition)
    // {
	// 	if (Entity::entities == nullptr)
	// 	{
	// 		world.get()->Update();

	// 		if (world.get()->FinishedTransitioning())
	// 		{
	// 			BeginNextPhase = 5000 + CUR_TIME;
	// 			curPhase = nextPhase;
	// 		}
	// 	}

	// 	return;
    // }

	sf::IntRect playableRegion = world.get()->GetRegion();
  std::uniform_int_distribution spawnChance(0, 99);
  int randomInt = spawnChance(Utility::rng);

  if (config.sawFrequency != 0 && nextSpikeSpawnTimeMin < CUR_TIME)
  {
    //std::cout << "Spawning obastacle! Last one spawned " << CUR_TIME - nextSpikeSpawnTimeMin + 1000 << " ms ago.\n";
    Entity* temp = new Saw(&entityTex, playableRegion, characters.size());
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();

    entities.InsertData(temp, searchFrom);
    nextSpikeSpawnTimeMin = 500 + randomInt * 10 + CUR_TIME; // Spawns every 1000 +- 500 ms
  }

  if (randomInt > config.targetSpawnChance)
  {           
    Entity* temp = new MovingTarget(&entityTex, playableRegion, characters.size());
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();
    entities.InsertData(temp, searchFrom);
  }  
}

void Game::Render(sf::RenderWindow* win) const
{
	world.get()->Render(win);

  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Render(win);
    node = node->GetNextNode();
  }

	for (auto& p : characters)
	{
		p.get()->Render(win);
	}

  if (config.mode == Mode::title)
  {
    return;
  }

	score.get()->Render(win);

  win->draw(timerRect);
}

void Game::CorrectCharacterPos(Character* player)
{
  // Clean up when adding rebound feature

	sf::Vector2f playerPos = player->GetPosition();
	float posBuffer = 0.5f * SCALED_DIM;

	sf::IntRect playableRegion = world.get()->GetRegion();

	if (playerPos.x - posBuffer < playableRegion.left)
	{
		playerPos.x = playableRegion.left + posBuffer;
		player->SetPosition(playerPos);
    player->SetXVelocity(0.0f);
	}
	else if (playerPos.x + posBuffer > playableRegion.left + playableRegion.width)
	{
		playerPos.x = playableRegion.left + playableRegion.width - posBuffer;
		player->SetPosition(playerPos);
    player->SetXVelocity(0.0f);
	}

  if (playerPos.y - posBuffer < playableRegion.top)
  {
    playerPos.y = playableRegion.top + posBuffer;
    player->SetPosition(playerPos);
    player->SetYVelocity(0.0f);

    HandleLandingSequence(player);
  }
  else if (playerPos.y + posBuffer > playableRegion.top + playableRegion.height)
  {
    playerPos.y = playableRegion.top + playableRegion.height - posBuffer;
    player->SetPosition(playerPos);
    player->SetYVelocity(0.0f);
    
    HandleLandingSequence(player);
	}
}

void Game::HandleLandingSequence(Character* player)
{
  if (player->GetCurState() != Character::State::airborne)
  {
    return;
  }

  int points = player->Land();
      
  if (config.mode != Mode::title)
  {
    score.get()->AddPoints(points);
  }
}

bool Game::IsGameOver() const
{
	return gameOver;
}
