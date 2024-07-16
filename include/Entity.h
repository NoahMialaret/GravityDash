#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "MotionHandler.h"
#include "Textures.h"
#include "Utility.h"

#include <memory>

// An Entity class used to encapsulate the various functions a typical
// game entity might have, such as sprites, animations, and motions
class Entity
{
public:
  // The paramaters used during the initialisation of an entity
  struct Params
  {
    // A modifier for the entities scale, multiplied with the gamescale
    float scaleModifier = 1.0f;
    // The shader that may be used during redering if given
    sf::Shader* shader = &Utility::entShad;
    // The size of the sprite's animation frame
    sf::Vector2i frameSize = sf::Vector2i(Utility::spriteDim, Utility::spriteDim);
  };
public:
  Entity() = default;
  // Constructs an entity with the given parameter
  Entity(const char* textureName, Params params);

  // Couples the position of the sprite to some external position vector
  void CouplePosition(sf::Vector2f* pos);
  // Decouples the sprite's position with the external position
  void DecouplePosition();

  // Updates the entity's animation, as well as its position with either the vector pointer or motion
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

  // Gets the sprite's position
  sf::Vector2f GetPosition() const;
  // Sets the sprite's rotation
  void SetRotation(float angle);

  // Clears the entity's animation and motion handlers
  void ClearHandlers();

  // Gets the entity's hitbox based on it's rendering region on the screen
  sf::FloatRect HitBox() const;

  // Queues an animation for the entity
  void QueueAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);
  // Clears the animation queue and replaces it with a new animation
  void SetAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);

  // Queues a motion from the entity's current position to some desired offset
  void QueueMotion(MotionHandler::Type type, float duration, sf::Vector2f offSet);
  void QueueMotion(MotionHandler::Type type, float duration, sf::Vector2f start, sf::Vector2f end);

private:
  // The sprite used by the entity for rendering
  std::unique_ptr<sf::Sprite> sprite;
  Params params;

  // A pointer to an external position vector if the sprite's position has been coupled
  sf::Vector2f* position = nullptr;

  // The entity's animation handler
  AnimationHandler anim;
  // The entity's motion handler, only gets used if the position isn't coupled
  MotionHandler motion;
  // May do rotations and scale handlers in the future
};

#endif