#include "Utility.h"
#include "Particle.h"

Utility* Utility::instance = nullptr;

// std::mt19937 Utility::rng = ;

// sf::Shader Utility::entShad;
// sf::Shader Utility::worldShad;

// std::forward_list<std::unique_ptr<Particle>> Utility::particles;

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
}

void Utility::LoadSave(const char *filename)
{
  
  try
  {
    std::ifstream file(filename);
    nlohmann::json save = nlohmann::json::parse(file);

    ProgramSettings::Init(save["settings"]);
    Stats::Init(save["stats"]);

    file.close();
  }
  catch (...)
  {
    std::cout << "Save file was corrupted or could not be found, creating new one...\n";
    ProgramSettings::Init();
    Stats::Init();
    SaveData(filename);
  }
}

void Utility::SaveData(const char* filename)
{
  nlohmann::json save;

  ProgramSettings::Save(save);

  Stats::Save(save);

  std::ofstream file(filename);

  file << save.dump(2);

  file.close();

  std::cout << "Data saved!\n";
}

int Utility::GetSpriteDim() const
{
  return spriteDim;
}

std::mt19937& Utility::GetRNG()
{
  return rng;
}

sf::Shader& Utility::GetEntityShader()
{
  return entShad;
}

sf::Shader& Utility::GetWorldShader()
{
  return worldShad;
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

void Utility::InitSprite(sf::Sprite &sprite, std::string tex, sf::Vector2f pos, sf::Vector2i subTexCount, sf::Vector2f origin)
{
  sprite.setScale(DEFAULT_SCALE);
  sprite.setTexture(Textures::textures.at(tex));
  sprite.setPosition(pos);
  sf::Vector2i texSize = (sf::Vector2i)Textures::textures.at(tex).getSize();
  sprite.setTextureRect({0, 0, texSize.x / subTexCount.x, texSize.y / subTexCount.y});
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
