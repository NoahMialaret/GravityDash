#include "ParticleManager.h"

ParticleManager* ParticleManager::instance = nullptr;

void ParticleManager::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

void ParticleManager::UpdateParticles()
{
  if (particles.empty())
    return;
    
  for (auto prev = particles.before_begin(), cur = particles.begin(); cur != particles.end();)
  {
    cur->get()->Update();
    if (cur->get()->HasFinished())
    {
      while (cur != particles.end() && cur->get()->HasFinished())
        cur = particles.erase_after(prev);
      continue;
    }

    prev++;
    cur++;
  }  
}

void ParticleManager::RenderParticles(sf::RenderWindow* win)
{
  for (auto cur = particles.begin(); cur != particles.end(); cur++)
    cur->get()->Render(win);
}

ParticleManager* ParticleManager::GetInstance()
{
  if (instance == nullptr)
    instance = new ParticleManager();
  return instance;
}
