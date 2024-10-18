#ifndef EVENT_H
#define EVENT_H

// Stores a list of runtime game events to be processed in the Program class
class Event
{
public:
  enum class MenuType
  {
    gameEnd,
    main,
    options,
    pause,
    play,
    score,
    stats,
    title
  };

  // A struct containing different parameters used by the game
  struct GameConfig
  {
    enum class Type
    {
      title,
      min,
      rush
    };

    Type type;
    int numPlayers;
    int numComputers;
    int targetSpawnChance;
    int sawFrequency;
    int maxTime;
  };

  enum class Type
  {
    programClose,
    reloadMenu,
    pushMenu,
    menuReturn,
    loadNewGame,
    pause,
    resumePlay,
    exitGame,
    gameDone,
    restartGame
  };

  Event() = default;

  // Gets the next event off of the stack
  static bool PollEvent(Event& event)
  {
    if (events.size() == 0)
    {
      return false;
    }

    event = events[events.size() - 1];
    events.pop_back();
    return true;
  }

public:
  static std::vector<Event> events;   // Accumulated program events

  Type type = Type::programClose;
  
  union 
  {
    int value;
    MenuType menuType;
    GameConfig gameConfig;
  };
};

#endif