#include "AnimationHandler.h"

AnimationHandler::AnimationHandler(sf::Sprite* sprite)
  : 
  AnimationHandler(
    sprite,
    (int)(sprite->getTexture()->getSize().y / Utility::spriteDim),
    (int)(sprite->getTexture()->getSize().x / Utility::spriteDim))
{}

AnimationHandler::AnimationHandler(sf::Sprite* sprite, int numAnimations, int numFrames)
  : 
  sprite(sprite),
  frameRect(sf::IntRect(0, 0, (int)Utility::spriteDim, (int)Utility::spriteDim)),
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

  while (nextFrameStart < CUR_TIME)
  {
    AdvanceFrame();
  }
}

void AnimationHandler::Update(sf::Sprite* sprite)
{
  this->sprite = sprite;
  Update();
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
        nextFrameStart += animations.front().frameDuration + animations.front().hold;
        SetSpriteRegion();
        return;
      }
      
      animations.front().loops--;
    }
  }

  nextFrameStart += animations.front().frameDuration;

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
    nextFrameStart = anim.frameDuration + anim.hold + CUR_TIME;
    frameIndex = 0;
    SetSpriteRegion();
  }
}

void AnimationHandler::QueueAnimation(int index, sf::Int32 dur, int loops, sf::Int32 hold)
{
  Animation anim;
  anim.index = index;
  anim.frameDuration = dur;
  anim.loops = loops;
  anim.hold = hold;
  QueueAnimation(anim);
}