#include "Number.h"

sf::Texture Number::smallNumTex;
sf::Texture Number::bigNumTex;

Number::Number(sf::Vector2f centre, sf::Vector2i digitSize, sf::Texture* tex)
  :
  Number(0, centre, digitSize, tex)
{}

Number::Number(int startingValue, sf::Vector2f centre, sf::Vector2i digitSize, sf::Texture* tex)
  :
  centre(centre),
  digitSize(digitSize),
  tex(tex)
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

sf::Vector2f Number::GetCentre() const
{
  return centre;
}

void Number::PushBackNumber(int value)
{
  sf::IntRect texRect((value)*digitSize.x, 0, digitSize.x, digitSize.y);

  sf::Sprite temp;

  temp.setTexture(*tex);
  temp.setTextureRect(texRect);


  temp.setOrigin({(float)digitSize.x / 2.0f, (float)digitSize.y / 2.0f});
  temp.setScale(DEFAULT_SCALE);
  if (tex == &smallNumTex)
  {
    temp.setScale(0.5f * DEFAULT_SCALE);
  }

  scoreSprites.push_back(temp);
  totalScore.push_back(value);

  Recentre();
}

void Number::Recentre()
{
  float scale = (tex == &smallNumTex ? 0.5f : 1.0f);
  float xPos = centre.x + scale * (scoreSprites.size() - 1) * (float)digitSize.x * Utility::gameScale / 2.0f;
  for (auto& s : scoreSprites)
  {
    s.setPosition(sf::Vector2f(xPos, centre.y));
    xPos -= scale * digitSize.x * Utility::gameScale;
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
  Number(startingValue, centre, sf::Vector2i(7, 8), &bigNumTex)
{}

void GameScore::Update()
{
}

// ====================
// --- TargetPoints ---
// ====================

TargetPoints::TargetPoints(sf::Vector2f centre, sf::Vector2f vel)
  :
  TargetPoints(0, centre, vel)
{}

TargetPoints::TargetPoints(int startingValue, sf::Vector2f centre, sf::Vector2f vel)
  :
  Number(startingValue, centre, sf::Vector2i(5, 6), &smallNumTex),
  vel(vel)
{
  prevFrameTime = CUR_TIME;
}

void TargetPoints::Update()
{
  for (auto& s : scoreSprites)
  {
    s.move(float(CUR_TIME - prevFrameTime) * vel);
  }
  centre += float(CUR_TIME - prevFrameTime) * vel;
  prevFrameTime = CUR_TIME;
}

void TargetPoints::SetVelocity(sf::Vector2f newVel)
{
  vel = newVel;
}

// ===================
// --- TotalPoints ---
// ===================

TotalPoints::TotalPoints(std::forward_list<TargetPoints> targetPoints)
  :
  Number(0, ZERO_VECTOR, sf::Vector2i(5, 6), &smallNumTex),
  targetPoints(targetPoints)
{

  sf::Vector2f accumulatedPosition = ZERO_VECTOR;
  int numPoints = 0;
  int accumulatedPoints = 0;
  
  for (auto& point : this->targetPoints)
  {
    accumulatedPosition += point.GetCentre();
    numPoints++;
    accumulatedPoints += point.GetAsInt();

    point.SetVelocity(ZERO_VECTOR);
  }

  centre = (1.0f / (float)numPoints) * accumulatedPosition;
  Recentre();
  AddPoints(accumulatedPoints);

  prevIndex = CUR_TIME / 150 % scoreSprites.size();
  scoreSprites[prevIndex].move(sf::Vector2f(0.0f, -0.5f * Utility::gameScale));

  creationTime = CUR_TIME;
}

void TotalPoints::Update()
{
  switch (curState)
  {
  case State::start:
    for (auto& point : targetPoints)
    {
      point.Update();
    }
    if (creationTime + 500 < CUR_TIME)
    {
      for (auto& point : targetPoints)
      {
        point.SetVelocity(1.0f / 500.0f * (centre - point.GetCentre()));
      }
      curState = State::accumulate;
    }
    break;

  case State::accumulate:
    for (auto& point : targetPoints)
    {
      point.Update();
    }
    if (creationTime + 1000 < CUR_TIME)
    {
      curState = State::total;
    }
    break;

  case State::total:
    for (auto& s : scoreSprites)
    {
      s.move(sf::Vector2f(0.0f, -Utility::gameScale / 10));
    }
    if (creationTime + 2000 < CUR_TIME)
    {
      curState = State::finish;
    }
    break;
  
  default:
    break;
  }

  int index = CUR_TIME / 150 % scoreSprites.size();

  if (index == prevIndex)
  {
    return;
  }

  scoreSprites[prevIndex].move(sf::Vector2f(0.0f, 0.5f * Utility::gameScale));
  scoreSprites[index].move(sf::Vector2f(0.0f, - 0.5f * Utility::gameScale));

  prevIndex = index;
}

void TotalPoints::Render(sf::RenderWindow *win) const
{
  switch (curState)
  {
  case State::start:
  case State::accumulate:
    for (auto& point : targetPoints)
    {
      point.Render(win);
    }
    break;

  case State::total:
    Number::Render(win);
    break;
  
  default:
    break;
  }
}

bool TotalPoints::HasFinished()
{
  return curState == State::finish;
}
