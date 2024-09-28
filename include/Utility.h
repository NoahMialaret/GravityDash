#ifndef UTILITY_H
#define UTILITY_H

// The zero vector, used to set default values
#define ZERO_VECTOR sf::Vector2f(0.0f, 0.0f)
// The ingame pixel dimensions of a sprite
#define SCALED_DIM Utility::gameScale * Utility::spriteDim
// The default scale used by sprites
#define DEFAULT_SCALE sf::Vector2f(Utility::gameScale, Utility::gameScale)
// Used to set the origin of a sprite to its centre
#define CENTRED_ORIGIN 0.5f * sf::Vector2f(Utility::spriteDim, Utility::spriteDim)

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Textures.h"

#include <forward_list>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

class Particle;

enum class Curve
{
  linear,
  easeIn,
  easeOut
};

union Utility
{
public:
  // Converts interger numbers into a string
	static std::string IntToString(int number, int minDigits = 0);

  // Gets the sign of a number
  static int GetSign(int num);
  static int GetSign(float num);

  // Given a point and a line segment defined by two points, return whether the point is less than a tile's distance away
  static float GetSquaredDistanceToLineSegment(sf::Vector2f centrePos, std::pair<sf::Vector2f, sf::Vector2f> lineSegPoints);

  // Render debug sprites
  static void Render(sf::RenderWindow* win);

  // Adds a key to 'initialKeyPresses'
  static void AddKeyPress(int key);
  // Checks whether a key was initially pressed in the current frame
  static bool CheckInitialPress(int key);

  // Removes all debug sprites
  static void FlushDebugSprites();

  static void UpdateParticles();
  static void RenderParticles(sf::RenderWindow* win);

  static void InitSprite(sf::Sprite& sprite, std::string tex, sf::Vector2f pos, sf::Vector2i subRect = {1, 1}, sf::Vector2f origin = {0.5f, 0.5f});
  static void InitText(sf::Text& text, sf::Font& font, std::string str, sf::Vector2f pos, sf::Vector2f origin = {0.5f, 0.5f}, sf::Color col = {173, 103, 78});

public:
  //settings
  static float targetFrameRate;   // The max framerate that the game runs at

  // settings (gamescale)
  static float gameScale; // The scale that the game is rendered at compared to the original sprites
  static int spriteDim; // The dimensions of a typical sprite (like tiles and units)
  static sf::Vector2f windowDim;

  static std::mt19937 rng;    // Used for random number generation

  static std::vector<Bezier> curves; // The bezier curves used by handlers for smooth transitions

  // keyboard
  static std::vector<int> initialKeyPresses;  // A vector of keys which were initially pressed in the current frame

  // Debug
  static sf::Sprite debugSprite;              // The sprite used for rendering debug positions on the screen
  static std::vector<sf::Vector2f> debugPos;  // Holds all the saved debug sprite positions

  static sf::Shader entShad;
  static sf::Shader worldShad;

  static std::forward_list<std::unique_ptr<Particle>> particles;

  static float scoreMultiplier;
};

#endif