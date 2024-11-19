#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(sf::Sprite* sprite, sf::Vector2i frameSize)
  : 
  AnimationHandler(
    sprite,
    (int)(sprite->getTexture()->getSize().y / frameSize.y),
    (int)(sprite->getTexture()->getSize().x / frameSize.x),
    frameSize)
{}

AnimationHandler::AnimationHandler(sf::Sprite* sprite, int numAnimations, int numFrames, sf::Vector2i frameSize)
  : 
  sprite(sprite),
  frameRect(sf::IntRect((sf::Vector2i)ZERO_VECTOR, frameSize)),
  numAnimations(numAnimations),
  numFrames(numFrames)
{
  sprite->setTextureRect(frameRect);
}

void AnimationHandler::Update()
{
  if (animations.empty())
  {
    return;
  }

  frameTimer -= Clock::Delta();
  while (frameTimer <= 0 && !animations.empty())
  {
    AdvanceFrame();
  }
}

void AnimationHandler::Clear()
{
  while (!animations.empty())
  {
    animations.pop();
  }
}

void AnimationHandler::AdvanceFrame()
{
  frameIndex++;
  if (frameIndex >= numFrames)
  {
    frameIndex = 0;

    if (animations.front().loops != ALWAYS)
    {
      if (animations.front().loops == 0)
      {
        animations.pop();
        if (animations.empty())
        {
          return;
        }
        frameTimer += animations.front().frameDuration + animations.front().hold;
        SetSpriteRegion();
        return;
      }
      
      animations.front().loops--;
    }
  }

  frameTimer += animations.front().frameDuration;

  SetSpriteRegion();
}

void AnimationHandler::SetSpriteRegion()
{
  frameRect.top = animations.front().index * (int)Utility::spriteDim;
  frameRect.left = frameIndex * (int)Utility::spriteDim;
  sprite->setTextureRect(frameRect);
}

void AnimationHandler::QueueAnimation(Animation& anim)
{
  animations.push(anim);

  if (animations.size() == 1) // This animation was pushed to an empty queue
  {
    frameTimer = anim.frameDuration + anim.hold;
    frameIndex = 0;
    SetSpriteRegion();
  }
}

void AnimationHandler::QueueAnimation(int index, int dur, int loops, int hold)
{
  Animation anim;
  anim.index = index;
  anim.frameDuration = dur;
  anim.loops = loops;
  anim.hold = hold;
  QueueAnimation(anim);
}