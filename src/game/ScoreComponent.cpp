#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(Game* game, Stats::StatType gameType)
  :
  GameComponent(game),
  gameType(gameType)
{
  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    if (digits.empty())
      return;

    float width = digits.front().GetTextureRectWidth();
    sf::Vector2f offset(-0.5f * width * ((float)digits.size() - 1.0f), -1.0f);
    for (auto& digit : digits)
    {
      digit.SetPositon(pos + offset);
      offset.x += width;
    }
  };

  game->Attach(World::AttachPoint::top, updatePosFunction);
}

void ScoreComponent::ProcessEvent(Event& event)
{
  switch (event.type)
  {
  case Event::Type::timerRefill:
    multiplier += 0.1f;
    if (multiplier >= 2.0f)
      multiplier = 2.0f;
    break;

  case Event::Type::playerCombo:
  {
    if (event.data.combo.count == 0)
      break;

    int comboValue = 0;
    if (event.data.combo.wasSuperJump)
    {
      // Each target is worth 1000 points when collected in a super jump
      comboValue = 1000 * multiplier * event.data.combo.count;
      Add(comboValue);
      break;
    }

    // Calculates sum(2^n), each targets value is double the previous one
    comboValue = std::pow(2, std::min(event.data.combo.count, 10)) - 1;
    
    // Doubling stops after 10 targets to prevent scores getting too large
    int excess = event.data.combo.count - 10;
    if (excess > 0)
      comboValue += excess * std::pow(2, 10);

    // Each target is worth 50 points
    comboValue *= 50 * multiplier;
    Add(comboValue);

    break;
  }

  case Event::Type::playerHit:
    // Saws take away 5000 points
    Add(-5000 * multiplier);
    break;

  case Event::Type::gameDone:
    std::cout << "Game Over! You scored " << ToInt() << " points\n";
    PUSH_EVENT(Event(Event::Type::newScore, {.newScore = {(int)gameType, (int)ToInt()}}));
    break;

  default:
    break;
  }
}

void ScoreComponent::Update()
{}

void ScoreComponent::Render(sf::RenderWindow* win) const
{
  for (auto& digit : digits)
    digit.Render(win);
}

void ScoreComponent::Add(int amount)
{
  int originalSize = digits.size();

  Number<DisplayDigit>::AddValue(digits, amount);

  if (originalSize != (int)digits.size())
    game->UpdateAttachment(World::AttachPoint::top);
}

int ScoreComponent::ToInt() const
{
  int score = 0;
  for (auto& digit : digits)
  {
    score *= 10;
    score += (int)digit;
  }
  return score;
}

ScoreComponent::DisplayDigit::DisplayDigit(int digit)
  :
  digit(std::clamp(digit, 0, 10))
{
  Utility::InitSprite(sprite, "nums_big", {11, 1}, {0.5f, 1.0f});
  UpdateTextureRect();
}

ScoreComponent::DisplayDigit& ScoreComponent::DisplayDigit::operator+=(const int& rhs)
{
  digit += rhs;
  UpdateTextureRect();
  return *this;
}

ScoreComponent::DisplayDigit& ScoreComponent::DisplayDigit::operator-=(const int& rhs)
{
  digit -= rhs;
  UpdateTextureRect();
  return *this;
}

ScoreComponent::DisplayDigit& ScoreComponent::DisplayDigit::operator=(const int& rhs)
{
  digit = rhs;
  UpdateTextureRect();
  return *this;
}

ScoreComponent::DisplayDigit::operator int() const
{
  return digit;
}

void ScoreComponent::DisplayDigit::Render(sf::RenderWindow* win) const
{
  Utility::RenderSpriteWithScale(win, sprite, &STATIC_SHADER);
}

void ScoreComponent::DisplayDigit::SetPositon(sf::Vector2f newPos)
{
  sprite.setPosition(newPos);
}

int ScoreComponent::DisplayDigit::GetTextureRectWidth() const
{
  return sprite.getTextureRect().width;
}

void ScoreComponent::DisplayDigit::UpdateTextureRect()
{
  sf::IntRect rect = sprite.getTextureRect();
  rect.left = digit * rect.width;
  sprite.setTextureRect(rect);
}
