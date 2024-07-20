#include "Game.h"

Game::Game(GameConfig& config)
  :
  config(config)
{
  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

	score = std::make_unique<GameScore>(sf::Vector2f(0.0f, worldRect.top - 6 * Utility::gameScale));

  int playerNum = 0;
  sf::Vector2f boostPos(worldRect.left + 5.0f * Utility::gameScale, - worldRect.top + Utility::gameScale);

  for (int i = 0; i < config.numPlayers; i++)
	{
    if (playerNum % 2 == 1)
      boostPos.x = -boostPos.x;
    else 
      boostPos.y = -boostPos.y;
  
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(playerNum);
		characters.push_back(std::make_unique<PlayableCharacter>(i, control, boostPos, score.get()));
		characters[playerNum].get()->Jump();
    playerNum++;
	}
  
  for (int i = 0; i < config.numComputers; i++)
  {    
    if (playerNum % 2 == 0)
      boostPos.x = -boostPos.x;
    else 
      boostPos.y = -boostPos.y;
    characters.push_back(std::make_unique<ComputerCharacter>(characters.size(), boostPos, score.get()));
    characters[playerNum].get()->Jump();
    playerNum++;
  }

  if (playerNum == 0)
  {    
    std::cout << "WARNING! Game was started with no characters!\n";
    characters.push_back(std::make_unique<ComputerCharacter>(0, boostPos, score.get()));
    characters[0].get()->Jump();
  }

  if (config.mode == Mode::title)
  {
    return;
  }


  spikeSpawnTimer = 1000;
  // BeginNextPhase = 5000 + CUR_TIME;

  timer = std::make_unique<GameTimer>(60000, sf::Vector2f(worldRect.left + worldRect.width + Utility::gameScale, 
    worldRect.top + worldRect.height - 2 * Utility::gameScale));
}

Game::~Game()
{
  std::cout << "Deleting the game and it's objects...\n";

  objects.DeleteAll();
}

void Game::Update() // Should have different update and render functions based on the mode
{
	for (auto& p : characters)
	{
		p.get()->Update();
	}

  if (config.mode != Mode::title)
    score.get()->Update();

	std::vector<Character*> pls;
	for (auto& p : characters)
	{
		pls.push_back(p.get());
	}

  auto node = objects.Start();
  while(node != nullptr)
  {
    node->GetData()->Update(pls);
    if (node->GetData()->EndOfLife())
    {
      node = objects.Delete(node);
      continue;
    }
    node = node->GetNextNode();
  }

	for (auto& p : characters)
	{
    CorrectCharacterPos(p.get());
	}

	if (gameOver)
	{
		return;
	}

  gameOver = true;
  for (auto& p : characters)
  {
    if (p.get()->GetCurState() != Character::State::dead)
    {
      gameOver = false;
      break;
    }
  }

  if (gameOver)
  {
		std::cout << "\nThe game is over! Well played!\n";
		std::cout << "\tYou scored: " << score.get()->GetAsString() << " points!\n\n";
    return;
  }

  if (Utility::CheckInitialPress(sf::Keyboard::P))
  {
    if (timer.get()->IsPaused())
    {
      std::cout << "Resuming Timer!\n";
      timer.get()->Unpause();
    }
    else
    {
      std::cout << "Pausing Timer!\n";
      timer.get()->Pause();
    }
  }

  switch (config.mode)
  {
    case Mode::time:
      if (!timeUp && timer.get()->Update())
      {
        std::cout << "Time is up, it's you're last jump!\n";
        timeUp = true;
        for (auto& p : characters)
        {
          p.get()->Kill();
        }

        auto node = objects.Start();
        while(node != nullptr)
        {
          node->GetData()->Freeze();
          node = node->GetNextNode();
        }
      }
      break;
    
    default:
      break;
  }

  if (timeUp)
  {
    return;
  }

	// if (characters.size() == 1 && characters[0].get()->IsLastStand())
	// {
  //   node = objects.Start();
  //   while(node != nullptr)
  //   {
  //     node->GetData()->Update(pls);
  //     node = node->GetNextNode();
  //   }
	// 	return;
	// }
    
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
	// 	if (Entity::objects == nullptr)
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
  spikeSpawnTimer -= Clock::Delta();
  spawnTimer -= Clock::Delta();

  while (spawnTimer <= 0)
  {
    spawnTimer += 16;
    std::uniform_int_distribution spawnChance(0, 99);
    int randomInt = spawnChance(Utility::rng);


    if (config.sawFrequency != 0 && spikeSpawnTimer <= 0)
    {
      //std::cout << "Spawning obastacle! Last one spawned " << CUR_TIME - nextSpikeSpawnTimeMin + 1000 << " ms ago.\n";
      GameObject* temp = new Saw(playableRegion, characters.size());
      auto searchFrom = objects.Start();
      if (*temp > 0)
        searchFrom = objects.End();

      objects.InsertData(temp, searchFrom);
      spikeSpawnTimer = 500 + randomInt * 10; // Spawns every 1000 +- 500 ms
    }

    if (randomInt > config.targetSpawnChance)
    {           
      GameObject* temp = new MovingTarget(playableRegion, characters.size());
      auto searchFrom = objects.Start();
      if (*temp > 0)
        searchFrom = objects.End();
      objects.InsertData(temp, searchFrom);
    }  
  }
}

void Game::Render(sf::RenderWindow* win) const
{
	world.get()->Render(win);

  Utility::RenderParticles(win);

  if (config.mode != Mode::title)
  {
    timer.get()->Render(win);
	  score.get()->Render(win);
  }

  auto node = objects.Start();
  while(node != nullptr)
  {
    node->GetData()->Render(win);
    node = node->GetNextNode();
  }

	for (auto& p : characters)
	{
		p.get()->Render(win);
	}
}

void Game::CorrectCharacterPos(Character* player)
{
	sf::Vector2f playerPos = player->GetPosition();
	float posBuffer = 0.5f * SCALED_DIM;

	sf::FloatRect playableRegion = (sf::FloatRect)world.get()->GetRegion();

	if (playerPos.x <= playableRegion.left + posBuffer)
	{
    float offset = playableRegion.left + posBuffer - playerPos.x;
    player->WallCollision(offset);
	}
	else if (playerPos.x + posBuffer > playableRegion.left + playableRegion.width)
	{
    float offset = playableRegion.left + playableRegion.width - posBuffer - playerPos.x;
    player->WallCollision(offset);
	}

  if (playerPos.y - posBuffer < playableRegion.top)
  {
    float offset = playableRegion.top + posBuffer - playerPos.y;
    player->FloorCollision(offset);
  }
  else if (playerPos.y + posBuffer > playableRegion.top + playableRegion.height)
  {
    float offset = playableRegion.top + playableRegion.height - posBuffer - playerPos.y;
    player->FloorCollision(offset);
	}
}


bool Game::IsGameOver() const
{
	return gameOver;
}
