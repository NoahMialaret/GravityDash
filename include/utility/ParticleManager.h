#ifndef PARTICLE_MANAGER_H
#define PARTICLE_MANAGER_H

#include "Particle.h"

#include <memory>

class ParticleManager
{
private:
  // Private constructor as only one instance should exist in the program
  ParticleManager() = default;
  // The global ParticleManager instance
  static ParticleManager* instance;
  // Deletes the ParticleManager instance
  static void Clean();

  // Updates all currently existing particles
  void UpdateParticles();
  // Renders all particles to the screen
  void RenderParticles(sf::RenderWindow* win);

public:
  // Creates and returns the global instance of ParticleManager
  static ParticleManager* GetInstance();

  // Creates a new particle to maintain
  template <typename P>
  void CreateParticle(P particle);
  
private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  std::forward_list<std::unique_ptr<Particle>> particles; // The list of currently active particles 
};

template <typename P>
inline void ParticleManager::CreateParticle(P particle)
{
  particles.push_front(std::make_unique<P>(particle));
}

#endif
