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
        geToMainMenu
    };

public:
    Event::Type type;
};

#endif