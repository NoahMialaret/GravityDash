#ifndef BOOSTCOMPONENT_H
#define BOOSTCOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Event.h"
#include "Game.h"
#include "GameComponent.h"
#include "Settings.h"
#include "Utility.h"
#include "World.h"

#include <functional>
#include <vector>

// A GameComponent that enables players to build boost and perform a "super jump"
class BoostComponent : public GameComponent
{
private:
  // A boost meter associated with one of the players, can be incremented under certain conditions
  class BoostMeter
  {
  public:
    BoostMeter() = default;
    // Constructs `BoostMeter` with an id identical with charID, 
    // a maximum limit (in milliseconds), and attaching it to the game world
    BoostMeter(Game* game, int id, int limit);

    // Reduces the amount the meter is filled (if not already full)
    void Update();
    // Renders the gauge and its fill
    void Render(sf::RenderWindow* win) const;

    // Adds the specified amount of fill to the meter (in milliseconds)
    void Increment(int amount);
    // Resets the meter
    void Clear();

  private:
    int id = 0; // The id, corresponds to a character's charID  

    sf::Sprite gauge;         // The gauge of the meter
    sf::RectangleShape fill;  // A rectangle displaying how full the meter is

    int fillAmount = 0; // How full the meter is in milliseconds
    int limit = 0;      // The max limit of the meter in milliseconds
  };

public:
  // Constructs a number of `BoostMeters` based on the number of players in the game
  BoostComponent(Game* game, int limit);

  // Processes boost related events, such as if a boost has been consumed by a player
  void ProcessEvent(Event& event) override;
  // Updates the boost meters
  void Update() override;
  // Renders the boost meters
  void Render(sf::RenderWindow* win) const override;

private:
  std::vector<BoostMeter> boosts; // The boosts, one is stored for each player in the game
};

#endif