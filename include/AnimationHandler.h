#ifndef ANIMATION_H
#define ANIMATION_H

#define ALWAYS -1

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Utility.h"

#include <iostream>
#include <queue>

struct Animation
{
  int index = 0;
  int frameDuration = 0; // How long an individual frame of animation lasts (in milliseconds)
  int loops = ALWAYS;    // How many times the animation should loop, -1 for always loop
  int hold = 0;          // How long an animation waits before starting
};

// Animation class handles the textureRects of sprites in a parent class to display animations
class AnimationHandler
{
public:
  AnimationHandler() = default;
  // Constructor that takes a sprite to determine
  // the number of animations and frames based on the sprite's texture
  AnimationHandler(sf::Sprite* sprite);
  // Constructor that explicitly sets how many animations it has and the number of frames in each animation
  AnimationHandler(sf::Sprite* sprite, int numAnimations, int numFrames);

  // Handles animation changes if enough time has passed
  void Update();
  void Update(sf::Sprite* sprite);

  // Clears the queue of all animations
  void Clear();
  // Pushes an animations to the current animation queue
  void QueueAnimation(Animation& anim);
  void QueueAnimation(int index, int dur, int loops = ALWAYS, int hold = 0);

private:
  // Advances the animation to the next frame
  void AdvanceFrame();
  // Updates the sprites textureRect with the current frame
  void SetSpriteRegion();

private:
  sf::Sprite* sprite;
  std::queue<Animation> animations; // A queue of animations, an animation is dequeued if it runs out of loops
  sf::IntRect frameRect;  // A rectangle representing what part of the texture the current frame is at

  int numAnimations = 0;  // The maximum number of animations a sprite has
  int numFrames = 0;      // The maximum number of frames in an animation

  int frameIndex = 0; // The current frame of animation
  int frameTimer = 0; // The amount of time before advancing the frame
};

#endif