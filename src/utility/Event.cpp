#include "Event.h"

EventManager* EventManager::instance = nullptr;

void EventManager::Clean()
{
  if (instance != nullptr)
    delete instance;
  instance = nullptr;
}

EventManager *EventManager::GetInstance()
{
  if (instance == nullptr)
    instance = new EventManager();
  return instance;
}

bool EventManager::PollEvent(Event& event)
{
  if (events.empty())
    return false;
  
  event = events.front();
  events.pop();
  return true;
}

void EventManager::PushEvent(Event::Type type, Event::Data data)
{
  PushEvent(Event(type, data));
}

void EventManager::PushEvent(Event event)
{
  events.push(event);
}