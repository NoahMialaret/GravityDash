#include "Utility.h"

Utility* Utility::instance = nullptr;

Utility* Utility::GetInstance()
{
  if (instance == nullptr)
  {
    instance = new Utility();
    std::mt19937(std::random_device()());
  }

  return instance;
}

void Utility::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

int Utility::GetSpriteDim() const
{
  return spriteDim;
}

std::mt19937& Utility::GetRNG()
{
  return rng;
}

int Utility::GetSign(int num)
{
	if (num > 0)
		return 1;
	else if (num < 0)
		return -1;
  return 0;
}

int Utility::GetSign(float num)
{
	if (num > 0.0f)
		return 1;
	else if (num < 0.0f)
		return -1;
  return 0;
}

float Utility::GetSquaredDistanceToLineSegment(sf::Vector2f point, LineSegment line)
{
  // If the both points in the line segment are bascially identical, treat them as the same point
  if (abs(line.start.x - line.end.x) < 1.0f 
      && abs(line.start.y - line.end.y) < 1.0f)
  {
    return std::pow(line.start.x - point.x, 2.0f) 
           + std::pow(line.start.y - point.y, 2.0f);
  }

	sf::Vector2f lineSeg = line.end - line.start;
	sf::Vector2f pointToStart = point - line.start;
	sf::Vector2f pointToEnd = point - line.end;

	float startDotProd = lineSeg.x * pointToStart.x + lineSeg.y * pointToStart.y;
	float endDotProd = lineSeg.x * pointToEnd.x + lineSeg.y * pointToEnd.y;

	float distanceSquared = 0.0f;

	if (endDotProd > 0.0f) // Projection of dot products are in the same direction
	{
		distanceSquared = std::pow(line.end.x - point.x, 2.0f) 
			+ std::pow(line.end.y - point.y, 2.0f);
	}
	else if (startDotProd < 0.0f) // Projection of dot products are in the opposite direction
	{
		distanceSquared = std::pow(line.start.x - point.x, 2.0f) 
			+ std::pow(line.start.y - point.y, 2.0f);
	}
	else // Projection of dot products are in different directions
	{
		float lineSegDistSquared = lineSeg.x * lineSeg.x + lineSeg.y * lineSeg.y;
		float crossProd = (lineSeg.x * pointToStart.y - lineSeg.y * pointToStart.x);
		distanceSquared = (crossProd * crossProd) / (lineSegDistSquared);
	}

	return distanceSquared;
}

void Utility::InitSprite(sf::Sprite& sprite, std::string tex, sf::Vector2i subTexCount, sf::Vector2f origin)
{
  sprite.setScale(DEFAULT_SCALE);
  sprite.setTexture(GET_TEXTURE(tex));
  sf::Vector2i texSize(GET_TEXTURE(tex).getSize());
  sprite.setTextureRect({0, 0, texSize.x / subTexCount.x, 
                               texSize.y / subTexCount.y});
  sprite.setOrigin({origin.x * sprite.getTextureRect().width, 
                    origin.y * sprite.getTextureRect().height});
}

void Utility::InitText(sf::Text& text, const sf::Font& font, std::string str, sf::Vector2f pos, sf::Vector2f origin, sf::Color col)
{
  text.setPosition(pos);
  text.setCharacterSize(SCALED_DIM);
  text.setFillColor(col);
  text.setFont(font);

  UpdateText(text, str, origin);
}

void Utility::UpdateText(sf::Text& text, std::string newStr, sf::Vector2f origin)
{
  text.setString(newStr);

  text.setOrigin({origin.x * text.getLocalBounds().width, 
                  origin.y * text.getLocalBounds().height - FSCALE});
}
