#include "Utility.h"
#include "Particle.h"

float Utility::targetFrameRate = 60.0f;
float Utility::gameScale = 6.0f;
int Utility::spriteDim = 8.0f;
sf::Vector2f Utility::windowDim;

std::mt19937 Utility::rng = std::mt19937(std::random_device()());

std::vector<Bezier> Utility::curves =
{
  Bezier({{0, 0},{1,1}}),
  Bezier({{0, 0},{0,1},{1, 1}}),
  Bezier({{0, 0},{1,0},{1, 1}})
};

std::vector<int> Utility::initialKeyPresses;

sf::Sprite Utility::debugSprite;
std::vector<sf::Vector2f> Utility::debugPos;

sf::Shader Utility::entShad;
sf::Shader Utility::worldShad;

std::forward_list<std::unique_ptr<Particle>> Utility::particles;

float Utility::scoreMultiplier = 1.0f;

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

  float height = Utility::gameScale;
  if (&font == &Textures::small)
    height *= 4.0f;
  else if (&font == &Textures::medium)
    height *= 5.0f;
  else // large
    height *= 6.0f;

  text.setOrigin(sf::Vector2f(origin.x * text.getLocalBounds().width, origin.y * height - Utility::gameScale));
}

void Utility::UpdateText(sf::Text &text, std::string newStr, sf::Vector2f origin)
{
  text.setString(newStr);

  float height = Utility::gameScale;
  if (text.getFont() == &Textures::small)
    height *= 4.0f;
  else if (text.getFont() == &Textures::medium)
    height *= 5.0f;
  else // large
    height *= 6.0f;

  text.setOrigin(sf::Vector2f(origin.x * text.getLocalBounds().width, origin.y * height - Utility::gameScale));
}

std::string Utility::GetStringFromKeyCode(sf::Keyboard::Key key)
{
  if ((int)key >= 0 && (int)key <= 25) // letters
  {
    std::string ret;
    ret = (char)key + 'A';
    return ret;
  }
  else if ((int)key >= 26 && (int)key <= 35) // numbers
    return std::to_string((int)key - 26);
  else if ((int)key >= 75 && (int)key <= 84) // numpad
  {
    std::string ret = "npad" + std::to_string((int)key - 75);
    return ret;
  }

  switch (key)
  {
  case sf::Keyboard::Key::Escape:
    return "esc";
  case sf::Keyboard::Key::Tab:
    return "tab";
  case sf::Keyboard::Key::Enter:
    return "enter";
  case sf::Keyboard::Key::Backspace:
    return "bcksp";
  case sf::Keyboard::Key::LShift:
    return "lshft";
  case sf::Keyboard::Key::RShift:
    return "rshft";
  case sf::Keyboard::Key::LControl:
    return "lctrl";
  case sf::Keyboard::Key::RControl:
    return "rctrl";
  case sf::Keyboard::Key::Space:
    return "space";
  case sf::Keyboard::Key::LAlt:
    return "lalt";
  case sf::Keyboard::Key::RAlt:
    return "ralt";  
  case sf::Keyboard::Key::Up:
    return "up";  
  case sf::Keyboard::Key::Down:
    return "down";  
  case sf::Keyboard::Key::Left:
    return "left";  
  case sf::Keyboard::Key::Right:
    return "right";  
  case sf::Keyboard::Key::PageUp:
    return "p-up";  
  case sf::Keyboard::Key::PageDown:
    return "p-dwn";  

  case sf::Keyboard::Key::Grave:
    return "`";  
  case sf::Keyboard::Key::Hyphen:
    return "-";  
  case sf::Keyboard::Key::Equal:
    return "=";  
  case sf::Keyboard::Key::LBracket:
    return "[";  
  case sf::Keyboard::Key::RBracket:
    return "]";  
  case sf::Keyboard::Key::BackSlash:
    return "\\";  
  case sf::Keyboard::Key::SemiColon:
    return ";";  
  case sf::Keyboard::Key::Quote:
    return "'";  
  case sf::Keyboard::Key::Comma:
    return ",";  
  case sf::Keyboard::Key::Period:
    return ".";  
  case sf::Keyboard::Key::Slash:
    return "/";  

  default:
    return "NULL";
    break;
  }
}
