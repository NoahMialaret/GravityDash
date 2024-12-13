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
  virtual void HandleCollision(Character* character);
  // Processes the tag and its related event
  virtual void ProcessTag();

  // Renders the object
  void Render(sf::RenderWindow *win) const;

  // Returns the position of the object
  sf::Vector2f GetPosition();

  // Stops the object from moving, potentially removing interaction
  virtual void Deactivate();
  // Allows the object to move and be interactionable again
  virtual void Activate();

  // Whether the object can be deleted after it has been tombstoned and enough time has passed
  bool IsTombstoned() const;

protected:
  const sf::Vector2f& worldBounds;  // World bounds, objects get tombstoned outside of this
  
  Entity entity; // The entity used by the object for rendering and animations

  sf::Vector2f* pos = nullptr;  // The objects position in the game world
  float vel = 0.0f;             // The object's horizontal velocity
  bool activated = true;        // Whether the object can move and (potentially) be interactable 

  bool destructable = true; // Whether collision with the object destroys it, being non-destructive
                            //    implies multiple collisions can happen simultaneously
  bool tombstone = false;   // Whether the object is set for deletion
  int tombstoneTimer = -1;  // A timer for when a tombstoned object can be deleted

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

  // Deactivates the object if a collision has happened and it has been frozen for sufficient time
  void Update() override;
  // Only handles collision if the character is not invincible in some form
  void HandleCollision(Character* character) override;
  // Tombstones and freezes the object if it was tagged
  void ProcessTag() override;

  // Causes the saw to move outside of the game region
  void Deactivate() override;
  // Moves the saw back into active play
  void Activate() override;

private:
  int freezeTimer = -1; // Represents how long the saw is frozen for after collision
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
  // Only handles collision with characters that are airborne
  void HandleCollision(Character* character) override;
  // Tombstones the object and creates an explosion if it was tagged
  void ProcessTag() override;

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

  // Only handles collision with characters that are airborne
  void HandleCollision(Character* character) override;
  // Tombstones the object and creates an explosion if it was tagged
  void ProcessTag() override;
};

#endif