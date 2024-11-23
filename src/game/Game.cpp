#include "Game.h"

Game::Game(int numHumans, int numComputers)
{
  world = std::make_unique<World>();

  int numPlayers = std::min(4, numHumans + numComputers);
  for (int i = 0; i < numPlayers; i++)
  {
    if (i < numHumans)
		  characters.push_back(std::make_unique<PlayableCharacter>(i, ProgramSettings::GetControls(i)));
    else
      characters.push_back(std::make_unique<ComputerCharacter>(i));
  }
}

void Game::Update()
{
  world.get()->Update();
  
	for (auto& character : characters)
		character.get()->Update();

  for (auto prev = objects.before_begin(), cur = objects.begin(); cur != objects.end();)
  {
    while (cur != objects.end() && cur->get()->IsTombstoned())
      cur = objects.erase_after(prev);

    if (cur == objects.end())
      break;

    cur->get()->Update();

    for (auto& character : characters)
      cur->get()->HandleCollision(character.get());

    cur->get()->ProcessTag();

    prev++;
    cur++;
  }

  // Corrects position afterwards to ensure that collision events are handled first
  for (auto& character : characters)
    CorrectCharacterPos(character.get());

	if (gameOver)
		return;

  gameOver = true;
  for (auto& character : characters)
  {
    if (character.get()->GetCurState() != Character::State::dead)
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
    Event::events.push(event);
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

void Game::SpawnObject(std::unique_ptr<GameObject> newObject)
{
  if (spawnersEnabled)
    objects.push_front(std::move(newObject));
}

int Game::NumCharacters() const
{
  return characters.size();
}

const sf::Vector2f& Game::GetWorldBounds() const
{
  return world.get()->GetBounds();
}

void Game::Attach(World::AttachPoint point, std::function<void(sf::Vector2f)> function)
{
  world.get()->Attach(point, function);
}

void Game::CorrectCharacterPos(Character* character)
{
	sf::Vector2f playerPos = character->GetPosition();
	float posBuffer = 0.5f * SCALED_DIM;

	sf::Vector2f bounds = world.get()->GetBounds();

  float horiOffset = std::abs(playerPos.x) + posBuffer - std::abs(bounds.x);
	if (horiOffset > 0)
    character->WallCollision(-horiOffset * Utility::GetSign(playerPos.x));

  float vertOffset = std::abs(playerPos.y) + posBuffer - std::abs(bounds.y);
	if (vertOffset > 0)
    character->FloorCollision(-vertOffset * Utility::GetSign(playerPos.y));
}

bool Game::IsGameOver() const
{
	return gameOver;
}

void Game::ProcessEvent(Event& event)
{
  switch (event.type)
  {
  case Event::Type::gameTimeUp:
    spawnersEnabled = false;
    for (auto& character : characters)
      character.get()->Kill();
    for (auto& object : objects)
      object.get()->Deactivate();
    break;

  case Event::Type::boostFull:
    characters[event.value].get()->EnableSuperJump();
    break;
  
  default:
    break;
  }
}