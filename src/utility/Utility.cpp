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
  // sprite.setScale(DEFAULT_SCALE);
  sprite.setTexture(GET_TEXTURE(tex));
  sf::Vector2i texSize(GET_TEXTURE(tex).getSize());
  sprite.setTextureRect({0, 0, texSize.x / subTexCount.x, 
                               texSize.y / subTexCount.y});
  sprite.setOrigin({origin.x * sprite.getTextureRect().width, 
                    origin.y * sprite.getTextureRect().height});
}

void Utility::InitText(Text& text, const sf::Font& font, std::string str, sf::Vector2f pos, sf::Vector2f origin, sf::Color col)
{
  text.drawable.setPosition(pos);
  text.drawable.setCharacterSize(SPRITE_DIM);
  text.drawable.setFillColor(col);
  text.drawable.setFont(font);
  text.drawable.setString(str);
  text.origin = origin;

  // UpdateText(text, str, origin);
}

// void Utility::UpdateText(Text& text, std::string newStr, sf::Vector2f origin)
// {
//   text.drawable.setString(newStr);

//   text..setOrigin({origin.x * text.getLocalBounds().width, 
//                   origin.y * text.getLocalBounds().height - 1.0f});
// }

void Utility::RenderSpriteWithScale(sf::RenderWindow* win, sf::Sprite& sprite, sf::Shader* shader)
{
  sf::Vector2f pos = sprite.getPosition();
  sf::Vector2f scale = sprite.getScale();

  sprite.setPosition(FSCALE * pos);
  sprite.setScale(FSCALE * scale);

  win->draw(sprite, shader);

  sprite.setPosition(pos);
  sprite.setScale(scale);
}

void Utility::RenderTextWithScale(sf::RenderWindow* win, Text& text, sf::Shader* shader)
{
  sf::Vector2f pos = text.drawable.getPosition();
  float outline = text.drawable.getOutlineThickness();
  float size = text.drawable.getCharacterSize();

  text.drawable.setPosition(FSCALE * pos);
  text.drawable.setOutlineThickness(FSCALE * outline);

  text.drawable.setCharacterSize(FSCALE * size);
  text.drawable.setOrigin({text.origin.x * text.drawable.getLocalBounds().width, 
                  text.origin.y * text.drawable.getLocalBounds().height - FSCALE});

  win->draw(text.drawable, shader);

  text.drawable.setPosition(pos);
  text.drawable.setOutlineThickness(outline);
  text.drawable.setCharacterSize(size);
}

void Utility::RenderRectWithScale(sf::RenderWindow* win, sf::RectangleShape& rect, sf::Shader* shader)
{
  sf::Vector2f pos = rect.getPosition();
  sf::Vector2f size = rect.getSize();
  sf::Vector2f origin = rect.getOrigin();
  float outline = rect.getOutlineThickness();

  rect.setPosition(FSCALE * pos);
  rect.setSize(FSCALE * size);
  rect.setOrigin(FSCALE * origin);
  rect.setOutlineThickness(FSCALE * outline);

  win->draw(rect, shader);

  rect.setPosition(pos);
  rect.setSize(size);
  rect.setOrigin(origin);
  rect.setOutlineThickness(outline);
}
