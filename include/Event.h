#ifndef EVENT_H
#define EVENT_H

// Stores a list of runtime game events to be processed in the Program class
class Event
{
public:
  enum class MenuType
  {
    title,
    main,
    play,
    options,
    score,
    medal,
    custom,
    multi,
    addPlayers,
    pause
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
    // int numPlayers = 1;
    // int numComputers = 0;
    // int targetSpawnChance = 90;
    // int sawFrequency = 10;
    // std::vector<ids> transitionIDs
  };

  enum class Type
  {
    programClose,
    loadNewMenu,
    loadNewGame,
    pause,
    resumePlay,
    exitGame,
    // returnCamera,
    // cameraUp,
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
    MenuType menuType;
    GameConfig gameConfig;
  };
};

#endif