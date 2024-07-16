#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Character.h"
#include "Clock.h"
#include "Entity.h"
#include "Particle.h"
#include "Textures.h"
#include "Utility.h"

#include <iostream>
#include <memory>

// A class representing the non-player interactable objects that can appear during gameplay
class GameObject
{
public:
  GameObject(int maxID);
  virtual ~GameObject() = default;

  // Virtual function for updating object logic
  void virtual Update(std::vector<Character *> players) = 0;
  // Renders the object
  void Render(sf::RenderWindow *win) const;

  // Stops the object from moving
  void Freeze();
  // Allows the object to move
  void Unfreeze();

  // Whether the object has reached the end of its life
  bool EndOfLife() const;

  // Operator overloads for comparing the vertical position of the enities
  bool operator<=(GameObject& rhs); 
  bool operator<(GameObject& rhs); 
  bool operator>=(GameObject& rhs); 
  bool operator>(GameObject& rhs); 
  bool operator<=(float rhs); 
  bool operator<(float rhs); 
  bool operator>=(float rhs); 
  bool operator>(float rhs); 

  sf::Vector2f GetPosition();

protected:
  int entID = 0;
  
  Entity entity; // The entity used by the object for rendering and animations

  sf::Vector2f pos = ZERO_VECTOR; // The objects position in the game world
  float vel = 0.0f;  // The object's horizontal velocity

  bool isFrozen = false; // Whether the movement of the object has be frozen

  bool endOfLife = false; // Whether the object has reached the end of its lifespan
};

// The saw is an object that deals damage to the player on contact, 
// moves along the upper or lower border of the world
class Saw : public GameObject
{
public:
  Saw() = delete;
  // Constructor uses the world border to determine spawn positions and cutoff points
  Saw(sf::IntRect &worldBorder, int maxID);
  // Updates the spike's position and checks for player collisions
  void Update(std::vector<Character *> players) override;

private:
  float cutOffPoint = 0.0f; // The pixel position of the screen at which the object should be culled
};

// The targets are the entities that the player is trying to hit to score points,
// they normally spawn randomly in the middle of the world
class MovingTarget : public GameObject
{
public:
  MovingTarget() = delete;
  // Constructor uses the world border to determine spawn positions and cutoff points
  MovingTarget(sf::IntRect &worldBorder, int maxID);
  // Updates the spike's position and checks for player collisions
  void Update(std::vector<Character *> players) override;

private:
  float cutOffPoint = 0.0f; // The pixel position of the screen at which the object should be culled
};

// class StationaryTarget : public GameObject
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