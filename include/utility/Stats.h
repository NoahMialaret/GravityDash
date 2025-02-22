#ifndef STATS_H
#define STATS_H

#include <json.hpp>

#include <iostream>
#include <vector>

// Static class used to store game statistics and high scores
class Stats
{
public:
  // The game modes which have tracked highscores
  enum class HighScoreModes
  {
    oneMinute,
    rush,
    coop
  };

public:
  // Initialises variables with default values
  static void Init();
  // Initialises variables using data stored in a `JSON` object
  static void Init(nlohmann::json& stats);
  // Writes stored data to a `JSON` object
  static void Save(nlohmann::json& save);

  // Inserts a score into the stored list of highscores, maintaining order
  static void InsertHighScore(HighScoreModes mode, int score);

public:
  // Vectors that store the highscores for various modes
  static std::vector<int> minHighScores;
  static std::vector<int> rushHighScores;
  static std::vector<int> coopHighScores;

  // Other game related statistics
  static int gamesPlayed;
  static int jumps;
  static int specialJumpes;
  static int combos;
  static int hits;
};

#endif