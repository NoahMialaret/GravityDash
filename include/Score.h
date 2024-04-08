#ifndef SCORE_H
#define SCORE_H

#define SCORE_TEX_WIDTH 7
#define SCORE_TEX_HEIGHT 8

#include <SFML/Graphics.hpp>

#include "Utility.h"

#include <string>
#include <vector>

//A class representing the accumulated score accuired during gameplay
class Score
{
public:
    Score(const char* spritePath, sf::Vector2f topRightOrigin);

    // void Update();
    void Render(sf::RenderWindow* win) const;

    void AddPoints(int value);
    std::string GetAsString() const;

private:
  void PushBackNumber(int value);
  
  void Add(unsigned int value);
  void Subtract(unsigned int value);
private:
    sf::Texture tex;
    std::vector<sf::Sprite> scoreSprites;
    std::vector<int> totalScore;

    sf::Vector2f topRightOrigin;

};

#endif