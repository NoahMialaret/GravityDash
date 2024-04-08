#include "Particle.h"
Particle* Particle::particles = nullptr;
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
            EndOfLifespan = CUR_TIME;
            break;
    }
}

void Particle::Delete()
{
    if (nextParticle != nullptr)
    {
        nextParticle->Delete();
        delete nextParticle;
    }
}

bool Particle::HasFinished() const
{
    return hasFinished;
}

Particle* Particle::GetNextParticle() const
{
    return nextParticle;
}

// ----------------
// Static functions
// ----------------

void Particle::CreateNewParticle(Particle* particle)
{
    if (particles == nullptr)
    {
        particles = particle;
        return;
    }

    Particle* endOfList = particles;

    while(true)
    {
        if (endOfList->nextParticle == nullptr)
        {
            endOfList->nextParticle = particle;
            break;
        }

        endOfList = endOfList->nextParticle;
    }
}

void Particle::DeleteParticles()
{
    if (particles != nullptr)
    {
        particles->Delete();
        delete particles;
        particles = nullptr;
    }
}

void Particle::UpdateParticles()
{
	if (particles == nullptr)
	{
        return;
    }

    particles->Update();

    if (particles->HasFinished())
    {
        Particle* temp = particles->GetNextParticle();
        delete particles;
        particles = temp;
    }
}

void Particle::RenderParticles(sf::RenderWindow* win)
{
    if (particles != nullptr)
    {
        particles->Render(win);
    }
}

// -----------------
// Private Functions
// -----------------

void Particle::Update()
{
    if (nextParticle != nullptr)
    {
        nextParticle->Update();

        if (nextParticle->HasFinished())
        {
            Particle* temp = nextParticle->GetNextParticle();
            delete nextParticle;
            nextParticle = temp;
        }
    }

    if (EndOfLifespan < CUR_TIME)
    {
        hasFinished = true;
        return;
    }

    sprite.move(vel);
    anim.Update();
}

void Particle::Render(sf::RenderWindow* win) const
{
    win->draw(sprite);

    if (nextParticle != nullptr)
    {
        nextParticle->Render(win);
    }
}
