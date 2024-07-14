#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Character.h"
#include "Particle.h"
#include "Textures.h"
#include "Utility.h"

#include <iostream>

// A class of non-player entities that can appear during gameplay
class Entity
{
public:
  Entity(int maxID);
  virtual ~Entity() = default;

  // Virtual function for updating entity logic
  void virtual Update(std::vector<Character *> players) = 0;
  // Renders the entity
  void Render(sf::RenderWindow *win) const;

  // Stops the entity from moving
  void Freeze();
  // Allows the entity to move
  void Unfreeze();

  // Whether the entity has reached the end of its life
  bool EndOfLife() const;

  // Operator overloads for comparing the vertical position of the enities
  bool operator<=(Entity& rhs); 
  bool operator<(Entity& rhs); 
  bool operator>=(Entity& rhs); 
  bool operator>(Entity& rhs); 
  bool operator<=(float rhs); 
  bool operator<(float rhs); 
  bool operator>=(float rhs); 
  bool operator>(float rhs); 

  sf::Vector2f GetPosition()
  {
    return sprite.getPosition();
  }

protected:
  int entID = 0;
  sf::Sprite sprite;      // The sprite used by the entity
  AnimationHandler anims; // The entity's animation handler

  float vel = 0.0f;  // The entity's horizontal velocity
  bool isFrozen = false; // Whether the movement of the entity has be frozen

  bool endOfLife = false; // Whether the entity has reached the end of its lifespan
};

// The saw is an entity that deals damage to the player on contact, 
// moves along the upper or lower border of the world
class Saw : public Entity
{
public:
  Saw() = delete;
  // Constructor uses the world border to determine spawn positions and cutoff points
  Saw(sf::IntRect &worldBorder, int maxID);
  // Updates the spike's position and checks for player collisions
  void Update(std::vector<Character *> players) override;

private:
  float cutOffPoint = 0.0f; // The pixel position of the screen at which the entity should be culled
};

// The targets are the entities that the player is trying to hit to score points,
// they normally spawn randomly in the middle of the world
class MovingTarget : public Entity
{
public:
  MovingTarget() = delete;
  // Constructor uses the world border to determine spawn positions and cutoff points
  MovingTarget(sf::IntRect &worldBorder, int maxID);
  // Updates the spike's position and checks for player collisions
  void Update(std::vector<Character *> players) override;

private:
  float cutOffPoint = 0.0f; // The pixel position of the screen at which the entity should be culled
};

// class StationaryTarget : public Entity
// {
//   StationaryTarget() = delete;
//   // Constructor uses the world border to determine spawn positions and cutoff points
//   StationaryTarget(sf::Texture* tex, sf::IntRect &worldBorder, int lifespan);
//   // Spawns a stationary target at the desired position
//   StationaryTarget(sf::Vector2f pos);
//   // Updates the spike's position and checks for player collisions
//   void Update(std::vector<Character *> players) override;

// }

#endif