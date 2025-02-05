#ifndef TEXTURES_H
#define TEXTURES_H

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <iostream>
#include <unordered_map>

#define LIGHTEST  sf::Color(200, 200, 200)
#define LIGHT     sf::Color(150, 150, 150)
#define DARK      sf::Color(100, 100, 100)
#define DARKEST   sf::Color( 50,  50,  50)

// Shorthand for accessing `AssetManager`'s `GetTex()` function
#define GET_TEXTURE(texName) AssetManager::GetInstance()->GetTex(texName)

// Gets a reference to the small program font 
#define SMALL_FONT AssetManager::GetInstance()->GetFont("smallFont")
// Gets a reference to the medium program font 
#define MEDIUM_FONT AssetManager::GetInstance()->GetFont("mediumFont")
// Gets a reference to the large program font 
#define LARGE_FONT AssetManager::GetInstance()->GetFont("largeFont")

// Gets a reference to the shader used for rendering the world
#define WORLD_SHADER AssetManager::GetInstance()->GetShad("world")
// Gets a reference to the shader used for rendering entities
#define ENTITY_SHADER AssetManager::GetInstance()->GetShad("entity")
// Gets a reference to the shader used for rendering static UI elements 
#define STATIC_SHADER AssetManager::GetInstance()->GetShad("static")
// Gets a reference to the shader used for rendering text and rectangles 
#define RECT_SHADER AssetManager::GetInstance()->GetShad("rect")

// Stores the relative origin as seperate since
struct Text
{
  sf::Text drawable;
  sf::Vector2f origin;
};

// Stores variables used by the rectangle shader
struct Rect
{
  sf::RectangleShape drawable;
  int fillCol0;
  int fillCol1;
  int outlineCol;
};

// The `AssetManager` class is a global singleton that provides an interface to access
//   the program's various graphical assets, including textures, fonts, and shaders
class AssetManager
{
private:
  // Private constructor as only one instance should exist in the program
  //  Preloads all the textures, fonts, and shaders through lookup in the `assets/` directory
  AssetManager();
  // The global AssetManager instance
  static AssetManager* instance;
  // Deletes the AssetManager instance
  static void Clean();

public:
  // Creates and returns the global instance of AssetManager
  static AssetManager* GetInstance();

  // Returns a reference to a texture given its identifying name (the name of the file without extensions)
  sf::Texture& GetTex(std::string name);
  // Returns a reference to a font given its identifying name (the name of the file without extensions)
  sf::Font& GetFont(std::string name);
  // Returns a reference to a shader given its identifying name (the name of the file without extensions)
  sf::Shader& GetShad(std::string name);

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  // The container used to map an identifier to a texture
  std::unordered_map<std::string, sf::Texture> textures;
  // The container used to map an identifier to a font
  std::unordered_map<std::string, sf::Font> fonts;
  // The container used to map an identifier to a shader
  std::unordered_map<std::string, sf::Shader> shaders;
};

#endif