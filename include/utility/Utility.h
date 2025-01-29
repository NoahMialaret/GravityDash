#ifndef UTILITY_H
#define UTILITY_H

// The path to the file that stores save data
#define SAVE_FILE "files/save.json"

#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>

#include "Bezier.h"
#include "GameStats.h"
#include "ProgramSettings.h"
#include "Stats.h"
#include "AssetManager.h"

#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <random>

// The zero vector, used to set default values
#define ZERO_VECTOR (sf::Vector2f(0.0f, 0.0f))
// The dimensions of a typical sprite when rendered to the screen
#define SCALED_DIM (ProgramSettings::gameScale * Utility::GetInstance()->GetSpriteDim())
// The default scale used by most sprites
#define DEFAULT_SCALE (sf::Vector2f(ProgramSettings::gameScale, ProgramSettings::gameScale))

// Linear bezier curve that increases at a constant rate
#define LINEAR_CURVE   (Bezier({{0, 0},{1,1}}))
// Ease-In bezier curve decays rapidly before tapering out
#define EASE_IN_CURVE  (Bezier({{0, 0},{0,1},{1, 1}}))
// Ease-Out bezier grows slowly before growing expontentially
#define EASE_OUT_CURVE (Bezier({{0, 0},{1,0},{1, 1}}))

// `Utility` is a global singleton class used to access commonly used functions and variables
struct Utility
{
public:
  struct LineSegment
  {
    sf::Vector2f start;
    sf::Vector2f end;
  };

private:
  // Private constructor as only one instance should exist in the program
  Utility() = default;
  // The global Utility instance
  static Utility* instance;
  // Deletes the Utility instance
  static void Clean();

public:
  // Creates and returns the global instance of Utility
  static Utility* GetInstance();

  // Loads save data from disk
  void LoadSave(const char* filename);
  // Writes save data to disk
  void SaveData(const char* filename);

  // Returns the typical dimensions of a sprite
  int GetSpriteDim() const;
  // Returns a reference to the RNG used throughout the program
  std::mt19937& GetRNG();

  // Returns the squared distance from a point to a line segment (represented as two points)
  static float GetSquaredDistanceToLineSegment(sf::Vector2f point, LineSegment line);

  // Gets the sign of an integer, returns `1` for positive, `-1` for negative, and `0` for zero
  static int GetSign(int num);
  // Gets the sign of a float, returns `1` for positive, `-1` for negative, and `0` for zero
  static int GetSign(float num);

  // Initialises Sprite objects using its texture, the number of sub-textures that are contained, and the relative origin of the sprite (ranging from `0` - `1`) 
  static void InitSprite(sf::Sprite& sprite, std::string tex, sf::Vector2i subTexCount = {1, 1}, sf::Vector2f origin = {0.5f, 0.5f});
  // Initialises Text objects using its font, the text to display, its position, its relative origin (ranging from `0` to `1`), and its colour 
  static void InitText(sf::Text& text, const sf::Font& font, std::string str, sf::Vector2f pos, sf::Vector2f origin = {0.5f, 0.5f}, sf::Color col = {173, 103, 78});
  // Updates a Text object's new relative origin when its display text changes
  static void UpdateText(sf::Text& text, std::string newStr, sf::Vector2f origin = {0.5f, 0.5f});

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  static constexpr int spriteDim = 8; // The dimensions of a typical sprite

  std::mt19937 rng; // The random number generator used throughout the program
};

#endif
