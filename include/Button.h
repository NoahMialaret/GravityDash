#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Textures.h"
#include "Utility.h"

#include <string>
#include <vector>

struct ButtonConfig
{
  std::string name;
  Event event;
  int size;
};

#define SMALL 0
#define MEDIUM 1
#define LARGE 2

class Button
{
public:
  Button(std::string name, Event action, std::string textureName, sf::Vector2f pos);
  // Button(const Button& button);

  virtual void Update() = 0;
  virtual void Render(sf::RenderWindow* win) const;

  void Move(sf::Vector2f offset);

  void Click() const;

  // std::string GetName() const;
  // sf::Vector2f GetBounds() const;

  void ToggleHighlight();
  
protected:
  sf::Sprite button;
  sf::Text displayName;

  Event action;

  bool isHighlighted = false;

  // The name of the button, used by other classes to determine which button was selected
  //std::string name;
  // A rectangle that determines the button's grid position and size
  // sf::IntRect gridBounds;
};

#define SMALL_TEXT_OFFSET Utility::gameScale * sf::Vector2f(0.0f, 0.5f)

class SmallButton : public Button
{
public:
  SmallButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
};

#define MEDIUM_TEXT_OFFSET Utility::gameScale * sf::Vector2f(-2.5f, 7.0f)

class MediumButton : public Button
{
public:
  MediumButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
};

#define LARGE_TEXT_OFFSET Utility::gameScale * sf::Vector2f(-2.5f, 6.0f)

class LargeButton : public Button
{
public:
  LargeButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
  // buttonimage
};

// class ButtonImage

#endif