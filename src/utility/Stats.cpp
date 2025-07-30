#include "Stats.h"

Stats* Stats::instance = nullptr;

void Stats::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

Stats::Stats()
{
  minHighScores   = {{}, "min"};
  rushHighScores  = {{}, "rush"};
  coopHighScores  = {{}, "coop"};

  gamesPlayed   = {0, "gamesPlayed"};
  jumps         = {0, "jumps"};
  specialJumps  = {0, "specialJumps"};
  hits          = {0, "hits"};
}

void Stats::Load(nlohmann::json& stats)
{
  try
  {
    std::vector<Stat<std::vector<int>>*> highScoreTrackers
    {
      &minHighScores, 
      &rushHighScores, 
      &coopHighScores
    };

    for (auto& tracker : highScoreTrackers)
    {
      tracker->value = {};
      for (auto& score : stats[tracker->id])
        tracker->value.push_back(score);

      std::sort(tracker->value.begin(), tracker->value.end(), std::greater());

      tracker->value.resize(MAX_HIGHSCORES);
    }

    gamesPlayed.value   = stats[gamesPlayed.id];
    jumps.value         = stats[jumps.id];
    specialJumps.value  = stats[specialJumps.id];
    hits.value          = stats[hits.id];
  }
  catch(...)
  {
    std::cout << "Error loading statistics...\n";
    throw;
  }
}

Stats* Stats::GetInstance()
{
  if (instance == nullptr)
    instance = new Stats();
  return instance;
}

void Stats::ProcessEvents(const Event& event)
{
  switch (event.type)
  {
  case Event::Type::incrementStat:
    switch ((StatType)event.data.value)
    {
    case StatType::gamesPlayed:
      gamesPlayed.value++;
      break;

    case StatType::jumps:
      jumps.value++;
      localJumps++;
      break;

    case StatType::specials:
      specialJumps.value++;
      localSpecialJumps++;
      break;

    case StatType::hits:
      hits.value++;
      localHits++;
      break;

    case StatType::refills:
      localRefills++;
      break;
    
    default:
      break;
    }
    break;

  case Event::Type::newScore:
    InsertScore((StatType)event.data.newScore.gameType, event.data.newScore.score);
    break;

  case Event::Type::gameNew:
  case Event::Type::gameReset:
    localJumps = 0;
    localSpecialJumps = 0;
    localHits = 0;
    localRefills = 0;
    break;
  
  default:
    break;
  }
}

void Stats::Save(nlohmann::json &save)
{
  save["stats"] = 
  {
    {minHighScores.id,  minHighScores.value},
    {rushHighScores.id, rushHighScores.value},
    {coopHighScores.id, coopHighScores.value},
    {gamesPlayed.id,    gamesPlayed.value},
    {jumps.id,          jumps.value},
    {specialJumps.id,   specialJumps.value},
    {hits.id,           hits.value}
  };
}

int Stats::GetStat(StatType stat, unsigned int position)
{
  switch (stat)
  {
  case StatType::gamesPlayed:
    return gamesPlayed.value;

  case StatType::jumps:
    return jumps.value;

  case StatType::specials:
    return specialJumps.value;

  case StatType::hits:
    return hits.value;

  case StatType::minScores:
    if (position > minHighScores.value.size())
      return 0;
    return minHighScores.value[position];

  case StatType::rushScores:
    if (position > rushHighScores.value.size())
      return 0;
    return rushHighScores.value[position];

  case StatType::coopScores:
    if (position > coopHighScores.value.size())
      return 0;
    return coopHighScores.value[position];
  
  default:
    std::cout << "Stat type not recognised\n";
    break;
  }

  return 0;
}

int Stats::GetLocalStat(StatType stat)
{
  switch (stat)
  {
  case StatType::jumps:
    return localJumps;

  case StatType::specials:
    return localSpecialJumps;

  case StatType::hits:
    return localHits;

  case StatType::refills:
    return localRefills;

  default:
    break;
  }
  return 0;
}

void Stats::InsertScore(StatType scoreStat, int score)
{
  std::vector<int>* tracker = nullptr;

  switch (scoreStat)
  {
  case StatType::minScores:
    tracker = &minHighScores.value;
    break;

  case StatType::rushScores:
    tracker = &rushHighScores.value;
    break;

  case StatType::coopScores:
    tracker = &coopHighScores.value;
    break;
  
  default:
    std::cout << "Score type not recognised\n";
    return;
  }

  if (tracker == nullptr)
    return;

  tracker->push_back(score);
  std::sort(tracker->begin(), tracker->end(), std::greater());
  tracker->resize(MAX_HIGHSCORES);
}