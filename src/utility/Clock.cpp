#include "Clock.h"

Clock* Clock::instance = nullptr;

Clock* Clock::GetInstance()
{
  if (instance == nullptr)
  {
    instance = new Clock();
    instance->lastFrameTime = instance->clock.getElapsedTime().asMilliseconds();
  }
  return instance;
}

void Clock::Clean()
{
  if (instance != nullptr)
    delete instance;
}

void Clock::Update()
{
  deltaTime = clockSpeed * float(clock.getElapsedTime().asMilliseconds() - lastFrameTime);
  lastFrameTime = clock.getElapsedTime().asMilliseconds();
}

int Clock::Delta()
{
  return deltaTime;
}

sf::Int32 Clock::Elapsed()
{
  return clock.getElapsedTime().asMilliseconds();
}

void Clock::SetSpeed(float speed)
{
  clockSpeed = speed;
}
