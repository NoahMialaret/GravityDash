#ifndef CHARACTER_H
#define CHARACTER_H

#include <SFML/Graphics.hpp>

#include "Animation.h"
#include "Controls.h"
#include "Particle.h"
#include "Point.h"
#include "Utility.h"

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
    stunned,
    dead
  };

public:
  // The player contructors, takes the relative filepath to its texture and the controls that will be used to move it
  Character(const char *spritePath, int charID);
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
  virtual int Land();
  // Attempts to damage the player if it is not invincible
  bool Hit();

  // Returns the players current state
  State GetCurState() const;
  // Returns whether it is the player's last stand
  bool IsLastStand() const;
  // Returns the player's current sprite position
  sf::Vector2f GetPosition() const;
  // Sets the player's sprite position
  virtual void SetPosition(sf::Vector2f &newPos);
  // Returns the player's sprite hitbox
  sf::FloatRect GetHitBox() const;
  // Returns a line segment hitbox represented by the player's previous and current position, used for when
  // the player is airborne and is therefore likely travelling faster than the width of its regular hitbox
  std::pair<sf::Vector2f, sf::Vector2f> GetLineHitBox() const;

  // Adds a point to the points linked list
  void AddNewPoint(sf::Vector2f pos, sf::Vector2f vel);
  void AddNewPoint(int value, sf::Vector2f pos, sf::Vector2f vel);
  // Increments the combo counter by one
  void IncrementComboCount();

protected:
  int charID = 0;

  sf::Texture tex;   // The player's spritesheet
  sf::Sprite sprite; // The player's sprite used for rendering
  Animation anim;    // The player's animation handler

  sf::Vector2f prevPos = ZERO_VECTOR; // The player's position on the previous frame, used in hitbox calculations
  sf::Vector2f vel = ZERO_VECTOR;     // The player's velocity
  int move = 0;
  float acceleration = 0; // A modifier that determines how much the player's velocity changes each frame

  bool isUpright = false; // Whether the player is standing upright (i.e. at the bottom of the world) or is upside down

  State curState = State::idle; // The current state of the player
  bool isLastStand = false;     // Whether this is the player's last jump due to getting hit

  sf::Int32 nextRunParticle = 0; // The time at which to spawn a run particle

  sf::Int32 waitToJump = 0; // The time at which it is okay to jump again

  Point *points = nullptr; // The points a player accumulates from hitting targets during a jump
  int comboCount = 0;      // The number of consecutive targets destroyed in a jump

  int invincibleEnd = 0;
};

class PlayableCharacter : public Character
{
public:
  PlayableCharacter(const char *spritePath, int charID, std::unique_ptr<Controls> &controls);

  void Update() override;

private:
  std::unique_ptr<Controls> controls; // The controls for the player
};

class ComputerCharacter : public Character
{
public:
  ComputerCharacter(const char *spritePath, int charID);

  void Update() override;
};

#endif