#ifndef EVENT_H
#define EVENT_H

#include <queue>

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
  enum class GamePreset
  {
    title,
    minute,
    rush,
    coop,
    vs,
    null
  };

  enum class Type
  {
    programClose,
    reloadMenu,
    pushMenu,
    menuReturn,

    // Game events
    gameNew,    // GamePreset
    gameReset,  // null
    gameTimeUp, // null
    gameDone,   // null
    gameExit,   // null

    pause,  // null
    resume, // null

    collisionSaw,       // int (charID)
    collisionTarget,    // int (charID)
    collisionTimeBonus, // int (charID)

    boostFull,    // int (charID)
    playerLand,   // int (charID)
    timerRefill,  // null
  };

  Event() = default;

  // Gets the next event off of the stack
  static bool PollEvent(Event& event)
  {
    if (events.size() == 0)
      return false;

    event = events.front();
    events.pop();
    return true;
  }

public:
  static std::queue<Event> events;   // Accumulated program events

  Type type = Type::programClose;
  
  union 
  {
    int value;
    MenuType menuType;
    GamePreset gamePreset;
  };
};

#endif