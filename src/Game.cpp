#include "Game.hpp"

Game::Game(GameConfig& config)
  :
  config(config)
{
  entityTex.LoadFromFile("assets/GravWEs.png");
  charTex.LoadFromFile("assets/GravTestChar.png");

	for (int i = 0; i < config.numCharacters; i++)
	{
    std::unique_ptr<Controls> control = std::make_unique<Keyboard>(i);
		players.push_back(std::make_unique<PlayableCharacter>(&charTex, control));
		players[i].get()->StartJump();
	}

  iVec2 worldSize = iVec2(16, 8) * int(SCALED_DIM);
	IRect worldRect(worldSize / -2, worldSize);
  world = std::make_unique<World>(worldRect);

	fVec2 topRightPos(worldRect.left + worldRect.width, worldRect.top - (SCORE_TEX_HEIGHT + 1) * Utility::gameScale);
	score = std::make_unique<Score>("assets/BigNums.png", topRightPos);

  nextSpikeSpawnTimeMin = 1.0 + CUR_TIME;
  // BeginNextPhase = 5000 + CUR_TIME;

  timer = CUR_TIME + 60.0;

  // timerRect.setFillColor(sf::Color(173, 103, 78));
  // timerRect.setPosition((- (fVec2)worldSize / 2.0f) - fVec2(0.0f, 32.0f));
}

Game::~Game()
{
  std::cout << "Deleting the game and it's entities...\n";

  entities.DeleteAll();
}

void Game::Update()
{

  if (Utility::CheckInitialPress(GLFW_KEY_H))
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

  // timerRect.setSize(fVec2(((float)timer - CUR_TIME) * 400.0f / 60000.0f, 32.0f));

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

	IRect playableRegion = world.get()->GetRegion();
  std::uniform_int_distribution spawnChance(0, 99);
  int randomInt = spawnChance(Utility::rng);

  if (nextSpikeSpawnTimeMin < CUR_TIME)
  {
    //std::cout << "Spawning obastacle! Last one spawned " << CUR_TIME - nextSpikeSpawnTimeMin + 1000 << " ms ago.\n";
    Entity* temp = new Saw(&entityTex, playableRegion);
    auto searchFrom = entities.Start();
    if (*temp > 0)
      searchFrom = entities.End();

    entities.InsertData(temp, searchFrom);
    nextSpikeSpawnTimeMin = 0.500 + randomInt * 10 + CUR_TIME; // Spawns every 1000 +- 500 ms
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

void Game::Render() const
{
	world.get()->Render();

	score.get()->Render();

  auto node = entities.Start();
  while(node != nullptr)
  {
    node->GetData()->Render();
    node = node->GetNextNode();
  }

	for (auto& p : players)
	{
		p.get()->Render();
	}

  // win->draw(timerRect);
}

void Game::CorrectCharacterPos(Character* player)
{
  // Clean up when adding rebound feature

	fVec2 playerPos = player->GetPosition();
	float posBuffer = 0.5f * SCALED_DIM;

	IRect playableRegion = world.get()->GetRegion();

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
