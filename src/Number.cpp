#include "Number.h"

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
    Add(abs(startingValue));
    if (startingValue < 0)
    {
      PushBackNumber(NEGATIVE_SIGN);
      isNegative = true;
    }
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
  if (value == 0)
    return;

  if (isNegative)
    scoreSprites.pop_back();

  int curVal = GetAsInt();

  // If the numbers are the same magnitude but different signs
  if (-value == curVal)
  {
    Zero();
    return;
  }
  
  if (value > 0)
  {
    if (isNegative)
    {
      if (value >= -curVal)
      {
        scoreSprites.clear();
        totalScore.clear();
        isNegative = false;
        Add(value + curVal);
        return;
      }
      Subtract(value);
      return;
    }
    Add(value);
  }
  else if (value < 0)
  {
    if (!isNegative)
    {
      if (-value >= curVal)
      {
        scoreSprites.clear();
        totalScore.clear();
        isNegative = true;
        Add(- value - curVal);
        return;
      }
      Subtract(-value);
      return;
    }
    Add(-value);
  }
}

void Number::Zero()
{
  scoreSprites.clear();
  totalScore.clear();
  isNegative = false;
  PushBackNumber(0);
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
  return (isNegative ? -1 : 1) * num;
}

std::string Number::GetAsString() const
{
  std::string num;

  if (isNegative)
  {
    num = "-";
  }

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
  if (tex == &Textures::textures.at("nums_small"))
  {
    temp.scale(sf::Vector2f(0.5f, 0.5f));
  }

  scoreSprites.push_back(temp);

  if (value != NEGATIVE_SIGN)
  {
    totalScore.push_back(value);
  }

  Recentre();
}

void Number::Recentre()
{
  float scale = (tex == &Textures::textures.at("nums_small") ? 0.5f : 1.0f);
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

  if (isNegative)
    PushBackNumber(NEGATIVE_SIGN);
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
    return;
  }

  while (totalScore[totalScore.size() - 1] == 0)
  {
    totalScore.pop_back();
    scoreSprites.pop_back();
  }

  if (isNegative)
    PushBackNumber(NEGATIVE_SIGN);
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
  Number(startingValue, centre, sf::Vector2i(5, 6), &Textures::textures.at("nums_small")),
  vel(vel)
{}

void TargetPoints::Update()
{
  timer -= Clock::Delta();
  if (spawnVelocity && timer <= 0)
  {
    vel = ZERO_VECTOR;
  }
  
  for (auto& s : scoreSprites)
  {
    s.move(float(Clock::Delta()) * vel);
  }
  centre += float(Clock::Delta()) * vel;
}

void TargetPoints::SetVelocity(sf::Vector2f newVel)
{
  vel = newVel;
  spawnVelocity = false;
}

// ===================
// --- TotalPoints ---
// ===================

TotalPoints::TotalPoints(std::forward_list<TargetPoints> targetPoints)
  :
  Number(0, ZERO_VECTOR, sf::Vector2i(5, 6), &Textures::textures.at("nums_small")),
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

  prevIndex = Clock::Elapsed() / 150 % scoreSprites.size();
  scoreSprites[prevIndex].move(sf::Vector2f(0.0f, -0.5f * Utility::gameScale));

  timer = 500;
}

void TotalPoints::Update()
{
  timer -= Clock::Delta();
  switch (curState)
  {
  case State::start:
    for (auto& point : targetPoints)
    {
      point.Update();
    }
    if (timer <= 0)
    {
      for (auto& point : targetPoints)
      {
        point.SetVelocity(1.0f / 250.0f * (centre - point.GetCentre()));
      }
      timer = 250;
      curState = State::accumulate;
    }
    break;

  case State::accumulate:
    for (auto& point : targetPoints)
    {
      point.Update();
    }
    if (timer <= 0)
    {
      timer = 1000;
      curState = State::total;
    }
    break;

  case State::total:
    for (auto& s : scoreSprites)
    {
      s.move((Clock::Delta() / 16.0f) * sf::Vector2f(0.0f, -Utility::gameScale / 10));
    }
    if (timer <= 0)
    {
      curState = State::finish;
    }
    break;
  
  default:
    break;
  }

  int index = Clock::Elapsed() / 150 % scoreSprites.size();

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

// =================
// --- GameScore ---
// =================

GameScore::GameScore(sf::Vector2f centre)
  :
  GameScore(0, centre)
{}

GameScore::GameScore(int startingValue, sf::Vector2f centre)
  :
  Number(startingValue, centre, sf::Vector2i(7, 8), &Textures::textures.at("nums_big"))
{}

void GameScore::Update()
{
  for (auto& point : totalPoints)
  {
    point.Update();
  }
  while (totalPoints.front().HasFinished())
  {
    totalPoints.pop_front();
  }
}

void GameScore::Render(sf::RenderWindow *win) const
{
  for (auto& point : totalPoints)
  {
    point.Render(win);
  }
  Number::Render(win);
}

void GameScore::AddTargetPoints(std::forward_list<TargetPoints>& target)
{
  if (target.empty())
  {
    return;
  }
  
  totalPoints.push_back(target);

  AddPoints(totalPoints.back().GetAsInt());

  if (GetAsInt() < 0)
  {
    Zero();
  }
}