#include "Particle.h"

Particle::Particle(int timer, int colID)
  :
  entity("particles", {4, 4}),
  timer(timer),
  colID(colID)
{}

bool Particle::HasFinished() const
{
  return timer <= 0;
}

void Particle::Update()
{
  if (timer <= 0)
    return;

  timer -= DELTA_TIME;

  entity.Update();
}

void Particle::Render(sf::RenderWindow *win) const
{  
  ENTITY_SHADER.setUniform("colorID", colID);
  if (timer > 0)
    entity.Render(win);
}

// ============
// --- Puff ---
// ============

Puff::Puff(sf::Vector2f source, sf::Vector2f dir, int colID)
  :
  Particle(400, colID)
{
  entity.PushAnimation(PUFF, 100, 0);

  if (dir.y > 0)
    entity.FlipY();

  sf::Vector2f end;
  end.x = source.x + dir.x * SPRITE_DIM;
  end.y = source.y + dir.y;
  entity.PushPositionTransition(EASE_IN_CURVE, 400, source, end);
}

// ============
// --- Dust ---
// ============

Dust::Dust(sf::Vector2f source, bool flip, int colID)
  :
  Particle(200, colID),
  entityMirror("particles", {4, 4})
{
  if (flip)
  {
    entity.FlipY();
    entityMirror.FlipY();
  }

  entity.PushAnimation(DUST, 50, 0);
  entity.PushPositionTransition(EASE_IN_CURVE, 200, source, source + sf::Vector2f(SPRITE_DIM, 0.0f));

  entityMirror.FlipX();
  entityMirror.PushAnimation(DUST, 50, 0);
  entityMirror.PushPositionTransition(EASE_IN_CURVE, 200, source, source - sf::Vector2f(SPRITE_DIM, 0.0f));
}

void Dust::Update()
{
  if (timer <= 0)
    return;

  Particle::Update();
  entityMirror.Update();
}

void Dust::Render(sf::RenderWindow* win) const
{
  if (timer <= 0)
    return;

  Particle::Render(win);
  entityMirror.Render(win);
}

// =================
// --- Explosion ---
// =================

Explosion::Explosion(sf::Vector2f source, int colID)
  :
  Particle(100, colID)
{
  entity.PushAnimation(EXPLOSION, 25, 0);
  entity.PushPositionTransition(LINEAR_CURVE, 0, source, source);
}

// =================
// --- SpeedLine ---
// =================

SpeedLine::SpeedLine(sf::Vector2f start, float speed, int colID)
  :
  Particle(400, colID)
{
  if (speed < 0)
    entity.FlipY();

  entity.PushAnimation(SPEEDLINE, 100, 0);
  entity.PushPositionTransition(LINEAR_CURVE, 400, start, start + speed * sf::Vector2f(0.0f, 1.0f));
}
