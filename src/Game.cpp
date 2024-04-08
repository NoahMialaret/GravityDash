#include "Game.h"

Game::Game(GameConfig& config)
  :
  config(config)
{
	for (int i = 0; i < config.numCharacters; i++)
	{
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(i);
		players.push_back(std::make_unique<PlayableCharacter>("assets/GravTestChar.png", control));
		players[i].get()->StartJump();
	}

  if (!entityTex.loadFromFile("assets/GravWEs.png")) 
  {
    std::cout << "\tEntity textures could not be loaded!\n";
  }

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

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

void Game::Update()
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

	for (auto& p : players)
	{
		p.get()->Update();
	}

  timerRect.setSize(sf::Vector2f(((float)timer - CUR_TIME) * 400.0f / 60000.0f, 32.0f));

	if ((players.size() == 1 && players[0].get()->GetCurState() == Character::State::dead) || (timer < CUR_TIME))
	{
		std::cout << "\nThe game is over! Well played!\n";
		std::cout << "\tYou scored: " << score.get()->GetAsString() << " points!\n\n";
		gameOver = true;
		return;
	}

	std::vector<Character*> pls;
	for (auto& p : players)
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

	for (auto& p : players)
	{
    CorrectCharacterPos(p.get());
	}

	if (players.size() == 1 && players[0].get()->IsLastStand())
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

  if (nextSpikeSpawnTimeMin < Utility::clock.getElapsedTime().asMilliseconds())
  {
    //std::cout << "Spawning obastacle! Last one spawned " << CUR_TIME - nextSpikeSpawnTimeMin + 1000 << " ms ago.\n";
    Entity* temp = new Saw(&entityTex, playableRegion);
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();

    entities.InsertData(temp, searchFrom);
    nextSpikeSpawnTimeMin = 500 + randomInt * 10 + CUR_TIME; // Spawns every 1000 +- 500 ms
  }

  if (randomInt > config.targetSpawnChance)
  {           
    Entity* temp = new MovingTarget(&entityTex, playableRegion);
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();
    entities.InsertData(temp, searchFrom);
  }  
}

void Game::Render(sf::RenderWindow* win) const
{
	world.get()->Render(win);

	score.get()->Render(win);

  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Render(win);
    node = node->GetNextNode();
  }

	for (auto& p : players)
	{
		p.get()->Render(win);
	}

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
	}
	else if (playerPos.x + posBuffer > playableRegion.left + playableRegion.width)
	{
		playerPos.x = playableRegion.left + playableRegion.width - posBuffer;
		player->SetPosition(playerPos);
	}

	if (player->GetCurState() == Character::State::airborne)
	{
		if (playerPos.y - posBuffer < playableRegion.top)
		{
			playerPos.y = playableRegion.top + posBuffer;
			player->SetPosition(playerPos);

			HandleLandingSequence(player);
		}
		else if (playerPos.y + posBuffer > playableRegion.top + playableRegion.height)
		{
			playerPos.y = playableRegion.top + playableRegion.height - posBuffer;
			player->SetPosition(playerPos);
			
			HandleLandingSequence(player);
		}
	}
}

void Game::HandleLandingSequence(Character* player)
{
	score.get()->AddPoints(player->Land());
}

bool Game::IsGameOver() const
{
	return gameOver;
}
