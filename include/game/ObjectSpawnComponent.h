#ifndef OBJECTSPAWNCOMPONENT_H
#define OBJECTSPAWNCOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Game.h"
#include "GameComponent.h"
#include "GameObject.h"
#include "Utility.h"

#include <functional>
#include <memory>
#include <random>

// A GameComponent which handles the spawning of game objects specified by the template
template <typename Object>
class ObjectSpawnComponent : public GameComponent
{
public:
  // Constructs a spawner with a given cooldown rate and spawn probability
  ObjectSpawnComponent(Game* game, int cooldown, int cooldownVar, float probability = 1.0f);

  // Spawns an object with probability `probability` if enough time has passed
  void Update() override;

private:
  int timer;  // Time left until next object spawn is attempted

  int cooldown;     // How much the timer is increased after spawning
  int cooldownVar;  // An addition variance range (+-) that is added to the timer

  float probability;  // The probabilty that an object is spawned when attempted
};

template <typename Object>
inline ObjectSpawnComponent<Object>::ObjectSpawnComponent(Game *game, int cooldown, int cooldownVar, float probability)
  :
  GameComponent(game),
  timer(cooldown),
  cooldown(cooldown),
  cooldownVar(cooldownVar),
  probability(probability)
{}

template <typename Object>
inline void ObjectSpawnComponent<Object>::Update()
{
  timer -= Clock::Delta();

  while (timer < 0)
  {
    std::uniform_int_distribution variance(-cooldownVar, cooldownVar);
    std::uniform_real_distribution<float> poll;

    timer += cooldown + variance(Utility::rng);
    if (poll(Utility::rng) < probability)
      game->SpawnObject(std::make_unique<Object>(game->GetWorldBounds()));
  }
}

#endif