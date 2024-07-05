#include "Utility.h"
#include "Particle.h"

sf::Clock Utility::clock;

float Utility::targetFrameRate = 60.0f;
float Utility::gameScale = 6.0f;
float Utility::spriteDim = 8.0f;

std::mt19937 Utility::rng = std::mt19937(std::random_device()());

std::vector<int> Utility::initialKeyPresses;

sf::Texture Utility::debugTexture;
sf::Sprite Utility::debugSprite;
std::vector<sf::Vector2f> Utility::debugPos;

sf::Font Utility::programFont;

std::vector<Event> Utility::events;

sf::Shader Utility::entShad;
sf::Shader Utility::worldShad;

std::forward_list<Particle> Utility::particles;

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

void Utility::AddKeyPress(int key)
{
	initialKeyPresses.push_back(key);
}

bool Utility::CheckInitialPress(int key)
{
	for (auto& k : initialKeyPresses)
	{
		if (k == key)
		{
			return true;
		}
	}

	return false;
}

void Utility::FlushDebugSprites()
{
	std::cout << "Clearing debug sprites...\n";
	debugPos.clear();
}

bool Utility::PollEvent(Event &event)
{
	if (events.size() == 0)
	{
		return false;
	}

	event = events[events.size() - 1];
	events.pop_back();
	return true;
}

void Utility::UpdateParticles()
{
  for (auto prev = particles.before_begin(), cur = particles.begin(); cur != particles.end();)
  {
    cur->Update();
    if (cur->HasFinished())
    {
      while (cur != particles.end() && cur->HasFinished())
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
    cur->Render(win);
  }
}
