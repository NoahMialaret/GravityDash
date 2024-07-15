#include "Clock.h"

sf::Int32 Clock::lastFrameTime = 0;
float Clock::clockSpeed = 1.0f;
sf::Clock Clock::clock;
int Clock::deltaTime = 0;

void Clock::Init()
{
  lastFrameTime = clock.getElapsedTime().asMilliseconds();
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
