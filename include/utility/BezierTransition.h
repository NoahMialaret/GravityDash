#ifndef BEZIERTRANSITION_HPP
#define BEZIERTRANSITION_HPP

#include <SFML/Graphics.hpp>

#include "Bezier.h"
#include "Clock.h"
#include "Utility.h"

#include <queue>

// The `BezierTransition` class utilises a queue of `Transition`s to allow smooth
// transitions, using Bezier curves, of some data from one value to another
template <typename T>
class BezierTransition
{
public:
  // A transition defines how a given data type should change between two values 
  struct Transition
  {
    Curve curve;    // The curve used to define the motion from `start` to `end`
    int duration;   // The duration the transition lasts for
    T start;        // The starting value of the transition, i.e. when time is 0
    T end;          // The end value of the transition, i.e. when time is 1
  };

public:
  BezierTransition() = default;
  // Constructs `BezierTransition` with a pointer to some data for it to store and modify
  BezierTransition(T* data);

  // Updates `timer` and processes any active transitions
  void Update();

  // Clears all the transitions *without processing them*
  void Clear();
  // Processes all transitions instantly, sets `data` to the `end` value of the last transition
  void ProcessAll();

  // Pushes a transition to the queue, as an offset of the current value stored at `data`
  void Push(Curve curve, int duration, T offset);
  // Pushes a transition to the queue given the curve, duration, start, and end value of the transition
  void Push(Curve curve, int duration, T start, T end);
  // Pushes the given transition to the queue
  void Push(Transition transition);

private:
  // Processes the current transition based on a given time `t` between 0 and 1
  void Process(float t);
  // Processes the current transition as if it has reached its end, and pops it from the queue
  void ProcessAndPop();

private:
  T* data = nullptr; // The data that is modified by the transitions

  std::queue<Transition> transitions; // A queue of transitions, pops whenever the duration of one is reached

  int timer = 0; // The timer used to determine transition point of the active transition
};

template <typename T>
inline BezierTransition<T>::BezierTransition(T* data)
  :
  data(data)
{}

template <typename T>
inline void BezierTransition<T>::Update()
{
  if (transitions.empty() || data == nullptr)
    return;

  timer += Clock::Delta();

  while (timer >= transitions.front().duration)
  {
    ProcessAndPop();
    if (transitions.empty())
      return;
  }

  Process(float(timer) / float(transitions.front().duration));
}

template <typename T>
inline void BezierTransition<T>::Clear()
{
  while (!transitions.empty())
    transitions.pop();
}

template <typename T>
inline void BezierTransition<T>::ProcessAll()
{
  T end = *data;
  while (!transitions.empty())
  {
    end = transitions.front().end;
    transitions.pop();
  }
  *data = end;
}

template <typename T>
inline void BezierTransition<T>::Process(float t)
{
  Transition cur = transitions.front();

  float p = Utility::curves[(int)cur.curve].GetValue(t);
  *data = cur.start + p * (cur.end - cur.start);
}

template <typename T>
inline void BezierTransition<T>::ProcessAndPop()
{
  timer -= transitions.front().duration;

  *data = transitions.front().end;

  transitions.pop();
}

template <typename T>
inline void BezierTransition<T>::Push(Curve curve, int duration, T offset)
{
  Push(curve, duration, *data, *data + offset);
}

template <typename T>
inline void BezierTransition<T>::Push(Curve curve, int duration, T start, T end)
{
  Push({curve, duration, start, end});
}

template <typename T>
inline void BezierTransition<T>::Push(Transition transition)
{
  transitions.push(transition);

  if (transitions.size() == 1)
    timer = 0;
}

#endif