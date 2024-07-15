#ifndef TITLESEQUENCE_H
#define TITLESEQUENCE_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Bezier.h"
#include "Clock.h"
#include "Particle.h"
#include "Textures.h"
#include "Utility.h"
#include "World.h"

#include <forward_list>
#include <iostream>
#include <memory>

enum class Sequence
{
  start,
  logoIn,
  logo,
  logoOut,
  intermission,
  spawnWorld,
  spawnTitle,
  title,
  end,
};

class TitleSequence
{
public:
  TitleSequence();
  void Update();
  void Render(sf::RenderWindow* win) const;

private:
  void SpawnBGTile();

private:
  Sequence curSeq = Sequence::start;
  Bezier bezier;

  std::unique_ptr<World> world;
  std::forward_list<sf::Sprite> bgTiles;
  float speed = 5.0f;

  sf::Sprite character;
  AnimationHandler charAnim;

  sf::Sprite title;

  sf::Sprite frenchie;

  sf::Text frenchieText;

  sf::Text start;
  sf::Text startShadow;

  int timer = 0;
};

#endif