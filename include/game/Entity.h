#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "Bezier.h"
#include "BezierTransition.h"
#include "AssetManager.h"
#include "Utility.h"

#include <memory>

// An Entity class used to encapsulate the various functions a typical
// game entity might have, such as sprites, animations, and motions
class Entity
{
public:
  Entity() = default;
  // Constructs `Entity` with a sprite using the texture specified at `texName`, 
  // as well as the shader and the number of sprites in the texture used
  Entity(const char* texName, 
          sf::Vector2i numSprites = {1, 1}, 
          sf::Vector2f pos = ZERO_VECTOR, 
          sf::Vector2f origin = {0.5f, 0.5f});

  Entity(const Entity& rhs);
  // Entity(Entity&& rhs);
  Entity& operator=(const Entity& rhs);
  // Entity& operator=(Entity&& rhs);

  // Gets a pointer to the entity's position
  sf::Vector2f* GetPosition();
  // Gets a pointer to the entity's scale
  sf::Vector2f* GetScale();
  // Gets a pointer to the entity's rotation
  float* GetRotation();

  // Updates the entity's animation, as well as its position, scale, and rotation
  // with either the respective pointers or handlers 
  void Update();
  // Renders the sprite to the screen, using a shader if given
  void Render(sf::RenderWindow* win) const;

  // Flips the entity's X directon (i.e. left <-> right)
  void FlipX();
  // Flips the entity's Y directon (i.e. up <-> down)
  void FlipY();
  // Sets the entity's X direction to right if bool is true, left otherwise
  void SetXDir(bool right);
  // Sets the entity's X direction to upright if bool is true, upsidedown otherwise
  void SetYDir(bool up);

  // Gets the entity's hitbox based on it's rendering region on the screen
  sf::FloatRect HitBox() const;


  // Pushes an animation for the entity
  void PushAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);
  // Clears the animation queue and replaces it with a new animation
  void SetAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);
  // Clears all animations in the queue
  void ClearAnimation();


  // Clears the all transitions
  void ClearTransitions();

  // Pushes a position transition from its current rotation to a given offset
  void PushPositionTransition(Bezier curve, float duration, sf::Vector2f offset);
  // Pushes a position transition between a start and end rotation
  void PushPositionTransition(Bezier curve, float duration, sf::Vector2f start, sf::Vector2f end);

  // Pushes a scale transition from its current rotation to a given offset
  void PushScaleTransition(Bezier curve, float duration, sf::Vector2f offset);
  // Pushes a scale transition between a start and end rotation
  void PushScaleTransition(Bezier curve, float duration, sf::Vector2f start, sf::Vector2f end);

  // Pushes a rotation transition from its current rotation to a given offset
  void PushRotationTransition(Bezier curve, float duration, float offset);
  // Pushes a rotation transition between a start and end rotation
  void PushRotationTransition(Bezier curve, float duration, float start, float end);

private:
  // The sprite used by the entity for rendering
  mutable sf::Sprite sprite;

  // The entity's position in the game window
  sf::Vector2f position = ZERO_VECTOR;
  // The entity's scale
  sf::Vector2f scale = {1.0f, 1.0f};
  // The entity's rotation
  float rotation = 0.0f;

  // The entity's animation handler
  AnimationHandler anim;
  // The entity's smooth motion handler
  BezierTransition<sf::Vector2f> positionTransition;
  // The entity's smooth scaling handler
  BezierTransition<sf::Vector2f> scaleTransition;
  // The entity's smooth rotation handler
  BezierTransition<float> rotationTransition;
};

#endif