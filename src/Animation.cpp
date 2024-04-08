#include "Animation.h"

Animation::Animation(sf::Sprite* sprite)
    :
    Animation
    (
        sprite, 
        (int)(sprite->getTexture()->getSize().y / Utility::spriteDim),
        (int)(sprite->getTexture()->getSize().x / Utility::spriteDim)
    )
{}

Animation::Animation(sf::Sprite* sprite, int numAnimations, int numFrames)
    : 
    sprite(sprite),
    frameRect(sf::IntRect(0, 0, (int)Utility::spriteDim, (int)Utility::spriteDim)),
    numAnimations(numAnimations),
    numFrames(numFrames)
{
    sprite->setTextureRect(frameRect);
}

void Animation::Update()
{
    while (nextFrameStart < CUR_TIME)
    {
        AdvanceFrame();
    }
}

void Animation::ChangeAnimation(int index, sf::Int32 frameDur, int loopAmount, int nextIndex, sf::Int32 nextDur, sf::Int32 animationHold)
{
    nextAnimationIndex = nextIndex;
    nextDuration = nextDur;

    ChangeAnimation(index, frameDur, animationHold);

    loops = loopAmount;
}

void Animation::ChangeAnimation(int index, sf::Int32 frameDur, sf::Int32 animationHold)
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

    sprite->setTextureRect(frameRect);
}
