// #include "TitleSequence.h"

// TitleSequence::TitleSequence()
// {
//   character = Entity("character", &Utility::worldShad);
//   character.QueueAnimation(2, 50);
//   character.QueueMotion(Curve::easeIn, 2000, sf::Vector2f(0.0f, ProgramSettings::gameScale * 100.0f), ZERO_VECTOR);

//   title = Entity("title", &Utility::entShad, (sf::Vector2i)Textures::textures.at("title").getSize());
//   title.QueueMotion(Curve::linear, 0, sf::Vector2f(0.0f, - 1.25f * ProgramSettings::gameScale * Utility::spriteDim));

//   frenchie = Entity("frenchie", nullptr, (sf::Vector2i)Textures::textures.at("frenchie").getSize());
//   frenchie.QueueScale(Curve::linear, 0, DEFAULT_SCALE, 0.035f * DEFAULT_SCALE);
//   frenchie.Update();
//   frenchie.CoupleRotation(&frenchieRot);


//   start.setFont(Textures::medium);
//   start.setPosition(sf::Vector2f(0.0f, 1.5f * SCALED_DIM));
//   startShadow.setFont(Textures::medium);
//   start.setString("Press Any Button");
//   startShadow.setString(start.getString());
//   start.setCharacterSize(SCALED_DIM);
//   startShadow.setCharacterSize(SCALED_DIM);
//   startShadow.setPosition(start.getPosition() + DEFAULT_SCALE);
//   start.setFillColor(sf::Color(173, 103, 78));
//   startShadow.setFillColor(sf::Color(245, 204, 164));
//   start.setOrigin(sf::Vector2f(start.getGlobalBounds().width / 2, start.getGlobalBounds().height / 2));
//   startShadow.setOrigin(sf::Vector2f(startShadow.getGlobalBounds().width / 2, startShadow.getGlobalBounds().height / 2));

//   frenchieText.setFont(Textures::medium);
//   frenchieText.setString("A game by:\nFrenchie!");
//   frenchieText.setCharacterSize(SCALED_DIM);
//   frenchieText.setFillColor(sf::Color(255, 229, 181));
//   frenchieText.setOrigin(sf::Vector2f(frenchieText.getGlobalBounds().width / 2, - frenchie.HitBox().height / 4.0f));

//   sf::Vector2i worldSize = int(SCALED_DIM) * sf::Vector2i(16, 8);
//   sf::IntRect worldRect(- worldSize / 2, worldSize);
//   world = std::make_unique<World>();

//   timer = 2000;
// }

// void TitleSequence::Update()
// {
//   timer -= Clock::Delta();
  
//   character.Update();
//   title.Update();
//   frenchie.Update();
//   frenchieRot = 8 * std::sin((float)Clock::Elapsed() / 512.0f);

//   std::uniform_int_distribution chance(0, 100);
//   int spawn = chance(Utility::rng);
//   if (curSeq < Sequence::spawnWorld && spawn > 40 / (speed / 5.0f))
//   {
//     SpawnBGTile();
//     float xpos = (float)chance(Utility::rng) * 10.0f;
//     float ypos = (float)chance(Utility::rng) * 10.0f;
//     Utility::particles.push_front(std::make_unique<SpeedLine>(sf::Vector2f(xpos - 500, ypos - 500), - speed * 10.0f));
//   }

//   for (auto& tile : bgTiles)
//   {
//     tile.move((Clock::Delta() / 16.0f) * sf::Vector2f(0.0f, - speed * ProgramSettings::gameScale));
//   }

//   // if (!Keyboard::keysPressed.empty())
//   if (true)
//   {
//     if (curSeq < Sequence::title)
//     {
//       bgTiles.clear();
//       Utility::particles.clear();
//       curSeq = Sequence::title;
//       character.ClearHandlers();
//       character.QueueAnimation(4, 100, 0, 300);
//       character.QueueAnimation(0, 150);
//       int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
//       sf::Vector2f pos(0.0f, (float)worldBottom - character.HitBox().height / 2);
//       character.QueueMotion(Curve::linear, 0, pos, pos);
//     }
//     else
//     {
//       Event newEvent;
//       newEvent.type = Event::Type::reloadMenu;
//       newEvent.menuType = Event::MenuType::main;

//       Event::events.push(newEvent);
//       return;
//     }
//   }

