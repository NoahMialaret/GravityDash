#ifndef SCORECOMPONENT_H
#define SCORECOMPONENT_H

#include <SFML/Graphics.hpp>

#include "Game.h"
#include "GameComponent.h"
#include "Number.h"
#include "ProgramSettings.h"
#include "Utility.h"
#include "World.h"

#include <functional>
#include <list>

// A GameComponent which keeps track of the score accumulated through the game
class ScoreComponent : public GameComponent
{
private:
  // DisplayDigit can be both manipulated like an int, and be drawn to the screen as a sprite
  class DisplayDigit
  {
  public:
    // Constructs `DisplayDigit` with a starting digit
    DisplayDigit(int digit);

    // Performs `digit` += `rhs`, and updates the texture rect
    DisplayDigit& operator+=(const int& rhs);
    // Performs `digit` -= `rhs`, and updates the texture rect
    DisplayDigit& operator-=(const int& rhs);
    // Performs `digit` = `rhs`, and updates the texture rect
    DisplayDigit& operator=(const int& rhs);
    // Returns `digit` as `DisplayDigits` integer representation
    operator int() const;

    // Renders the digit to the screen
    void Render(sf::RenderWindow* win) const;

    // Updates the position of the sprite with a new positon
    void SetPositon(sf::Vector2f newPos);
    // Returns the width of the sprite before transformations
    int GetTextureRectWidth() const;

  private:
    // Updates the texture rect to remain consistent with `digit`
    void UpdateTextureRect();
    
  private:
    int digit;          // The integer representation of `DisplayDigit`
    sf::Sprite sprite;  // The graphical representation of `DisplayDigit`
  };

public:
  // Constucts `ScoreComponent` with a base score of `0` and attaching to the top of the world
  ScoreComponent(Game* game);

  // Processes score related events, such as when a player has completed a combo
  void ProcessEvent(Event& event) override;
  // Currently Unused
  void Update() override;
  // Renders the digits
  void Render(sf::RenderWindow* win) const override;

private:
  // Adds `amount` to the score
  void Add(int amount);

private:
  std::list<DisplayDigit> digits{0}; // A list of drawable digits, from most to least significant

  float multiplier = 1.0f;
};

#endif