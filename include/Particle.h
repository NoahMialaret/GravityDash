#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Clock.h"
#include "Entity.h"
#include "Textures.h"
#include "ProgramSettings.h"

#include <iostream>

#define PUFF 0
#define DUST 1
#define EXPLOSION 2
#define SPEEDLINE 3

class Particle
{
public:
  Particle() = delete;
  // Constructor that takes the type of particle that should be spawned, as well as its position, velocity, and (optionally) scale
  Particle(int timer);

  // Returns whether the particle has reached the end of its lifespan
  bool HasFinished() const;

  // Updates the particles animation and motion
  virtual void Update();
  // Renders the particle
  virtual void Render(sf::RenderWindow *win) const;

protected:
  Entity entity; // The particle's entity, used for movement and animation
  int timer = 0; // How long the particle lasts before being deleted
};

// Puff particle, such as when the player walks
class Puff : public Particle
{
public:
  Puff(sf::Vector2f source, sf::Vector2f dir);
};

// Dust particles, such as when the player lands from a jump
class Dust : public Particle
{
public:
  Dust(sf::Vector2f source, bool flip);
  void Update() override;
  void Render(sf::RenderWindow *win) const override;

private:
  Entity entityMirror; // This is a mirrored version of the entity in the parent class
};

// Explosion particle, such as when a game object is destroyed
class Explosion : public Particle
{
public:
  Explosion(sf::Vector2f source);
};

// Speed line particle, uses to convey fast motion
class SpeedLine : public Particle
{
public:
  SpeedLine(sf::Vector2f start, float speed);
};

#endif