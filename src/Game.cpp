#include "Game.h"

Game::Game(GameConfig& config)
  :
  config(config)
{
  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  PlayerBoost boost = PlayerBoost(5, sf::Vector2f(worldRect.left - Utility::gameScale, worldRect.top + 3.0f * Utility::gameScale));
  for (int i = 0; i < config.numPlayers; i++)
	{
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(i);
		characters.push_back(std::make_unique<PlayableCharacter>(i, control, boost));
		characters[i].get()->StartJump();
	}
  
  for (int i = 0; i < config.numComputers; i++)
  {    
    characters.push_back(std::make_unique<ComputerCharacter>(characters.size(), boost));
    characters[characters.size() - 1].get()->StartJump();
  }

  if (characters.size() <= 0)
  {    
    std::cout << "WARNING! Game was started with no characters!\n";
    characters.push_back(std::make_unique<ComputerCharacter>(0, boost));
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
      Entity* temp = new Saw(playableRegion, characters.size());
      auto searchFrom = entities.Start();
      if (*temp > 0)
        searchFrom = entities.End();

      entities.InsertData(temp, searchFrom);
      spikeSpawnTimer = 500 + randomInt * 10; // Spawns every 1000 +- 500 ms
    }

    if (randomInt > config.targetSpawnChance)
    {           
      Entity* temp = new MovingTarget(playableRegion, characters.size());
      auto searchFrom = entities.Start();
      if (*temp > 0)
        searchFrom = entities.End();
      entities.InsertData(temp, searchFrom);
    }  
  }
}

void Game::Render(sf::RenderWindow* win) const
{
	world.get()->Render(win);

  Utility::RenderParticles(win);

  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Render(win);
    node = node->GetNextNode();
  }

  for (auto& point : totalPoints)
  {
    point.Render(win);
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

  timer.get()->Render(win);
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

  totalPoints.emplace_back(player->GetPoints());

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
