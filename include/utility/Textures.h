#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>

#include <iostream>
#include <map>

// Preloads all textures on program start, due to the small number of textures
class Textures
{
public:
  static void LoadTextures();
  static void ClearTextures();

  static std::map<std::string, sf::Texture> textures;
  static sf::Font large;
  static sf::Font medium;
  static sf::Font small;
};

#endif