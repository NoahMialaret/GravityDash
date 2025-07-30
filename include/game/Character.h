#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Controls.h"
#include "Event.h"
#include "Entity.h"
#include "Particle.h"
#include "ParticleManager.h"
#include "Utility.h"
#include "Settings.h"

#include <iostream>
#include <memory>
#include <string>

#define IDLE_ANIM 0
#define WALK_ANIM 1
#define JUMP_ANIM 2
#define STUN_ANIM 3
#define LAND_ANIM 4
#define REST_ANIM 5
#define NUM_ANIMS 6

// `Character` represents a game entity controlled by users or 
//  CPUs to interact with the game and its components
class Character
{
public:
  // The current state of the player, determines animations and behaviours
  enum class State
  {
    idle,
    moving, 
    // Cannot jump if `curState` is any of the below states
    airborne,
    stunned,
    dead
  };

public:
  // Constructs `character` and its entities
  Character(int charID, int playerNum);

  // Updates the characters's state, animation, position based on its current state
  virtual void Update();
  // Renders the character's entity and reticle
  void Render(sf::RenderWindow* win) const;

  // Corrects position for collisions with floors, and performs landing sequence if required
  void FloorCollision(float correction);
  // Corrects position for collisions with walls
  void WallCollision(float correction);

  // Attempts to stun the character given the position of the source, 
  //  returns `true` if the stun was successful
  bool Hit(sf::Vector2f source);
  // Stuns the character and leaves them with one remaining jump
  void MakeFinalJump();

  // Returns `charID`, the identifier of the character
  int GetID();
  // Returns `curState`, the current state of the character
  State GetCurState() const;
  // Returns if the character is invincible or otherwise cannot be hit
  bool IsInvincible() const;
  // Returns `*pos`, the characters's current world position
  sf::Vector2f GetPosition() const;
  // Returns the global bounds of the character's sprite
  sf::FloatRect GetHitBox() const;
  // Returns a line segment hitbox represented by the character's previous and current position,
  //  correlates to the region the character occupied between frames
  Utility::LineSegment GetLineHitBox() const;

  // Enables the ability to do a super jump
  void EnableSuperJump();

protected:
  // Updates the character's velocity based on horizontal direction `dir`
  void UpdateVelocity(int dir);
  // Updates the reticle's position and entity with reference to the character
  void UpdateReticle();

  // Attempts to cause the character to jump, returns true on success
  bool Jump();
  // Attempts to cause the character to perform a "super jump", returns true on success
  bool SuperJump();
  // Handles animations and other variable management when landing from a jump
  void Land();

  // Stuns the character, causing them to freeze in place for a short period
  void Stun();

protected:
  int charID = 0;     // The unique identifier used by game components and events
  int playerNum = 0;  // Used for colour lookup

  State curState = State::idle; // The current state of the player

  mutable Entity entity;  // The character's entity used for animations and rendering

  sf::Vector2f* pos =    nullptr;     // The character's position in the game world
  sf::Vector2f prevPos = ZERO_VECTOR; // The character's position on the previous frame, used in hitbox calculations
  sf::Vector2f vel =     ZERO_VECTOR; // The character's velocity

  int horiDir =        0; // The horizontal direction of movement
  float acceleration = 0; // How much the velocity changes each frame

  bool isUpright = true;  // Whether the character is standing upright in relation to the window
  bool grounded = false;  // Whether the character is currently grouned

  bool queueFinalJump = false;  // Used to enable the final jump if the character is already mid-jump
  bool finalJump =      false;  // Whether this is the player's last jump

  int invincibilityTimer = 0; // The timer for how long the player is invincible for
  int runParticleTimer =   0; // The timer for how long until the next run particle is spawned
  int stunTimer =          0; // The timer for how long the player is stunned for
  int velTimer =          16; // The timer for when the characters velocity can be updated

  bool canSuperJump = false;  // Wether the character is able to perform a "super jump"
  int superBouncesLeft = -1;  // How many bounces are left before the player lands from a "super jump"

  Entity reticle;                     // `reticle` is an entity displaying the angle that a "super jump" will travel
  sf::Vector2f* reticlePos = nullptr; // The position of `reticle`
  float reticleAngle = 0.0f;          // The angle of `reticle`
};

// Represents a `Character` class that is controlled by the user
class PlayableCharacter : public Character
{
public:
  // Construct `PlayableCharacter` with the given controls used for user interfacing
  PlayableCharacter(int charID, int playerNum);

  // Updates movement information based on the controls being pressed
  void Update() override;
};

// Represents a `Character` class that is controlled by the computer (i.e. random inputs)
class ComputerCharacter : public Character
{
public:
  // Constructs `ComputerCharacter` by only calling the `Character` contructor
  ComputerCharacter(int charID, int playerNum);

  // Updates movement information based on randomness, with additional weight
  // to the action already being performed
  void Update() override;

private:
  int decisionTimer = 16; // The time until the computer can make another decision
};

#endif