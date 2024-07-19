#include "Textures.h"

std::map<std::string, sf::Texture> Textures::textures =
{
  {"arrow", {}},
  {"background_tiles", {} },
  {"boost_meter", {} },
  {"buttons", {} },
  {"character", {} },
  {"debug", {} },
  {"entities", {} },
  {"frenchie", {} },
  {"nums_big", {} },
  {"nums_small", {} },
  {"particles", {} },
  {"tabs", {} },
  {"timer", {} },
  {"title", {} },
  {"toggle", {} }
};
sf::Font Textures::font;

void Textures::LoadTextures()
{
	std::cout << "Loading program textures..." << std::endl;

  for (auto& pair : textures)
  {
    if (!pair.second.loadFromFile("assets/" + pair.first + ".png"))
    {
      std::cout << "\tCould not load the following texture: " << pair.first << '\n';
    }
  }

  if (!font.loadFromFile("assets/GravDash.ttf"))
  {
    std::cout << "\tCould not load the program font!\n";
  }
}

void Textures::ClearTextures()
{
  textures =
  {
    {"arrow", {}},
    {"background_tiles", {} },
    {"boost_tab", {} },
    {"buttons", {} },
    {"character", {} },
    {"debug", {} },
    {"entities", {} },
    {"frenchie", {} },
    {"nums_big", {} },
    {"nums_small", {} },
    {"particles", {} },
    {"tabs", {} },
    {"timer", {} },
    {"title", {} },
    {"toggle", {} }
  };
}
