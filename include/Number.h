#ifndef SCORE_H
#define SCORE_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Utility.h"

#include <string>
#include <vector>

// A class representing the accumulated score accuired during gameplay
class Number
{
public:
  // Constructs a number which is centred at the given position
  Number(sf::Vector2f centre, sf::Vector2i digitSize);
  // Constructs a number with a specified value
  Number(int startingValue, sf::Vector2f centre, sf::Vector2i digitSize);

  virtual void Update() = 0;
  // Renders the digits to the screen
  void Render(sf::RenderWindow* win) const;

  // Add a given value to the number, uses Add() and Subtract()
  void AddPoints(int value);
  // Returns the number as an integer value
  int GetAsInt() const;
  // Returns the number as a string
  std::string GetAsString() const;

private:
  // Adds a digit to the number
  void PushBackNumber(int value);
  // Recentres the digits when a new one is added
  void Recentre();
  
  // Adds a positve value to the number
  void Add(unsigned int value);
  // Substracts a value from the number, Number cannot go below 0
  void Subtract(unsigned int value);

protected:
  sf::Texture tex;                      // The texture used by digits
  std::vector<sf::Sprite> scoreSprites; // The rendered digits of the number
  std::vector<int> totalScore;          // The digits of the number

  sf::Vector2i digitSize; // The pixel dimensions of a single digit

  sf::Vector2f centre; // The point where the number is centred on
};

// A Number child class that represents the score accumulated during a game
class GameScore : public Number
{
public:
  GameScore(sf::Vector2f centre);
  GameScore(int startingValue, sf::Vector2f centre);

  void Update() override;
private:

};

// A Number child class that represents the scores that appear when a player hits a target
class PlayerPoints : public Number
{
public:
  PlayerPoints(sf::Vector2f centre, sf::Vector2f vel);
  PlayerPoints(int startingValue, sf::Vector2f centre, sf::Vector2f vel);

  void Update() override;

private:
  sf::Vector2f vel;
  Bezier curve;
};

#endif