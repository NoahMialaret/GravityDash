#include "Textures.h"

std::map<std::string, sf::Texture> Textures::textures =
{
  {"arrow", {}},
  {"background_tiles", {} },
  {"boost_meter", {} },
  {"buttons", {} },
  {"character", {} },
  {"entities", {} },
  {"frenchie", {} },
  {"nums_big", {} },
  {"nums_small", {} },
  {"particles", {} },
  {"reticle", {}},
  {"tabs", {} },
  {"timer", {} },
  {"title", {} },
  {"toggle", {} },
  {"large_button", {}},
  {"medium_button", {}}
};
sf::Font Textures::large;
sf::Font Textures::medium;
sf::Font Textures::small;

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

  if (!large.loadFromFile("assets/fonts/largeFont.ttf"))
  {
    std::cout << "\tCould not load large font, check that largeFont.ttf is in the assets folder\n";
  }
  if (!medium.loadFromFile("assets/fonts/mediumFont.ttf"))
  {
    std::cout << "\tCould not load medium font, check that mediumFont.ttf is in the assets folder\n";
  }
  if (!small.loadFromFile("assets/fonts/smallFont.ttf"))
  {
    std::cout << "\tCould not load small font, check that smallFont.ttf is in the assets folder\n";
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
    {"entities", {} },
    {"frenchie", {} },
    {"nums_big", {} },
    {"nums_small", {} },
    {"particles", {} },
    {"tabs", {} },
    {"timer", {} },
    {"title", {} },
    {"toggle", {} },
    {"large_button", {}},
    {"medium_button", {}}
  };
}
