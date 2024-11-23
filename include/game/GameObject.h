#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Character.h"
#include "Clock.h"
#include "Entity.h"
#include "Event.h"
#include "Particle.h"
#include "ProgramSettings.h"
#include "Textures.h"
#include "Utility.h"

#include <iostream>
#include <memory>

// A class representing interactable game objects,
// collision causes them to be tagged and send out an event
class GameObject
{
public:
  // Constructs the object's boundary and entity
  GameObject(const sf::Vector2f& worldBounds);

  // Updates the objects position and entity
  virtual void Update();
  // Checks and handles collision with a character
  void HandleCollision(Character* character);
  // Processes the tag and its related event
  void ProcessTag();

  // Renders the object
  void Render(sf::RenderWindow *win) const;

  // Returns the position of the object
  sf::Vector2f GetPosition();

  // Stops the object from moving, potentially removing interaction
  virtual void Deactivate();
  // Allows the object to move and be interactionable again
  virtual void Activate();

  // Whether the object is set for deletion
  bool IsTombstoned() const;

protected:
  const sf::Vector2f& worldBounds;  // World bounds, objects get tombstoned outside of this
  
  Entity entity; // The entity used by the object for rendering and animations

  sf::Vector2f pos;       // The objects position in the game world
  float vel = 0.0f;       // The object's horizontal velocity
  bool activated = true;  // Whether the object can move and (potentially) be interactable 

  bool destructable = true; // Whether collision with the object destroys it, being non-destructive
                            //    implies multiple collisions can happen simultaneously
  bool tombstone = false;   // Whether the object is set for deletion

  int tag = -1;         // The charID of the character who tagged this object
  float tagSquareDist;  // The squared distance the character was when tagging
  Event tagEvent;       // The event associated with the act of tagging
};

// The `Saw` is an object that stuns the player on contact, 
// moves along the upper or lower border of the world
class Saw : public GameObject
{
public:
  // Initialises `Saw` with a random position and direction on the top or bottom of the world
  Saw(const sf::Vector2f& worldBounds);

  // Causes the saw to move outside of the game region
  void Deactivate() override;
  // Moves the saw back into active play
  void Activate() override;
};

// The `MovingTarget` is what the player is trying to hit to score points,
// they spawn randomly in the vertical centre of the world
class MovingTarget : public GameObject
{
public:
  // Initialises `MovingTarget` with a random velocity and vertical position 
  MovingTarget(const sf::Vector2f& worldBounds);
  // Updates the oscillation of the object
  void Update() override;

private:
  float oscillationSpeed;  // How fast the object oscillates
  float yBase;             // The base y positon of the sprite before oscilations
};

// The `TimeBonus` can be collected to increase the duration of the game timer,
// spawns randomly in the vertical centre of the world
class TimeBonus : public GameObject
{
public:
  // Initialises `TimeBonus` with a random velocity and vertical position 
  TimeBonus(const sf::Vector2f& worldBounds);
};

#endif