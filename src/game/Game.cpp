#include "Game.h"

Game::Game(Event::GameConfig& config)
  :
  config(config)
{
  GameStats::localStats = GameStats::Local();

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
	sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  int playerNum = 0;

  for (int i = 0; i < config.numPlayers && playerNum < 4; i++)
	{  
		characters.push_back(std::make_unique<PlayableCharacter>(playerNum, ProgramSettings::GetControls(playerNum)));
    playerNum++;
	}
  
  for (int i = 0; i < config.numComputers && playerNum < 4; i++)
  {    
    characters.push_back(std::make_unique<ComputerCharacter>(playerNum));
    playerNum++;
  }

  if (playerNum == 0)
  {    
    std::cout << "WARNING! Game was started with no characters!\n";
    characters.push_back(std::make_unique<ComputerCharacter>(0));
    characters[0].get()->Jump();
  }
}

Game::~Game()
{
  std::cout << "Deleting the game and it's objects...\n";

  Utility::scoreMultiplier = 1.0f;

  objects.clear();
}

void Game::Update()
{
	for (auto& p : characters)
	{
		p.get()->Update();
    CorrectCharacterPos(p.get());
	}

  for (auto prev = objects.before_begin(), cur = objects.begin(); cur != objects.end();)
  {
    while (cur != objects.end() && cur->get()->IsTombstoned())
    {
      cur = objects.erase_after(prev);
    }

    if (cur == objects.end())
      break;

    cur->get()->Update();

    for (auto& character : characters)
      cur->get()->HandleCollision(character.get());

    cur->get()->ProcessTag();

    prev++;
    cur++;
  }

	if (gameOver)
		return;

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
    objects.clear();
    Event event;
    event.type = Event::Type::gameDone;
    Event::events.push_back(event);
    return;
  }

  if (canSpawnObjects)
    SpawnObjects();
}

void Game::SpawnObjects()
{
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
      objects.push_front(std::make_unique<Saw>(playableRegion));
      spikeSpawnTimer = 500 + randomInt * 10; // Spawns every 1000 +- 500 ms
    }

    if (randomInt > config.targetSpawnChance)
    {           
      objects.push_front(std::make_unique<MovingTarget>(playableRegion));
    }  
  }
}

