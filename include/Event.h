#ifndef EVENT_H
#define EVENT_H

// Stores a list of runtime game events to be processed in the Program class
class Event
{
public:
  enum class Type
  {
    programClose,
    loadNewGame,
    returnCamera,
    cameraUp,
    geToMainMenu,
    loadNewMenu
  };

  Event() = default;
  Event(Type type, int data = 0)
    :
    type(type),
    data(data)
  {}

public:
  Type type = Type::programClose;
  int data = 0;
  // void*;
};

#endif