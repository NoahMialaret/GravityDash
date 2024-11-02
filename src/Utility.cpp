#include "Utility.h"
#include "Particle.h"

std::mt19937 Utility::rng = std::mt19937(std::random_device()());

std::vector<Bezier> Utility::curves =
{
  Bezier({{0, 0},{1,1}}),
  Bezier({{0, 0},{0,1},{1, 1}}),
  Bezier({{0, 0},{1,0},{1, 1}})
};

sf::Sprite Utility::debugSprite;
std::vector<sf::Vector2f> Utility::debugPos;

sf::Shader Utility::entShad;
sf::Shader Utility::worldShad;

std::forward_list<std::unique_ptr<Particle>> Utility::particles;

float Utility::scoreMultiplier = 1.0f;

void Utility::LoadSave(const char* filename)
{

  try
  {
    std::ifstream file(filename);
    nlohmann::json save = nlohmann::json::parse(file);

    ProgramSettings::Init(save["settings"]);

    file.close();
  }
  catch (const nlohmann::json::exception& e)
  {
    std::cout << "Save file was corrupted or could not be found, creating new one...\n";
    ProgramSettings::Init();
    SaveData(filename);
  }
}

void Utility::SaveData(const char* filename)
{
  nlohmann::json save;

  ProgramSettings::Save(save);

  std::ofstream file(filename);

  file << save.dump(2);

  file.close();

  std::cout << "Data saved!\n";
}

std::string Utility::IntToString(int number, int minDigits)
{
	// The int represented by a string in reverse
	std::string RevInt;

	while (number >= 10) 
	{
		RevInt.push_back((number % 10) + 48);
		number = number / 10; 
	}
	RevInt.push_back(number + 48);

	while (RevInt.size() < minDigits)
	{
		RevInt.push_back('0');
	}

	std::string stringedInt = "";

	for (auto i = RevInt.end(); i != RevInt.begin();)
	{
		i--;
		stringedInt.push_back(*i);
	}

	return stringedInt;
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

float Utility::GetSquaredDistanceToLineSegment(sf::Vector2f centrePos, std::pair<sf::Vector2f, sf::Vector2f> lineSegPoints)
{
  // If the both points in the line segment are bascially identical, treat them as the same point
  if (abs(lineSegPoints.first.x - lineSegPoints.second.x) < 1.0f && abs(lineSegPoints.first.y - lineSegPoints.second.y) < 1.0f)
  {
    return std::pow(lineSegPoints.first.x - centrePos.x, 2.0f) + std::pow(lineSegPoints.first.y - centrePos.y, 2.0f);
  }

	sf::Vector2f lineSeg = lineSegPoints.second - lineSegPoints.first;
	sf::Vector2f firstToCentre = centrePos - lineSegPoints.first;
	sf::Vector2f secondToCentre = centrePos - lineSegPoints.second;

	float dotProdFirst = lineSeg.x * firstToCentre.x + lineSeg.y * firstToCentre.y;
	float dotProdSecond = lineSeg.x * secondToCentre.x + lineSeg.y * secondToCentre.y;

	float distanceSquared = 0.0f;

	if (dotProdSecond > 0.0f) // Projection of dot products are in the same direction
	{
		distanceSquared = std::pow(lineSegPoints.second.x - centrePos.x, 2.0f) 
			+ std::pow(lineSegPoints.second.y - centrePos.y, 2.0f);
	}
	else if (dotProdFirst < 0.0f) // Projection of dot products are in the opposite direction
	{
		distanceSquared = std::pow(lineSegPoints.first.x - centrePos.x, 2.0f) 
			+ std::pow(lineSegPoints.first.y - centrePos.y, 2.0f);
	}
	else // Projection of dot products are in different directions
	{
		float lineSegDistSquared = lineSeg.x * lineSeg.x + lineSeg.y * lineSeg.y;
		float crossProd = (lineSeg.x * firstToCentre.y - lineSeg.y * firstToCentre.x);
		distanceSquared = (crossProd * crossProd) / (lineSegDistSquared);
	}

	return distanceSquared;
}

void Utility::Render(sf::RenderWindow* win)
{
	for(auto& pos : debugPos)
	{
		debugSprite.setPosition(pos);
		win->draw(debugSprite);
	}
}

void Utility::FlushDebugSprites()
{
	std::cout << "Clearing debug sprites...\n";
	debugPos.clear();
}

void Utility::UpdateParticles()
{
  if (particles.empty())
    return;
    
  for (auto prev = particles.before_begin(), cur = particles.begin(); cur != particles.end();)
  {
    cur->get()->Update();
    if (cur->get()->HasFinished())
    {
      while (cur != particles.end() && cur->get()->HasFinished())
      {
        cur = particles.erase_after(prev);
      }
    }
    else
    {
      prev++;
      cur++;
    }
  }
  
}

void Utility::RenderParticles(sf::RenderWindow* win)
{

  for (auto cur = particles.begin(); cur != particles.end(); cur++)
  {
    cur->get()->Render(win);
  }
}

void Utility::InitSprite(sf::Sprite &sprite, std::string tex, sf::Vector2f pos, sf::Vector2i subRect, sf::Vector2f origin)
{
  sprite.setScale(DEFAULT_SCALE);
  sprite.setTexture(Textures::textures.at(tex));
  sprite.setPosition(pos);
  sf::Vector2i texSize = (sf::Vector2i)Textures::textures.at(tex).getSize();
  sprite.setTextureRect({0, 0, texSize.x / subRect.x, texSize.y / subRect.y});
  sprite.setOrigin(sf::Vector2f(origin.x * sprite.getTextureRect().width, origin.y * sprite.getTextureRect().height));
}

void Utility::InitText(sf::Text &text, sf::Font &font, std::string str, sf::Vector2f pos, sf::Vector2f origin, sf::Color col)
{
  text.setPosition(pos);
  text.setString(str);
  text.setCharacterSize(SCALED_DIM);
  text.setFillColor(col);
  text.setFont(font);

  float height = ProgramSettings::gameScale;
  if (&font == &Textures::small)
    height *= 4.0f;
  else if (&font == &Textures::medium)
    height *= 5.0f;
  else // large
    height *= 6.0f;

  text.setOrigin(sf::Vector2f(origin.x * text.getLocalBounds().width, origin.y * height - ProgramSettings::gameScale));
}

void Utility::UpdateText(sf::Text &text, std::string newStr, sf::Vector2f origin)
{
  text.setString(newStr);

  float height = ProgramSettings::gameScale;
  if (text.getFont() == &Textures::small)
    height *= 4.0f;
  else if (text.getFont() == &Textures::medium)
    height *= 5.0f;
  else // large
    height *= 6.0f;

  text.setOrigin(sf::Vector2f(origin.x * text.getLocalBounds().width, origin.y * height - ProgramSettings::gameScale));
}
