#ifndef SCORE_HPP
#define SCORE_HPP

#define SCORE_TEX_WIDTH 7
#define SCORE_TEX_HEIGHT 8

#include "Sprite.hpp"
#include "Texture.hpp"
#include "Utility.hpp"
#include "Vec2.hpp"

#include <string>
#include <vector>

//A class representing the accumulated score accuired during gameplay
class Score
{
public:
  Score(const char* spritePath, fVec2 topRightOrigin);

  // void Update();
  void Render() const;

  void AddPoints(int value);
  std::string GetAsString() const;

private:
  void PushBackNumber(int value);

  void Add(unsigned int value);
  void Subtract(unsigned int value);

private:
  Texture tex;
  std::vector<Sprite> scoreSprites;
  std::vector<int> totalScore;

  fVec2 topRightOrigin;
};

#endif