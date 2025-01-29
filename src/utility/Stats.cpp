#include "Stats.h"

std::vector<int> Stats::minHighScores;
std::vector<int> Stats::rushHighScores;
std::vector<int> Stats::coopHighScores;

int Stats::gamesPlayed;
int Stats::jumps;
int Stats::specialJumpes;
int Stats::combos;
int Stats::hits;

void Stats::Init()
{
  minHighScores =  {0,0,0};
  rushHighScores = {0,0,0};
  coopHighScores = {0,0,0};

  gamesPlayed =   0;
  jumps =         0;
  specialJumpes = 0;
  combos =        0;
  hits =          0;
}

void Stats::Init(nlohmann::json& stats)
{
  try
  {
    minHighScores = {stats["min"][0], stats["min"][1], stats["min"][2]};
    std::sort(minHighScores.begin(), minHighScores.end(), std::greater());

    rushHighScores = {stats["rush"][0], stats["rush"][1], stats["rush"][2]};
    std::sort(rushHighScores.begin(), rushHighScores.end(), std::greater());

    coopHighScores = {stats["coop"][0], stats["coop"][1], stats["coop"][2]};
    std::sort(coopHighScores.begin(), coopHighScores.end(), std::greater());

    gamesPlayed =   stats["gamesPlayed"];
    jumps =         stats["jumps"];
    specialJumpes = stats["specialJumps"];
    combos =        stats["combos"];
    hits =          stats["hits"];
  }
  catch(...)
  {
    std::cout << "Error loading statistics...\n";
    throw;
  }
}

void Stats::Save(nlohmann::json &save)
{
  save["stats"] = {
    {"min", minHighScores},
    {"rush", rushHighScores},
    {"coop", coopHighScores},
    {"gamesPlayed", gamesPlayed},
    {"jumps", jumps},
    {"specialJumps", specialJumpes},
    {"combos", combos},
    {"hits", hits}
  };
}

void Stats::InsertHighScore(HighScoreModes mode, int score)
{
  std::vector<int>* highScore = nullptr;

  switch (mode)
  {
  case HighScoreModes::oneMinute:
    highScore = &minHighScores;
    break;
  case HighScoreModes::rush:
    highScore = &rushHighScores;
    break;
  case HighScoreModes::coop:
    highScore = &coopHighScores;
    break;
  }

  for (int i = 0; i < (int)highScore->size(); i++)
  {
    if (score > (*highScore)[i])
    {
      int temp = (*highScore)[i];
      (*highScore)[i] = score;
      score = temp;
    }
  }
}