void Game::Render(sf::RenderWindow* win) const
{
	world.get()->Render(win);

  for (auto& object : objects)
    object.get()->Render(win);

	for (auto& character : characters)
		character.get()->Render(win);
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

Event::GameConfig Game::GetConfig() const
{
  return config;
}

// ============
// --- Min ---
// ============

Min::Min(Event::GameConfig& config)
  :
  Game(config)
{
  sf::IntRect worldRect = world.get()->GetRegion();
	score = std::make_unique<GameScore>(sf::Vector2f(0.0f, worldRect.top - 6 * ProgramSettings::gameScale));
  spikeSpawnTimer = 1000;

  timer = std::make_unique<GameTimer>(config.maxTime * 1000, sf::Vector2f(worldRect.left + worldRect.width + ProgramSettings::gameScale, 
    worldRect.top + worldRect.height - 2 * ProgramSettings::gameScale));

  sf::Vector2f boostPos(worldRect.left + 5.0f * ProgramSettings::gameScale, - worldRect.top + ProgramSettings::gameScale);

  for (int i = 0; i < characters.size(); i++)
  {
    if (i % 2 == 1)
      boostPos.x = -boostPos.x;
    else 
      boostPos.y = -boostPos.y;

    characters[i].get()->EnableBoost(boostPos);
    characters[i].get()->LinkScore(score.get());
  }
}

void Min::Update()
{

  score.get()->Update();

  if (gameOver)
  {
    Game::Update();
    return;
  }
  
  if (ProgramSettings::GetControls()->IsBindingOnInitialClick(Controls::Binding::escape))
  {
    Event event;
    event.type = Event::Type::pause;
    Event::events.push_back(event);
    return;
  }
  
  UpdateTimer();

  Game::Update();

  if (gameOver)
  {
		std::cout << "\nThe game is over! Well played!\n";
		std::cout << "\tYou scored: " << score.get()->GetAsString() << " points!\n\n";
  }
}

void Min::Render(sf::RenderWindow *win) const
{
  Game::Render(win);

  score.get()->Render(win);
  timer.get()->Render(win);
}

void Min::UpdateTimer()
{
  if (!timeUp && timer.get()->Update())
  {
    std::cout << "Time is up, it's you're last jump!\n";
    timeUp = true;
    canSpawnObjects = false;
    for (auto& p : characters)
    {
      p.get()->Kill();
    }

    for (auto& object : objects)
      object.get()->Deactivate();
  }
}

// =============
// --- Rush ---
// =============

Rush::Rush(Event::GameConfig& config)
  :
  Min(config)
{
  GameStats::localStats.timeBoosts = 0;
  
  arrow = Entity("arrow", nullptr, (sf::Vector2i)Textures::textures.at("arrow").getSize());
  arrowBottom = timer.get()->GetPosition() + ProgramSettings::gameScale * sf::Vector2f(8.0f, -0.5f);
  arrow.QueueMotion(Curve::linear, 0, ZERO_VECTOR, arrowBottom);
  arrow.FlipX();

  sf::IntRect worldRect = world.get()->GetRegion();

  Utility::InitText(multiplierText, Textures::large, "*1.0", {0.0f, worldRect.height / 2.0f - SCALED_DIM + 2 * ProgramSettings::gameScale}, {0.5f, 0.0f}, {255, 229, 181});
  multiplierText.setOutlineColor({173, 103, 78});
  multiplierText.setOutlineThickness(ProgramSettings::gameScale);
}

void Rush::Update()
{  
  arrow.Update();
  if (gameOver)
  {
    Min::Update();
    return;
  }

  Min::Update();

  for (auto& p : characters)
  {
    int boost = p.get()->GetTimeBoost();
    if (boost)
    {
      storedTime += boost * 5;
      if (storedTime >= config.maxTime)
      {
        storedTime = config.maxTime;
      }
      arrow.ClearHandlers();
      arrow.QueueMotion(Curve::easeIn, 1000, arrow.GetPosition(), arrowBottom - (float)storedTime * sf::Vector2f(0.0f, ProgramSettings::gameScale) * 60.0f / (float)config.maxTime);
    }
  }

  // Increase target spawn chance on timer refill, but decrease time pickups?
}

void Rush::Render(sf::RenderWindow *win) const
{
  Min::Render(win);

  arrow.Render(win);

  win->draw(multiplierText);
}

void Rush::UpdateTimer()
{
  if (!timeUp && timer.get()->Update())
  {
    if (storedTime != 0)
    {
      timer.get()->AddTime(storedTime * 1000);
      storedTime = 0;
      arrow.ClearHandlers();
      arrow.QueueMotion(Curve::easeIn, 1000, arrow.GetPosition(), arrowBottom);
      Utility::scoreMultiplier += 0.1f;
      phase++;
      timeBonusCooldown = phase;
      if (Utility::scoreMultiplier >= 2.0f)
      {
        Utility::scoreMultiplier = 2.0f;
      }
      std::string multText = "*" + std::to_string((int)(10 * Utility::scoreMultiplier));
      multText.push_back(multText[2]);
      multText[2] = '.';
      Utility::UpdateText(multiplierText, multText, {0.5f, 0.0f});
      return;
    }
    std::cout << "Time is up, it's you're last jump!\n";
    timeUp = true;
    canSpawnObjects = false;
    for (auto& p : characters)
    {
      p.get()->Kill();
    }

    for (auto& object : objects)
      object.get()->Deactivate();
  }
}

void Rush::SpawnObjects()
{
  Game::SpawnObjects();

  timeBonusTimer -= Clock::Delta();

	sf::IntRect playableRegion = world.get()->GetRegion();

  while (timeBonusTimer <= 0)
  {
    timeBonusTimer += 16;
    std::uniform_int_distribution spawnChance(0, 99);
    int randomInt = spawnChance(Utility::rng);

    if (randomInt > 98)
    {
      if (timeBonusCooldown > 0)
      {
        timeBonusCooldown--;
        return;
      }     
      objects.push_front(std::make_unique<TimeBonus>(playableRegion));
      timeBonusCooldown = phase;
    }  
  } 
}

// ============
// --- Wild ---
// ============

Wild::Wild(Event::GameConfig& config)
  :
  Game(config)
{
}

void Wild::Update()
{
  Game::Update();
    
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
}

void Wild::Render(sf::RenderWindow *win) const
{
  Game::Render(win);
}
