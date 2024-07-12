#ifndef TITLESEQUENCE_H
#define TITLESEQUENCE_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Bezier.h"
#include "Particle.h"
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
  sf::Texture bgTileTex;
  float speed = 5.0f;

  sf::Texture charTex;
  sf::Sprite character;
  AnimationHandler charAnim;

  sf::Texture titleTex;
  sf::Sprite title;

  sf::Texture frenchieTex;
  sf::Sprite frenchie;

  sf::Text frenchieText;

  sf::Text start;
  sf::Text startShadow;

  sf::Int32 startTime = 0;
};

#endif