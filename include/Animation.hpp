#ifndef ANIMATION_H
#define ANIMATION_H

#define ALWAYS -1

#include "Rect.hpp"
#include "Sprite.hpp"
#include "Utility.hpp"

#include <iostream>

// Animation class handles the textureRects of sprites in a parent class to display animations
class Animation
{
public:
  Animation() = default;
  // Constructor that only takes a pointer to the sprite, determines
  // the number of animations and frames based on the sprite's texture
  Animation(Sprite* sprite);
  // Constructor that takes a pointer to a sprite, as well as explicitly how
  // many animations it has and the number of frames in each animation
  Animation(Sprite* sprite, int numAnimations, int numFrames);

  // Handles animation changes if enough time has passed
  void Update();
  // Changes the animation of the sprite, as well as the next animation if wanted
  void ChangeAnimation(int index, double frameDur, int loopAmount,
                       int nextIndex, double nextDur, double animationHold = 0);
  void ChangeAnimation(int index, double frameDur, double animationHold = 0);

private:
  // Advances the animation to the next frame, will change
  void AdvanceFrame();
  // Updates the sprites textureRect with the current frame
  void SetSpriteRegion();

private:
  Sprite* sprite = nullptr; // The sprite of the parent object, only textureRect is changed
  IRect frameRect;          // A rectangle representing what part of the texture the current frame is at

  int numAnimations = 0;      // The maximum number of animations a sprite has
  int curAnimationIndex = 0;  // The current animation
  int nextAnimationIndex = 0; // The animation to be played after the current animation if 'loops' is finite

  int numFrames = 0;  // The maximum number of frames in an animation
  int frameIndex = 0; // The current frame of animation

  double nextFrameStart = 0;  // The time at which to advance the animation frame
  double frameDuration = 100; // How long an individual frame of animation lasts (in milliseconds)
  double nextDuration = 100;  // Individual frame length for the animation after the current

  int loops = ALWAYS; // How many times the animation should loop, -1 for always loop
};

#endif