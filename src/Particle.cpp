#include "Particle.h"

Particle::Particle(int timer)
  :
  timer(timer)
{
  entity = Entity("particles", nullptr);
}

bool Particle::HasFinished() const
{
  return timer <= 0;
}

void Particle::Update()
{
  if (timer <= 0)
  {
    return;
  }

  timer -= Clock::Delta();

  entity.Update();
}

void Particle::Render(sf::RenderWindow *win) const
{
  if (timer > 0)
  {
    entity.Render(win);
  }
}

// ============
// --- Puff ---
// ============

Puff::Puff(sf::Vector2f source, sf::Vector2f dir)
  :
  Particle(400)
{
  entity.QueueAnimation(PUFF, 100, 0);

  if (dir.y > 0)
  {
    entity.FlipY();
  }

  sf::Vector2f end;
  end.x = source.x + dir.x * ProgramSettings::gameScale * Utility::spriteDim;
  end.y = source.y + dir.y * ProgramSettings::gameScale;
  entity.QueueMotion(Curve::easeIn, 400, source, end);
}

// ============
// --- Dust ---
// ============

Dust::Dust(sf::Vector2f source, bool flip)
  :
  Particle(200)
{
  entityMirror = Entity("particles", nullptr);

  if (flip)
  {
    entity.FlipY();
    entityMirror.FlipY();
  }

  entity.QueueAnimation(DUST, 50, 0);
  entity.QueueMotion(Curve::easeIn, 200, source, source + sf::Vector2f(SCALED_DIM, 0.0f));

  entityMirror.FlipX();
  entityMirror.QueueAnimation(DUST, 50, 0);
  entityMirror.QueueMotion(Curve::easeIn, 200, source, source - sf::Vector2f(SCALED_DIM, 0.0f));
}

void Dust::Update()
{
  if (timer <= 0)
  {
    return;
  }

  Particle::Update();
  entityMirror.Update();
}

void Dust::Render(sf::RenderWindow* win) const
{
  if (timer <= 0)
  {
    return;
  }

  Particle::Render(win);
  entityMirror.Render(win);
}

// =================
// --- Explosion ---
// =================

Explosion::Explosion(sf::Vector2f source)
  :
  Particle(100)
{
  entity.QueueAnimation(EXPLOSION, 25, 0);
  entity.QueueMotion(Curve::linear, 0, source, source);
}

// =================
// --- SpeedLine ---
// =================

SpeedLine::SpeedLine(sf::Vector2f start, float speed)
  :
  Particle(400)
{
  if (speed < 0)
  {
    entity.FlipY();
  }

  entity.QueueAnimation(SPEEDLINE, 100, 0);
  entity.QueueMotion(Curve::linear, 400, start, start + speed * sf::Vector2f(0.0f, ProgramSettings::gameScale));
}
