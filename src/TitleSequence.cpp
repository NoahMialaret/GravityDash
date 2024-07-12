#include "TitleSequence.h"

TitleSequence::TitleSequence()
{
  std::vector<sf::Vector2f> bezPoints = 
  {
    sf::Vector2f(0.0f, Utility::gameScale * 100.0f),
    sf::Vector2f(Utility::gameScale * 100.0f / 6.0f, 200.0f),
    sf::Vector2f(Utility::gameScale * 100.0f / 3.0f, 0.0f),
    sf::Vector2f(1500.0f, 0.0f)
  };
  bezier = Bezier(bezPoints);

  if (!charTex.loadFromFile("assets/charBW.png")) 
  {
    std::cout << "\tCharacter textures could not be loaded!\n";
  }
  character.setTexture(charTex);
  character.setOrigin(CENTRED_ORIGIN);
  charAnim = AnimationHandler(&character);
  character.setScale(DEFAULT_SCALE);
  charAnim.QueueAnimation(2, 50);

  if (!bgTileTex.loadFromFile("assets/background_tiles.png"))
  {
    std::cout << "\tBG Tiles textures could not be loaded!\n";
  }



  if (!titleTex.loadFromFile("assets/title.png")) 
  {
    std::cout << "\tTitle texture could not be loaded!\n";
  }
  title.setTexture(titleTex);
  title.setScale(DEFAULT_SCALE);
  // title.setOrigin(title.getTextureRect().width / 2, title.getTextureRect().height / 2);
  title.setPosition(sf::Vector2f(-Utility::gameScale * Utility::spriteDim * 7.0f + Utility::gameScale, 
                    -Utility::gameScale * Utility::spriteDim * 3.5));

  if (!frenchieTex.loadFromFile("assets/frenchie_t_sat.png")) 
  {
    std::cout << "\tFrenchie texture could not be loaded!\n";
  }
  frenchie.setTexture(frenchieTex);
  frenchie.setScale(sf::Vector2f(Utility::gameScale / 32.0f, Utility::gameScale / 32.0f));
  frenchie.setOrigin(frenchie.getTextureRect().width / 2, frenchie.getTextureRect().height / 2);
  frenchie.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, 0.0f));

  start.setFont(Utility::programFont);
  start.setPosition(sf::Vector2f(0.0f, 1.5f * SCALED_DIM));
  startShadow.setFont(Utility::programFont);
  start.setString("Press Any Button");
  startShadow.setString(start.getString());
  start.setCharacterSize(SCALED_DIM);
  startShadow.setCharacterSize(SCALED_DIM);
  startShadow.setPosition(start.getPosition() + DEFAULT_SCALE);
  start.setFillColor(sf::Color(173, 103, 78));
  startShadow.setFillColor(sf::Color(245, 204, 164));
  start.setOrigin(sf::Vector2f(start.getGlobalBounds().width / 2, start.getGlobalBounds().height / 2));
  startShadow.setOrigin(sf::Vector2f(startShadow.getGlobalBounds().width / 2, startShadow.getGlobalBounds().height / 2));

  frenchieText.setFont(Utility::programFont);
  frenchieText.setString("A game by:\nFrenchie!");
  frenchieText.setCharacterSize(SCALED_DIM);
  frenchieText.setFillColor(sf::Color(255, 229, 181));
  frenchieText.setOrigin(sf::Vector2f(frenchieText.getGlobalBounds().width / 2, - frenchie.getGlobalBounds().height / 4.0f));

  sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
  sf::IntRect worldRect(- worldSize / 2, worldSize);
  world = std::make_unique<World>(worldRect);

  startTime = CUR_TIME;
}

