#ifndef GAMESTATS_H
#define GAMESTATS_H

#include <vector>

// A static utility class to store publically accessible game highscores and stats
class GameStats
{
public:
  struct Global
  {
    int gamesPlayed;
    int jumps;
    int specials;
    int hits;

  };

  struct Local
  {
    int jumps       = 0;
    int specials    = 0;
    int hits        = 0;
    int combos      = 0;
    int timeBoosts  = -1;
  };
  
public:
  // Contains 5 best highscores for the 1-min mode
  static std::vector<int> minHighscores;
  // Contains 5 best highscores for the rush mode
  static std::vector<int> rushHighscores;
  // Contains 5 best highscores for co-op mode
  static std::vector<int> coopHighscores;

  // Global stats that accumulate from every game ever played
  static Global globalStats;  
  // Local stats for the game that is currently being played
  static Local localStats;
};

#endif