#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include <SFML/Graphics.hpp>

class ScoreComponent
{
public:
  ScoreComponent();
  ~ScoreComponent();

  void Update() = 0;
  void Render(sf::RenderWindow* win) const;

  void 
};

class SharedScoreComponent : public ScoreComponent
{

};

class IndividualScoreComponent : public ScoreComponent
{

};

#endif