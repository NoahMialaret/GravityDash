#ifndef PARTICLE_H
#define PARTICLE_H

#include "Animation.h"
#include "Utility.h"

#include <iostream>

// A class used to represent a linked list of short-lived particle affects
class Particle
{
public:
    // The type of particle, used to determine animation attributes and lifespan
    enum class Type
    {
        walkDust,
        landingImpact,
        targetExplosion
    };
    
public:
    Particle() = delete;
    // Constructor that takes the type of particle that should be spawned, as well as its position, velocity, and (optionally) scale
    Particle(Type type, sf::Vector2f vel, sf::Vector2f pos, sf::Vector2f scale = DEFAULT_SCALE);
    // Frees up dynamic memory by deleting the particle once the particles after this have also been deleted
    void Delete();

    // Returns whether the particle has reached the end of its lifespan
    bool HasFinished() const;
    // Returns a pointer to the next particle in the list
    Particle* GetNextParticle() const;

    // Appends a new particle at the end of the list
    static void CreateNewParticle(Particle* particle);
    // Function to be called outside this class to call Delete() for all particles
    static void DeleteParticles();
    // Function to be called outside this class to call Update() for all particles
    static void UpdateParticles();
    // Function to be called outside this class to call Render() for all particles
    static void RenderParticles(sf::RenderWindow* win);

private:
    // Updates this and the next in line particle's position and animation, and determines whether it has reached the end of its lifespan
    void Update();
    // Renders this and the next in line particle's current sprite
    void Render(sf::RenderWindow* win) const;

public:
    static Particle* particles; // A pointer to a particle representing the start of the list
    static sf::Texture tex;     // The texture used by all the particles

private:
    Particle* nextParticle = nullptr;   // A pointer to the next particle in the list

    sf::Sprite sprite;  // The particle's sprite to be used for rendering
    Animation anim;     // The particle's animation handler
    sf::Vector2f vel;   // The particle's velocity

    bool hasFinished = false;   // Whether or not the particle has reached the end of its lifespan
    sf::Int32 EndOfLifespan;    // The global time at which the particle will reach the end of its lifespan
};

#endif