#include "Animation.hpp"

Animation::Animation(Sprite *sprite)
    : Animation(
          sprite,
          (int)(sprite->GetTexture()->GetSize().y / Utility::spriteDim),
          (int)(sprite->GetTexture()->GetSize().x / Utility::spriteDim))
{
}

Animation::Animation(Sprite *sprite, int numAnimations, int numFrames)
    : sprite(sprite),
      frameRect(0, 0, (int)Utility::spriteDim, (int)Utility::spriteDim),
      numAnimations(numAnimations),
      numFrames(numFrames)
{
  sprite->SetTextureBounds(frameRect);
}

void Animation::Update()
{
  while (nextFrameStart < CUR_TIME)
  {
    AdvanceFrame();
  }
}

void Animation::ChangeAnimation(int index, double frameDur, int loopAmount, int nextIndex, double nextDur, double animationHold)
{
  nextAnimationIndex = nextIndex;
  nextDuration = nextDur;

  ChangeAnimation(index, frameDur, animationHold);

  loops = loopAmount;
}

void Animation::ChangeAnimation(int index, double frameDur, double animationHold)
{
  curAnimationIndex = index;
  frameIndex = 0;
  frameDuration = frameDur;
  loops = ALWAYS;

  if (curAnimationIndex < 0 || curAnimationIndex >= numAnimations)
  {
    std::cout << "Warning, animation was changed to an animation outside of the range.\n";
  }

  nextFrameStart = frameDuration + animationHold + CUR_TIME;

  SetSpriteRegion();
}

void Animation::AdvanceFrame()
{
  frameIndex++;
  if (frameIndex >= numFrames)
  {
    frameIndex = 0;

    if (loops != ALWAYS)
    {
      if (loops == 0)
      {
        curAnimationIndex = nextAnimationIndex;
        frameDuration = nextDuration;
      }

      loops--;
    }
  }

  nextFrameStart += frameDuration;

  SetSpriteRegion();
}

void Animation::SetSpriteRegion()
{
  frameRect.top = curAnimationIndex * (int)Utility::spriteDim;
  frameRect.left = frameIndex * (int)Utility::spriteDim;

  sprite->SetTextureBounds(frameRect);
}
