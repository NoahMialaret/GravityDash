#ifndef UTILITY_H
#define UTILITY_H

// The zero vector, used to set default values
#define ZERO_VECTOR fVec2(0.0f, 0.0f)
// The ingame pixel dimensions of a sprite
#define SCALED_DIM Utility::gameScale * Utility::spriteDim
// The default scale used by sprites
#define DEFAULT_SCALE fVec2(Utility::gameScale, Utility::gameScale)
// Used to set the origin of a sprite to its centre
#define CENTRED_ORIGIN fVec2(Utility::spriteDim, Utility::spriteDim) * 0.5f
// The current global time in milliseconds
#define CUR_TIME glfwGetTime()

#include "Event.hpp"
#include "Shader.hpp"
#include "Sprite.hpp"
#include "Texture.hpp"
#include "Vec2.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <random>

union Utility
{
public:
  // Converts interger numbers into a string
	static std::string IntToString(int number, int minDigits = 0);

  // Gets the sign of a number
  static int GetSign(int num);
  static int GetSign(float num);

  // Given a point and a line segment defined by two points, return whether the point is less than a tile's distance away
  static float GetSquaredDistanceToLineSegment(fVec2 centrePos, std::pair<fVec2, fVec2> lineSegPoints);

  // Render debug sprites
  static void Render();

  // Adds a key to 'initialKeyPresses'
  static void AddKeyPress(int key);
  // Checks whether a key was initially pressed in the current frame
  static bool CheckInitialPress(int key);

  // Removes all debug sprites
  static void FlushDebugSprites();

  // Gets the next event off of the stack
  static bool PollEvent(Event& event);

public:
  // static sf::Clock clock; // The internal game clock

  //settings
  static float targetFrameRate;   // The max framerate that the game runs at

  // settings (gamescale)
  static float gameScale; // The scale that the game is rendered at compared to the original sprites
  static float spriteDim; // The dimensions of a typical sprite (like tiles and units)

  static std::mt19937 rng;    // Used for random number generation

  // keyboard
  static std::vector<int> initialKeyPresses;  // A vector of keys which were initially pressed in the current frame

  // Debug
  static Texture debugTexture;            // The texture used for debug sprites
  // static Sprite debugSprite;              // The sprite used for rendering debug positions on the screen
  static std::vector<fVec2> debugPos;  // Holds all the saved debug sprite positions

  // static sf::Font programFont;    // The font used throughout the program

  // Events
  static std::vector<Event> events;   // Accumulated game events

  // static Shader defaultShader;

  // static sf::Shader shaderTest;
};

#endif