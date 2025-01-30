#ifndef EVENT_H
#define EVENT_H

#include <queue>

// Shorthand used to access `EventManager`'s `PushEvent()` function
#define PUSH_EVENT EventManager::GetInstance()->PushEvent

// `Event` is a utility struct designed to carry information between program components
//   Each `Event` is distinguished with a `type` and some `data`
struct Event
{
public:
  // `ComboEvent` contains data related to the number of targets hit by a player
  // - `charID`: The identifier for the character this event relates to
  // - `wasSuperJump`: Whether the combo was obtained using a 'super jump'
  // - `count`: The number of targets hit 
  struct ComboEvent
  {
    int charID;
    bool wasSuperJump;
    int count;
  };

  // `CollisionEvent` contains data related to player-object collisions
  // - `charID`: The identifier for the character that was in the collision
  // - `colX`: The X-position of the colliding object
  // - `colY`: The Y-position of the colliding object
  struct CollisionEvent
  {
    int charID;
    float colX;
    float colY;
  };

  struct UpdateSettingsEvent
  {
    const char* name;
    int value;
  };

  enum class Type
  {
    programClose, // null

    // Menu events
    reloadMenu, // int (maps to `Menu::Type`)
    pushMenu,   // int (maps to `Menu::Type`)
    menuReturn, // null

    // Game events
    gameNew,    // int (maps to `GameManager::Preset`)
    gameReset,  // null
    gameTimeUp, // null
    gameDone,   // null
    gameExit,   // null

    // State events
    pause,  // null
    resume, // null

    // Collision Events
    collisionSaw,       // `CollisionEvent`
    collisionTarget,    // `CollisionEvent`
    collisionTimeBonus, // `CollisionEvent`
    
    // Gameplay interaction events
    timerRefill,  // null
    boostFull,    // int (`charID`)
    playerJump,   // int (`charID`)
    playerSuper,  // int (`charID`)
    playerHit,    // int (`charID`)
    playerCombo,  // `ComboEvent`

    // Settings events
    updateSettings, // `UpdateSetting`
  };

  // The different formats the `data` member can take
  union Data
  {
    int value;
    ComboEvent combo;
    CollisionEvent collision;
    UpdateSettingsEvent updateSettings;
  };

  Event() = default;

  // Constructs `Event` with its identifier `type` and some `data`
  Event(Type type, Data data = Data())
    :
    type(type),
    data(data)
  {}

public:
  Type type = Type::programClose; // What this event signifies

  Data data; // The Event's data, having the format not match what is expected from `type` is ill-informed
};

// `EventManager` is a global singleton class that handles the main `Event` queue
class EventManager
{
private:
  // Private constructor as only one instance should exist in the program
  EventManager() = default;
  // The global EventManager instance
  static EventManager* instance;
  // Deletes the EventManager instance
  static void Clean();

  // Polls the Event queue for the next event, returns `false` if no event was polled
  bool PollEvent(Event& event);

public:
  // Creates and returns the global instance of Utility
  static EventManager* GetInstance();

  // Constructs and pushes an `Event` to the queue
  void PushEvent(Event::Type type, Event::Data data = Event::Data());
  // Pushes an `Event` to the queue
  void PushEvent(Event event);

private:
  friend class Program; // Allows `Program` to handle sensitive functions and data members

  std::queue<Event> events; // The `Event` queue that is polled from
};

#endif

