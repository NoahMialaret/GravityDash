#include "TitleSequence.h"

TitleSequence::TitleSequence()
{
  Entity::Params params;
  params.shader = &Utility::worldShad;
  character = Entity("character", params);
  character.QueueAnimation(2, 50);
  character.QueueMotion(MotionHandler::Type::easeIn, 2000, sf::Vector2f(0.0f, Utility::gameScale * 100.0f), ZERO_VECTOR);

  params.shader = &Utility::entShad;
  params.frameSize = (sf::Vector2i)Textures::textures.at("title").getSize();
  title = Entity("title", params);
  title.QueueMotion(MotionHandler::Type::linear, 0, ZERO_VECTOR, 
    sf::Vector2f(0.0f, - 1.25f * Utility::gameScale * Utility::spriteDim));

  params.shader = nullptr;
  params.frameSize = (sf::Vector2i)Textures::textures.at("frenchie").getSize();
  params.scaleModifier = 0.035;
  frenchie = Entity("frenchie", params);


  start.setFont(Textures::font);
  start.setPosition(sf::Vector2f(0.0f, 1.5f * SCALED_DIM));
  startShadow.setFont(Textures::font);
  start.setString("Press Any Button");
  startShadow.setString(start.getString());
  start.setCharacterSize(SCALED_DIM);
  startShadow.setCharacterSize(SCALED_DIM);
  startShadow.setPosition(start.getPosition() + DEFAULT_SCALE);
  start.setFillColor(sf::Color(173, 103, 78));
  startShadow.setFillColor(sf::Color(245, 204, 164));
  start.setOrigin(sf::Vector2f(start.getGlobalBounds().width / 2, start.getGlobalBounds().height / 2));
  startShadow.setOrigin(sf::Vector2f(startShadow.getGlobalBounds().width / 2, startShadow.getGlobalBounds().height / 2));

  frenchieText.setFont(Textures::font);
  frenchieText.setString("A game by:\nFrenchie!");
  frenchieText.setCharacterSize(SCALED_DIM);
  frenchieText.setFillColor(sf::Color(255, 229, 181));
  frenchieText.setOrigin(sf::Vector2f(frenchieText.getGlobalBounds().width / 2, - frenchie.HitBox().height / 4.0f));

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
  sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  timer = 2000;
}

