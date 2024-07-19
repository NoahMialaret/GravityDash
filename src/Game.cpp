#include "Game.h"

Game::Game(GameConfig& config)
  :
  config(config)
{
  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  sf::Vector2f boostPos(worldRect.left + 5.0f * Utility::gameScale, worldRect.top - Utility::gameScale);
  for (int i = 0; i < config.numPlayers; i++)
	{
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(i);
		characters.push_back(std::make_unique<PlayableCharacter>(i, control, boostPos));
		characters[i].get()->StartJump();
	}
  
  for (int i = 0; i < config.numComputers; i++)
  {    
    characters.push_back(std::make_unique<ComputerCharacter>(characters.size(), boostPos));
    characters[characters.size() - 1].get()->StartJump();
  }

  if (characters.size() <= 0)
  {    
    std::cout << "WARNING! Game was started with no characters!\n";
    characters.push_back(std::make_unique<ComputerCharacter>(0, boostPos));
    characters[0].get()->StartJump();
  }

  if (config.mode == Mode::title)
  {
    return;
  }

	score = std::make_unique<GameScore>(sf::Vector2f(0.0f, worldRect.top - 6 * Utility::gameScale));

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

  if (Utility::CheckInitialPress(sf::Keyboard::H))
  {
    auto node = objects.Start();
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

	for (auto& p : characters)
	{
		p.get()->Update();
	}

  for (auto& point : totalPoints)
  {
    point.Update();
  }
  while (totalPoints.front().HasFinished())
  {
    totalPoints.pop_front();
  }

	if ((characters.size() == 1 && characters[0].get()->GetCurState() == Character::State::dead) 
        || (config.mode == Mode::time && timer.get()->Update()))
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

	if (characters.size() == 1 && characters[0].get()->IsLastStand())
	{
    node = objects.Start();
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

  for (auto& point : totalPoints)
  {
    point.Render(win);
  }

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
    if(player->FloorCollision(offset))
    {
      HandleLandingSequence(player);
    }
  }
  else if (playerPos.y + posBuffer > playableRegion.top + playableRegion.height)
  {
    float offset = playableRegion.top + playableRegion.height - posBuffer - playerPos.y;
    if(player->FloorCollision(offset))
    {
      HandleLandingSequence(player);
    }
	}
}

void Game::HandleLandingSequence(Character* player)
{
  auto p = player->GetPoints();
  if (p.empty())
  {
    std::cout << "Nothing!\n";
    player->Land();
    return;
  }
  totalPoints.emplace_back(p);

  player->Land();
      
  if (config.mode != Mode::title)
  {
    score.get()->AddPoints(totalPoints.back().GetAsInt());
  }
}

bool Game::IsGameOver() const
{
	return gameOver;
}