//   switch (curSeq)
//   {
//   case Sequence::start:
//     if (timer <= 0)
//     {
//       sf::Vector2f logoTarget(- ProgramSettings::windowDim.x / 4.0f, 0.0f);
//       curSeq = Sequence::logoIn;
//       frenchie.QueueMotion(Curve::easeIn, 2000, sf::Vector2f(logoTarget.x, ProgramSettings::gameScale * 100.0f), logoTarget);
//       frenchie.QueueMotion(Curve::linear, 3000, ZERO_VECTOR);
//       frenchie.QueueMotion(Curve::easeOut, 2000, sf::Vector2f(0.0f, - ProgramSettings::gameScale * 100.0f));
//       frenchieText.setPosition(frenchie.GetPosition());
//       timer = 2000;
//     }
//     break;

//   case Sequence::logoIn:
//     frenchieText.setPosition(frenchie.GetPosition());
//     frenchieText.setRotation(frenchieRot);
//     if (timer <= 0)
//     {
//       curSeq = Sequence::logo;
//       timer = 3000;
//     }
//     break;
//   case Sequence::logo:
//     frenchieText.setRotation(frenchieRot);
//     if (timer <= 0)
//     {
//       curSeq = Sequence::logoOut;
//       timer = 2000;
//     }
//     break;

//   case Sequence::logoOut:
//     frenchieText.setRotation(frenchieRot);
//     frenchieText.setPosition(frenchie.GetPosition());
//     if (timer <= 0)
//     {
//       curSeq = Sequence::intermission;
//       timer = 3000;
//       character.QueueMotion(Curve::easeOut, 2000, ZERO_VECTOR, sf::Vector2f(0.0f, - ProgramSettings::gameScale * 100.0f));
//     }
//     break;

//   case Sequence::intermission:
//     speed *= 1.007;
//     if (timer <= 0)
//     {
//       curSeq = Sequence::spawnWorld;
//       timer = 2000;
//       bgTiles.clear();
//       Utility::particles.clear();
//     }
//     break;

//   case Sequence::spawnWorld:
//     if (timer <= 0)
//     {
//       int worldBottom = world.get()->GetRegion().top + world.get()->GetRegion().height;
//       character.QueueMotion(Curve::linear, 200, character.GetPosition(), {0.0f, (float)worldBottom - character.HitBox().height / 2});
//       character.SetAnimation(2, 50, 0);
//       character.QueueAnimation(4, 100, 0, 300);
//       character.QueueAnimation(0, 150);

//       title.QueueScale(Curve::easeIn, 1000, ZERO_VECTOR, DEFAULT_SCALE);
//       curSeq = Sequence::title;
//     }
//     break;

//   case Sequence::title:
//     break;
  
//   default:
//     break;
//   }
//   // charAnim.Update();
// }

// void TitleSequence::Render(sf::RenderWindow* win) const
// {
//   if (curSeq >= Sequence::spawnWorld)
//   {
//     world.get()->Render(win);
//   }
//   else
//   {
//     for (auto& s : bgTiles)
//     {
//       win->draw(s);
//     }
//   }

//   Utility::RenderParticles(win);
//   if (curSeq >= Sequence::title)
//   {
//     title.Render(win);

//     if ((Clock::Elapsed() / 512) % 2 == 1)
//     {
//       win->draw(startShadow);
//       win->draw(start);
//     }
//   }
//   character.Render(win);
//   if (curSeq >= Sequence::logoIn && curSeq <= Sequence::logoOut)
//   {
//     frenchie.Render(win);
//     win->draw(frenchieText);
//   }

// }

// void TitleSequence::SpawnBGTile()
// {
//   sf::Sprite newTile;
//   newTile.setTexture(Textures::textures.at("background_tiles"));
  
//   std::uniform_int_distribution texDist(0, 5);
//   int tex = texDist(Utility::rng);
//   sf::IntRect texRect(tex * Utility::spriteDim, 0, Utility::spriteDim, Utility::spriteDim);

//   newTile.setTextureRect(texRect);
//   newTile.setScale(DEFAULT_SCALE);

  
//   std::uniform_int_distribution xDist(- (((int)ProgramSettings::windowDim.x / 2) / (int)(SCALED_DIM)), 
//     ((int)ProgramSettings::windowDim.x / 2) / (int)(SCALED_DIM));
//   int xSpawn = xDist(Utility::rng);

//   sf::Vector2f pos(xSpawn * SCALED_DIM, ProgramSettings::windowDim.y);
//   newTile.setPosition(pos);

//   bgTiles.emplace_front(newTile);
// }