void TitleSequence::Update()
{
  timer -= Clock::Delta();
  
  character.Update();
  title.Update();
  frenchie.Update();
  frenchie.SetRotation(8 * std::sin((float)Clock::Elapsed() / 512.0f));

  std::uniform_int_distribution chance(0, 100);
  int spawn = chance(Utility::rng);
  if (curSeq < Sequence::spawnWorld && spawn > 40 / (speed / 5.0f))
  {
    SpawnBGTile();
    float xpos = (float)chance(Utility::rng) * 10.0f;
    float ypos = (float)chance(Utility::rng) * 10.0f;
    Utility::particles.push_front(std::make_unique<SpeedLine>(sf::Vector2f(xpos - 500, ypos - 500), - speed * 10.0f));
  }

  for (auto& tile : bgTiles)
  {
    tile.move((Clock::Delta() / 16.0f) * sf::Vector2f(0.0f, - speed * Utility::gameScale));
  }

  if (Utility::initialKeyPresses.size() > 0)
  {
    if (curSeq < Sequence::title)
    {
      bgTiles.clear();
      Utility::particles.clear();
      curSeq = Sequence::title;
      character.ClearHandlers();
      character.QueueAnimation(4, 100, 0, 300);
      character.QueueAnimation(0, 150);
      int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
      sf::Vector2f pos(0.0f, (float)worldBottom - character.HitBox().height / 2);
      character.QueueMotion(MotionHandler::Type::linear, 0, pos, pos);
    }
    else
    {
      Event newEvent;
      newEvent.type = Event::Type::geToMainMenu;
      Utility::events.push_back(newEvent);
      return;
    }
  }

  switch (curSeq)
  {
  case Sequence::start:
    if (timer <= 0)
    {
      sf::Vector2f logoTarget(- Utility::windowDim.x / 4.0f, 0.0f);
      curSeq = Sequence::logoIn;
      frenchie.QueueMotion(MotionHandler::Type::easeIn, 2000, sf::Vector2f(logoTarget.x, Utility::gameScale * 100.0f), logoTarget);
      frenchie.QueueMotion(MotionHandler::Type::linear, 3000, logoTarget, logoTarget);
      frenchie.QueueMotion(MotionHandler::Type::easeOut, 2000, logoTarget, sf::Vector2f(logoTarget.x, - Utility::gameScale * 100.0f));
      frenchieText.setPosition(frenchie.GetPosition());
      timer = 2000;
    }
    break;

  case Sequence::logoIn:
    frenchieText.setPosition(frenchie.GetPosition());
    frenchieText.setRotation(8 * std::sin((float)Clock::Elapsed() / 512.0f));
    if (timer <= 0)
    {
      curSeq = Sequence::logo;
      timer = 3000;
    }
    break;
  case Sequence::logo:
    frenchieText.setRotation(8 * std::sin((float)Clock::Elapsed() / 512.0f));
    if (timer <= 0)
    {
      curSeq = Sequence::logoOut;
      timer = 2000;
    }
    break;

  case Sequence::logoOut:
    frenchieText.setRotation(8 * std::sin((float)Clock::Elapsed() / 512.0f));
    frenchieText.setPosition(frenchie.GetPosition());
    if (timer <= 0)
    {
      curSeq = Sequence::intermission;
      timer = 3000;
      character.QueueMotion(MotionHandler::Type::easeOut, 2000, ZERO_VECTOR, sf::Vector2f(0.0f, - Utility::gameScale * 100.0f));
    }
    break;

  case Sequence::intermission:
    speed *= 1.007;
    if (timer <= 0)
    {
      curSeq = Sequence::spawnWorld;
      timer = 2000;
      bgTiles.clear();
      Utility::particles.clear();
      // Event newEvent;
      // newEvent.type = Event::Type::showTitle;
      // Utility::events.push_back(newEvent);
    }
    break;

  case Sequence::spawnWorld:
    if (timer <= 0)
    {
      int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
      character.QueueMotion(MotionHandler::Type::linear, 200, character.GetPosition(), {0.0f, (float)worldBottom - character.HitBox().height / 2});
      character.SetAnimation(2, 50, 0);
      character.QueueAnimation(4, 100, 0, 300);
      character.QueueAnimation(0, 150);
      curSeq = Sequence::title;
    }
    break;

  case Sequence::title:
    break;
  
  default:
    break;
  }
  // charAnim.Update();
}

void TitleSequence::Render(sf::RenderWindow* win) const
{
  if (curSeq >= Sequence::spawnWorld)
  {
    world.get()->Render(win);
  }
  else
  {
    for (auto& s : bgTiles)
    {
      win->draw(s);
    }
  }

  Utility::RenderParticles(win);
  if (curSeq >= Sequence::title)
  {
    title.Render(win);

    if ((Clock::Elapsed() / 512) % 2 == 1)
    {
      win->draw(startShadow);
      win->draw(start);
    }
  }
  character.Render(win);
  if (curSeq >= Sequence::logoIn && curSeq <= Sequence::logoOut)
  {
    frenchie.Render(win);
    win->draw(frenchieText);
  }

}

void TitleSequence::SpawnBGTile()
{
  sf::Sprite newTile;
  newTile.setTexture(Textures::textures.at("background_tiles"));
  
  std::uniform_int_distribution texDist(0, 5);
  int tex = texDist(Utility::rng);
  sf::IntRect texRect(tex * Utility::spriteDim, 0, Utility::spriteDim, Utility::spriteDim);

  newTile.setTextureRect(texRect);
  newTile.setScale(DEFAULT_SCALE);

  
  std::uniform_int_distribution xDist(- (((int)Utility::windowDim.x / 2) / (int)(SCALED_DIM)), 
    ((int)Utility::windowDim.x / 2) / (int)(SCALED_DIM));
  int xSpawn = xDist(Utility::rng);

  sf::Vector2f pos(xSpawn * SCALED_DIM, Utility::windowDim.y);
  newTile.setPosition(pos);

  bgTiles.emplace_front(newTile);
}
