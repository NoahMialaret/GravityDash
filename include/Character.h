#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Clock.h"
#include "Controls.h"
#include "Entity.h"
#include "Number.h"
#include "Particle.h"
#include "PlayerBoost.h"
#include "Textures.h"
#include "Utility.h"

#include <forward_list>
#include <iostream>
#include <memory>
#include <string>

// Represents a playable character in the game world which can be controlled with keyboard inputs
class Character
{
public:
  // The current state of the player, determines animation and control limitations
  enum class State
  {
    idle,
    moving,
    airborne,
    hit,
    stunned,
    dead
  };

public:
  // The constructor which takes an ID and PlayerBoost
  Character(int charID, sf::Vector2f boostPos);
  virtual ~Character();

  // Updates the player's state, animation, and position based on player inputs
  virtual void Update();
  // Renders the player's sprite
  void Render(sf::RenderWindow *win) const;

  // Updates the player's horizontal velocity based on which direction is being held
  void UpdateVelocity(int dir);
  // Initiates a jump if allowed
  void StartJump();
  // Reorientates the player and changes the player's state when the player has landed after jumping
  void Land();
  // Attempts to damage the player if it is not invincible
  bool Hit(sf::Vector2f entPos);

  // Returns the players current state
  State GetCurState() const;
  // Returns whether it is the player's last stand
  bool IsLastStand() const;
  // Returns the player's current sprite position
  sf::Vector2f GetPosition() const;
  // Sets the player's sprite position
  void SetPosition(sf::Vector2f& newPos);
  // Sets the player's velocity
  void SetXVelocity(float xVel);
  void SetYVelocity(float yVel);
  // Returns the player's sprite hitbox
  sf::FloatRect GetHitBox() const;
  // Returns a line segment hitbox represented by the player's previous and current position, used for when
  // the player is airborne and is therefore likely travelling faster than the width of its regular hitbox
  std::pair<sf::Vector2f, sf::Vector2f> GetLineHitBox() const;

  std::forward_list<TargetPoints> GetPoints() const;

  // Adds a point to the points linked list
  void AddNewPoint(sf::Vector2f pos, sf::Vector2f vel);
  void AddNewPoint(int value, sf::Vector2f pos, sf::Vector2f vel);
  // Increments the combo counter by one
  void IncrementComboCount();

protected:
  int charID = 0;

  // THe character's entity used for animations and rendering
  Entity entity;

  sf::Vector2f pos = ZERO_VECTOR;     // The character's position in the game world
  sf::Vector2f prevPos = ZERO_VECTOR; // The character's position on the previous frame, used in hitbox calculations
  sf::Vector2f vel = ZERO_VECTOR;     // The character's velocity

  int move = 0; // The direction that the character wants to move (i.e. the button a player is pressing)
  float acceleration = 0; // A modifier that determines how much the player's velocity changes each frame

  bool isUpright = false; // Whether the player is standing upright (i.e. at the bottom of the world) or is upside down

  State curState = State::idle; // The current state of the player
  bool isLastStand = false;     // Whether this is the player's last jump due to getting hit

  std::forward_list<TargetPoints> targetPoints;

  // Point *points = nullptr; // The points a player accumulates from hitting targets during a jump
  int comboCount = 0;      // The number of consecutive targets destroyed in a jump

  int velTimer = 16; // The timer for when the characters velocity can be updated

  int particleTimer = 0; // The timer for how long until the next run particle is spawned

  int invincibilityTimer = 0; // The timer for how long the player is invincible for
  int stunTimer = 0; // The timer for how long the player is stunned for

  PlayerBoost boost;
};

class PlayableCharacter : public Character
{
public:
  PlayableCharacter(int charID, std::unique_ptr<Controls> &controls, sf::Vector2f boostPos);

  void Update() override;

private:
  std::unique_ptr<Controls> controls; // The controls for the player
};

class ComputerCharacter : public Character
{
public:
  ComputerCharacter(int charID, sf::Vector2f boostPos);

  void Update() override;
};

#endif