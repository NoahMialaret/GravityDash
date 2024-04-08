#include "Score.h"

Score::Score(const char* spritePath, sf::Vector2f topRightOrigin)
    :
    topRightOrigin(topRightOrigin)
{
    if (!tex.loadFromFile(spritePath)) 
    {
        std::cout << "Score texture could not be loaded!\n";
    }

  PushBackNumber(0);
}

void Score::Render(sf::RenderWindow* win) const
{
  for (auto &digits : scoreSprites)
  {
    win->draw(digits);
  }
}

void Score::AddPoints(int value)
{
  if (value > 0)
  {
    Add(value);
  }
  else if (value < 0)
  {
    Subtract(abs(value));
  }
}

std::string Score::GetAsString() const
{
  std::string num;

  for (int i = totalScore.size() - 1; i >= 0; i--)
  {
    num.append(Utility::IntToString(totalScore[i]));
  }

  return num;
}

void Score::PushBackNumber(int value)
{
  sf::IntRect texRect((value)*SCORE_TEX_WIDTH, 0, SCORE_TEX_WIDTH, SCORE_TEX_HEIGHT);

  sf::Sprite temp;

  temp.setTexture(tex);
  temp.setTextureRect(texRect);

  sf::Vector2f pos = topRightOrigin;
  pos.x -= scoreSprites.size() * SCORE_TEX_WIDTH * Utility::gameScale;

  temp.setOrigin({SCORE_TEX_WIDTH, 0.0f});
  temp.setPosition(pos);
  temp.setScale(DEFAULT_SCALE);

  scoreSprites.push_back(temp);
  totalScore.push_back(value);
}

void Score::Add(unsigned int value)
{
  bool carry = false;
  for (int i = 0; i < scoreSprites.size() && (value != 0 || carry); i++, value /= 10)
  {
    int digit = value % 10;

    sf::IntRect texRect = scoreSprites[i].getTextureRect();
    texRect.left += (digit + carry) * SCORE_TEX_WIDTH;
    totalScore[i] += digit + carry;

    carry = false;

    if (texRect.left > 9 * SCORE_TEX_WIDTH)
    {
      texRect.left -= 10 * SCORE_TEX_WIDTH;
      totalScore[i] -= 10;
      carry = true;
    }
    scoreSprites[i].setTextureRect(texRect);
  }

  while (value != 0 || carry)
  {
    int digit = value % 10;

    int pushBackNum = digit + carry;

    carry = false;
    if (pushBackNum > 9)
    {
      pushBackNum -= 10;
      carry = true;
    }

    PushBackNumber(pushBackNum);

    value /= 10;
  }
}

void Score::Subtract(unsigned int value)
{
  bool carry = false;
  
  for (int i = 0; i < scoreSprites.size() && (value != 0 || carry); i++, value /= 10)
  {
    int digit = value % 10;

    sf::IntRect texRect = scoreSprites[i].getTextureRect();
    texRect.left -= (digit + carry) * SCORE_TEX_WIDTH;
    totalScore[i] -= digit + carry;

    carry = false;

    if (texRect.left < 0 * SCORE_TEX_WIDTH)
    {
      if (i == scoreSprites.size() - 1)
      {
        totalScore.clear();
        scoreSprites.clear();
        PushBackNumber(0);
        return;
      }
      texRect.left += 10 * SCORE_TEX_WIDTH;
      totalScore[i] += 10;
      carry = true;
    }
    scoreSprites[i].setTextureRect(texRect);
  }

  if (value != 0)
  {
        totalScore.clear();
        scoreSprites.clear();
        PushBackNumber(0);
  }

  // while (value != 0 || carry)
  // {
  //   int digit = value % 10;

  //   sf::IntRect texRect((digit + carry) * SCORE_TEX_WIDTH, 0, SCORE_TEX_WIDTH, SCORE_TEX_HEIGHT);
  //   totalScore.push_back(digit + carry);

  //   carry = false;

  //   if (texRect.left > 9 * SCORE_TEX_WIDTH)
  //   {
  //     texRect.left -= 10 * SCORE_TEX_WIDTH;
  //     totalScore[totalScore.size() - 1] -= 10;
  //     carry = true;
  //   }

  //   sf::Sprite temp;

  //   temp.setTexture(tex);
  //   temp.setTextureRect(texRect);

  //   sf::Vector2f pos = topRightOrigin;
  //   pos.x -= scoreSprites.size() * SCORE_TEX_WIDTH * Utility::gameScale;

  //   temp.setOrigin({SCORE_TEX_WIDTH, 0.0f});
  //   temp.setPosition(pos);
  //   temp.setScale(DEFAULT_SCALE);

  //   scoreSprites.push_back(temp);

  //   value /= 10;
  // }
}