void TitleSequence::Update()
{
  sf::Int32 time = CUR_TIME - startTime;
  charAnim.Update();

  std::uniform_int_distribution chance(0, 100);
  int spawn = chance(Utility::rng);
  if (curSeq < Sequence::spawnWorld && spawn > 40)
  {
    SpawnBGTile();
    int xpos = chance(Utility::rng) * 10;
    int ypos = chance(Utility::rng) * 10;
    Utility::particles.push_front(Particle(Particle::Type::speedLines, sf::Vector2f(0.0f, - speed * Utility::gameScale / 2), sf::Vector2f(xpos - 500, ypos - 500) , DEFAULT_SCALE));
  }

  for (auto& tile : bgTiles)
  {
    tile.move(sf::Vector2f(0.0f, - speed * Utility::gameScale));
  }

  if (Utility::initialKeyPresses.size() > 0)
  {
    if (curSeq < Sequence::title)
    {
      bgTiles.clear();
      Utility::particles.clear();
      curSeq = Sequence::title;
      charAnim.Clear();
      charAnim.QueueAnimation(4, 100, 0, 300);
      charAnim.QueueAnimation(0, 150);
      int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
      character.setPosition(sf::Vector2f(character.getPosition().x, (float)worldBottom - character.getGlobalBounds().height / 2));
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
    character.setPosition(sf::Vector2f(0.0f, bezier.GetValue(time/1500.0f)));
    if (time > 2000)
    {
      curSeq = Sequence::logoIn;
    frenchie.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, bezier.GetValue(-1.0f)));
    frenchieText.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, bezier.GetValue(-1.0f)));
      startTime = CUR_TIME;
    }
    break;

  case Sequence::logoIn:
    frenchie.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, bezier.GetValue(time/1500.0f)));
    frenchie.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    frenchieText.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, bezier.GetValue(time/1500.0f)));
    frenchieText.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    if (time > 2000)
    {
      curSeq = Sequence::logo;
      startTime = CUR_TIME;
    }
    break;
  case Sequence::logo:
    frenchie.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    frenchieText.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    if (time > 3000)
    {
      curSeq = Sequence::logoOut;
      startTime = CUR_TIME;
    }
    break;
  case Sequence::logoOut:
    frenchie.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    frenchie.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, - bezier.GetValue(time/1500.0f, true)));
    frenchieText.setRotation(8 * std::sin((float)CUR_TIME / 512.0f));
    frenchieText.setPosition(sf::Vector2f(-Utility::gameScale * 32.0f, - bezier.GetValue(time/1500.0f, true)));
    if (time > 2000)
    {
      curSeq = Sequence::intermission;
      startTime = CUR_TIME;
    }
    break;

  case Sequence::intermission:
    character.setPosition(sf::Vector2f(0.0f, - bezier.GetValue(time/1500.0f, true)));
    speed *= 1.007;
    if (time > 3000)
    {
      curSeq = Sequence::spawnWorld;
      startTime = CUR_TIME;
      bgTiles.clear();
      Utility::particles.clear();
      // Event newEvent;
      // newEvent.type = Event::Type::showTitle;
      // Utility::events.push_back(newEvent);
    }
    break;

  case Sequence::spawnWorld:
    if (time > 2000)
    {
      curSeq = Sequence::spawnTitle;
      startTime = CUR_TIME;
    }
    break;

  case Sequence::spawnTitle:
  {
    character.move(sf::Vector2f(0.0f, 100.0f));
    int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
    if (character.getGlobalBounds().top + character.getGlobalBounds().height > worldBottom)
    {
      charAnim.Clear();
      charAnim.QueueAnimation(4, 100, 0, 300);
      charAnim.QueueAnimation(0, 150);
      character.setPosition(sf::Vector2f(character.getPosition().x, (float)worldBottom - character.getGlobalBounds().height / 2));
      curSeq = Sequence::title;
    }
    break;
  }

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
    win->draw(title, &Utility::entShad);

    if ((CUR_TIME / 512) % 2 == 1)
    {
      win->draw(startShadow);
      win->draw(start);
    }
  }
  win->draw(character, &Utility::worldShad);
  if (curSeq >= Sequence::logoIn && curSeq <= Sequence::logoOut)
  {
    win->draw(frenchie);
    win->draw(frenchieText);
  }

}

void TitleSequence::SpawnBGTile()
{
  sf::Sprite newTile;
  newTile.setTexture(bgTileTex);
  
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
