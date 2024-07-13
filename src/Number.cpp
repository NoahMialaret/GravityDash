#include "Number.h"

Number::Number(sf::Vector2f centre, sf::Vector2i digitSize)
  :
  Number(0, centre, digitSize)
{}

Number::Number(int startingValue, sf::Vector2f centre, sf::Vector2i digitSize)
  :
  centre(centre),
  digitSize(digitSize)
{
  if (startingValue == 0)
  {
    PushBackNumber(0);
  }
  else
  {
    Add(startingValue);
  }
}

void Number::Render(sf::RenderWindow* win) const
{
  for (auto &digits : scoreSprites)
  {
    win->draw(digits);
  }
}

void Number::AddPoints(int value)
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

int Number::GetAsInt() const
{
  int num = 0;
  int place = 1;
  for (auto& n : totalScore)
  {
    num += n * place;
    place *= 10;
  }
  return num;
}

std::string Number::GetAsString() const
{
  std::string num;

  for (int i = totalScore.size() - 1; i >= 0; i--)
  {
    num.append(Utility::IntToString(totalScore[i]));
  }

  return num;
}

void Number::PushBackNumber(int value)
{
  sf::IntRect texRect((value)*digitSize.x, 0, digitSize.x, digitSize.y);

  sf::Sprite temp;

  temp.setTexture(tex);
  temp.setTextureRect(texRect);

  temp.setOrigin({(float)digitSize.x / 2.0f, (float)digitSize.y / 2.0f});
  temp.setScale(DEFAULT_SCALE);

  scoreSprites.push_back(temp);
  totalScore.push_back(value);

  Recentre();
}

void Number::Recentre()
{
  float xPos = centre.x + (scoreSprites.size() - 1) * (float)digitSize.x * Utility::gameScale / 2.0f;
  for (auto& s : scoreSprites)
  {
    s.setPosition(sf::Vector2f(xPos, centre.y));
    xPos -= digitSize.x * Utility::gameScale;
  }
}

void Number::Add(unsigned int value)
{
  bool carry = false;
  for (int i = 0; i < scoreSprites.size() && (value != 0 || carry); i++, value /= 10)
  {
    int digit = value % 10;

    sf::IntRect texRect = scoreSprites[i].getTextureRect();
    texRect.left += (digit + carry) * digitSize.x;
    totalScore[i] += digit + carry;

    carry = false;

    if (texRect.left > 9 * digitSize.x)
    {
      texRect.left -= 10 * digitSize.x;
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

void Number::Subtract(unsigned int value)
{
  bool carry = false;
  
  for (int i = 0; i < scoreSprites.size() && (value != 0 || carry); i++, value /= 10)
  {
    int digit = value % 10;

    sf::IntRect texRect = scoreSprites[i].getTextureRect();
    texRect.left -= (digit + carry) * digitSize.x;
    totalScore[i] -= digit + carry;

    carry = false;

    if (texRect.left < 0 * digitSize.x)
    {
      if (i == scoreSprites.size() - 1)
      {
        totalScore.clear();
        scoreSprites.clear();
        PushBackNumber(0);
        return;
      }
      texRect.left += 10 * digitSize.x;
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
}

// =================
// --- GameScore ---
// =================

GameScore::GameScore(sf::Vector2f centre)
  :
  GameScore(0, centre)
{}

GameScore::GameScore(int startingValue, sf::Vector2f centre)
  :
  Number(startingValue, centre, sf::Vector2i(7, 8))
{
  if (!tex.loadFromFile("assets/nums_big.png")) 
  {
    std::cout << "\tScore texture could not be loaded!\n";
  }
}

void GameScore::Update()
{
}

PlayerPoints::PlayerPoints(sf::Vector2f centre, sf::Vector2f vel)
  :
  PlayerPoints(0, centre, vel)
{}

PlayerPoints::PlayerPoints(int startingValue, sf::Vector2f centre, sf::Vector2f vel)
  :
  Number(startingValue, centre, sf::Vector2i(5, 6)),
  vel(vel)
{
  if (!tex.loadFromFile("assets/nums_small.png")) 
  {
    std::cout << "\tScore texture could not be loaded!\n";
  }
}

void PlayerPoints::Update()
{
}
