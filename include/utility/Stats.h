#ifndef STATS_H
#define STATS_H

#include <json.hpp>

#include "Event.h"

#include <iostream>
#include <vector>

#define MAX_HIGHSCORES 3

#define GET_STAT(stat) (Stats::GetInstance()->GetStat(stat))
#define GET_LOCAL_STAT(stat) (Stats::GetInstance()->GetLocalStat(stat))
#define GET_HIGHSCORE(scoreStat, position) (Stats::GetInstance()->GetStat(scoreStat, position))

// Static class used to store game statistics and high scores
class Stats
{
public:
  enum class StatType
  {
    gamesPlayed,
    jumps,
    specials,
    hits,
    refills,
    minScores,
    rushScores,
    coopScores,
    null,
  };

  template <typename T>
  struct Stat
  {
    T value;
    std::string id;
  };

private:
  // Private constructor as only one instance should exist in the program
  Stats();
  // The global Stats instance
  static Stats* instance;
  // Deletes the Stats instance
  static void Clean();

  // Initialises variables using data stored in a `JSON` object
  void Load(nlohmann::json& save);

public:
  // Creates and returns the global instance of Stats
  static Stats* GetInstance();

  // Process statistics related game events
  void ProcessEvents(const Event& event);

  // Writes stored data to a `JSON` object
  void Save(nlohmann::json& save);

  // Returns the value of a specified stat, `position` is used to return a certain rank for highscore stats
  int GetStat(StatType stat, unsigned int position = 0);

  // Retruns the local per-game version of a specified stat
  int GetLocalStat(StatType stat);

  // Attempts to insert a new high score into the specified highscore stat
  void InsertScore(StatType scoreStat, int score);

public:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  // Vectors that store the highscores for various modes
  Stat<std::vector<int>> minHighScores;
  Stat<std::vector<int>> rushHighScores;
  Stat<std::vector<int>> coopHighScores;

  // Other game related statistics
  Stat<int> gamesPlayed;
  Stat<int> jumps;
  Stat<int> specialJumps;
  Stat<int> hits;

  // Per-game stats
  int localJumps = 0;
  int localSpecialJumps = 0;
  int localHits = 0;
  int localRefills = 0;
};

#endif