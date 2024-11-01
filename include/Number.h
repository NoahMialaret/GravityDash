#ifndef SCORE_H
#define SCORE_H

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Textures.h"
#include "Utility.h"
#include "programSettings.h"

#include <forward_list>
#include <list>
#include <string>
#include <vector>

#define NEGATIVE_SIGN 10

// A class representing the accumulated score accuired during gameplay
class Number
{
public:
  // Constructs a number which is centred at the given position
  Number(sf::Vector2f centre, sf::Vector2i digitSize, sf::Texture* tex);
  // Constructs a number with a specified value
  Number(int startingValue, sf::Vector2f centre, sf::Vector2i digitSize, sf::Texture* tex);

  virtual void Update() = 0;
  // Renders the digits to the screen
  virtual void Render(sf::RenderWindow* win) const;

  // Add a given value to the number, uses Add() and Subtract()
  void AddPoints(int value);
  // Returns the number to zero
  void Zero();
  // Returns the number as an integer value
  int GetAsInt() const;
  // Returns the number as a string
  std::string GetAsString() const;
  // Gets the number's centre position
  sf::Vector2f GetCentre() const;

protected:
  // Adds a digit to the number
  void PushBackNumber(int value);
  // Recentres the digits when a new one is added
  void Recentre();
  
  // Adds a positve value to the number
  void Add(unsigned int value);
  // Substracts a value from the number, Number cannot go below 0
  void Subtract(unsigned int value);

protected:
  std::vector<sf::Sprite> scoreSprites; // The rendered digits of the number
  std::vector<int> totalScore;          // The digits of the number

  sf::Texture* tex;

  sf::Vector2i digitSize; // The pixel dimensions of a single digit

  sf::Vector2f centre; // The point where the number is centred on

  bool isNegative = false; // Whether or not the number being stored is negative
};

// A Number child class that represents the scores that appear when a player hits a target
class TargetPoints : public Number
{
public:
  TargetPoints(sf::Vector2f centre, sf::Vector2f vel);
  TargetPoints(int startingValue, sf::Vector2f centre, sf::Vector2f vel);

  void Update() override;

  void SetVelocity(sf::Vector2f newVel);

private:
  sf::Vector2f vel;
  int timer = 100; // Timer for when to stop moving after spawning if it hasn't been given to a TotalPoints yet
  bool spawnVelocity = true; // Whether the point is using it's spawn velocity
};

// A Number child class that represents the points that appear after combining TargetPoints
class TotalPoints : public Number
{
private:
  enum class State
  {
    start,
    accumulate,
    total,
    finish
  };

public:
  TotalPoints(std::forward_list<TargetPoints> targetPoints);

  // Updates the sprites to move up the screen
  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  bool HasFinished();

private:
  int prevIndex = 0; // The previous sprite index which was moved up
  int timer; // How long the number remains before being deleted
  std::forward_list<TargetPoints> targetPoints;
  State curState = State::start;
};

// A Number child class that represents the score accumulated during a game
class GameScore : public Number
{
public:
  GameScore(sf::Vector2f centre);
  GameScore(int startingValue, sf::Vector2f centre);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  void AddTargetPoints(std::forward_list<TargetPoints>& target); 

private:
  std::list<TotalPoints> totalPoints;
};

#endif