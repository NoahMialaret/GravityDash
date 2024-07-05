#ifndef PARTICLE_H
#define PARTICLE_H

#include <SFML/Graphics.hpp>

#include "Animation.h"

#include <iostream>

// A class used to represent a linked list of short-lived particle affects
class Particle
{
public:
  // The type of particle, used to determine animation attributes and lifespan
  enum class Type
  {
    walkDust,
    landingImpact,
    targetExplosion
  };

public:
  Particle() = delete;
  // Constructor that takes the type of particle that should be spawned, as well as its position, velocity, and (optionally) scale
  Particle(Type type, sf::Vector2f vel, sf::Vector2f pos, sf::Vector2f scale);

  // Returns whether the particle has reached the end of its lifespan
  bool HasFinished() const;

  // Updates this and the next in line particle's position and animation, and determines whether it has reached the end of its lifespan
  void Update();
  // Renders this and the next in line particle's current sprite
  void Render(sf::RenderWindow *win) const;

  static sf::Texture tex;     // The texture used by all the particles

private:
  sf::Sprite sprite; // The particle's sprite to be used for rendering
  Animation anim;    // The particle's animation handler
  sf::Vector2f vel;  // The particle's velocity

  bool hasFinished = false; // Whether or not the particle has reached the end of its lifespan
  sf::Int32 EndOfLifespan;  // The global time at which the particle will reach the end of its lifespan
};

#endif