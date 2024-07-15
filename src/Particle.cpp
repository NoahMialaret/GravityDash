#include "Particle.h"

Particle::Particle(Particle::Type type, sf::Vector2f vel, sf::Vector2f pos, sf::Vector2f scale)
  : 
  vel(vel)
{
  sprite.setTexture(Textures::textures.at("particles"));

  sprite.setOrigin(CENTRED_ORIGIN);
  sprite.setPosition(pos);
  sprite.setScale(scale);

  anims = AnimationHandler(&sprite);

  switch (type)
  {
  case Type::walkDust:
    anims.QueueAnimation((int)type, 100, 0);
    timer = 400;
    break;

  case Type::landingImpact:
    anims.QueueAnimation((int)type, 50, 0);
    timer = 200;
    break;

  case Type::targetExplosion:
    anims.QueueAnimation((int)type, 25, 0);
    timer = 100;
    break;
  
  case Type::speedLines:
    anims.QueueAnimation((int)type, 100, 0);
    timer = 400;
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
  if (timer <= 0)
  {
    hasFinished = true;
    return;
  }

  timer -= Clock::Delta();

  sprite.move((Clock::Delta() / 16.0f) * vel);

  anims.Update(&sprite);
}

void Particle::Render(sf::RenderWindow *win) const
{
  if (!hasFinished)
  {
    win->draw(sprite);
  }
}
