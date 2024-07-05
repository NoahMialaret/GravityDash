#include "Particle.h"
sf::Texture Particle::tex;

Particle::Particle(Particle::Type type, sf::Vector2f vel, sf::Vector2f pos, sf::Vector2f scale)
  : 
  vel(vel)
{
  sprite.setTexture(tex);

  sprite.setOrigin(CENTRED_ORIGIN);
  sprite.setPosition(pos);
  sprite.setScale(scale);

  anim = Animation(&sprite);

  switch (type)
  {
  case Type::walkDust:
    anim.ChangeAnimation((int)type, 100);
    EndOfLifespan = 300 + CUR_TIME;
    break;

  case Type::landingImpact:
    anim.ChangeAnimation((int)type, 50);
    EndOfLifespan = 200 + CUR_TIME;
    break;

  case Type::targetExplosion:
    anim.ChangeAnimation((int)type, 25);
    EndOfLifespan = 100 + CUR_TIME;
    break;

  default:
    std::cout << "Could not determine particle type (" << (int)type << ")\n";
    hasFinished = true;
    break;
  }
}

bool Particle::HasFinished() const
{
  return hasFinished;
}

void Particle::Update()
{
  if (EndOfLifespan < CUR_TIME)
  {
    hasFinished = true;
    return;
  }

  sprite.move(vel);
  anim.Update();
}

void Particle::Render(sf::RenderWindow *win) const
{
  if (!hasFinished)
  {
    win->draw(sprite);
  }
}
