#include "ScoreComponent.h"

ScoreComponent::ScoreComponent(Game* game)
  :
  GameComponent(game)
{
  std::function<void(sf::Vector2f)> updatePosFunction = [this](sf::Vector2f pos)
  {
    if (digits.empty())
      return;

    float width = ProgramSettings::gameScale * digits.front().GetTextureRectWidth();
    sf::Vector2f offset(-0.5f * width * ((float)digits.size() - 1.0f), -ProgramSettings::gameScale);
    for (auto& digit : digits)
    {
      digit.SetPositon(pos + offset);
      offset.x += width;
    }
  };

  game->Attach(World::AttachPoint::top, updatePosFunction);
}

void ScoreComponent::ProcessEvents(Event& event)
{
  switch (event.type)
  {
  case Event::Type::playerLand:
  {
    int originalSize = digits.size();

    Number<DisplayDigit>::AddValue(digits, 7);

    if (originalSize != digits.size())
      game->UpdateAttachment(World::AttachPoint::top);

    break;
  }
  
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

ScoreComponent::DisplayDigit::DisplayDigit(int digit)
  :
  digit(digit)
{
  Utility::InitSprite(sprite, "nums_big", ZERO_VECTOR, {11, 1}, {0.5f, 1.0f});
  UpdateTextureRect();
}

ScoreComponent::DisplayDigit& ScoreComponent::DisplayDigit::operator+=(const int &rhs)
{
  digit += rhs;
  UpdateTextureRect();
  return *this;
}

ScoreComponent::DisplayDigit& ScoreComponent::DisplayDigit::operator-=(const int &rhs)
{
  digit -= rhs;
  UpdateTextureRect();
  return *this;
}

ScoreComponent::DisplayDigit::operator int() const
{
  return digit;
}

void ScoreComponent::DisplayDigit::Render(sf::RenderWindow* win) const
{
  win->draw(sprite);
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
