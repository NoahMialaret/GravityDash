#include "Particle.hpp"
Particle* Particle::particles = nullptr;
Texture Particle::tex;


Particle::Particle(Particle::Type type, fVec2 vel, fVec2 pos, fVec2 scale)
    :
    vel(vel)
{
    sprite.SetTexture(&tex);

    sprite.SetOrigin(CENTRED_ORIGIN);
    sprite.SetTranslation(pos);
    sprite.SetScale(scale);

    anim = Animation(&sprite);

    switch (type)
    {
        case Type::walkDust:
            anim.ChangeAnimation((int)type, 0.100);
            EndOfLifespan = 0.300 + CUR_TIME;
            break;

        case Type::landingImpact:
            anim.ChangeAnimation((int)type, 0.050);
            EndOfLifespan = 0.200 + CUR_TIME;
            break;
            
        case Type::targetExplosion:
            anim.ChangeAnimation((int)type, 0.025);
            EndOfLifespan = 0.100 + CUR_TIME;
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

void Particle::RenderParticles()
{
    if (particles != nullptr)
    {
        particles->Render();
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

    sprite.Translate(vel);
    anim.Update();
}

void Particle::Render() const
{
    sprite.Render();

    if (nextParticle != nullptr)
    {
        nextParticle->Render();
    }
}
