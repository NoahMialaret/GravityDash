#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

#include "AnimationHandler.h"
#include "MotionHandler.h"
#include "RotationHandler.h"
#include "ScaleHandler.h"
#include "Textures.h"
#include "Utility.h"

#include <memory>

// An Entity class used to encapsulate the various functions a typical
// game entity might have, such as sprites, animations, and motions
class Entity
{
public:
  Entity() = default;
  // Constructs an entity with a texture, shader, and framesize if given
  Entity(const char* textureName, sf::Shader* shader = &Utility::entShad, 
          sf::Vector2i frameSize = sf::Vector2i(Utility::spriteDim, Utility::spriteDim));

  // Couples the position of the sprite to some external position vector
  void CouplePosition(sf::Vector2f* pos);
  // Decouples the sprite's position with the external position
  void DecouplePosition();

  // Couples the scale of the sprite to some external scale vector
  void CoupleScale(sf::Vector2f* scale);
  // Decouples the sprite's scale with the external scale
  void DecoupleScale();

  // Couples the rotation of the sprite to some external rotation vector
  void CoupleRotation(float* rot);
  // Decouples the sprite's rotation with the external rotation
  void DecoupleRotation();

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

  // Gets the sprite's position
  sf::Vector2f GetPosition() const;
  // Gets the sprite's scale
  sf::Vector2f GetScale() const;
  // Gets the sprite's rotation
  float GetRotation() const;

  // Clears the entity's handlers
  void ClearHandlers();

  // Gets the entity's hitbox based on it's rendering region on the screen
  sf::FloatRect HitBox() const;

  // Queues an animation for the entity
  void QueueAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);
  // Clears the animation queue and replaces it with a new animation
  void SetAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);

  // Queues a motion with an offset from the endpoint of that last motion
  void QueueMotion(Curve curve, float duration, sf::Vector2f offSet);
  // Queues a motion
  void QueueMotion(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end);

  // Queues a scale change
  void QueueScale(Curve curve, float duration, sf::Vector2f start, sf::Vector2f end);

  // Queues a rotation change
  void QueueRotation(Curve curve, float duration, float start, float end);

private:
  // The sprite used by the entity for rendering
  std::unique_ptr<sf::Sprite> sprite;
  
  // The shader used for rendering
  sf::Shader* shader = nullptr;

  // A pointer to an external position vector if the sprite's position has been coupled
  sf::Vector2f* position = nullptr;
  // A pointer to an external scale vector if the sprite's scale has been coupled
  sf::Vector2f* scale = nullptr;
  // A pointer to an external rotation value if the sprite's rotation has been coupled
  float* rotation = nullptr;

  // The entity's animation handler
  AnimationHandler anim;
  // The entity's motion handler, only gets used if the position isn't coupled
  MotionHandler motion;
  // The entity's scale handler, only gets used if the scale isn't coupled
  ScaleHandler scaleHand;
  // The entity's rotation handler, only gets used if the rotation isn't coupled
  RotationHandler rot;
};

#endif