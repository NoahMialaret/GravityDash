#ifndef MENUBUTTON_H
#define MENUBUTTON_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Textures.h"
#include "Utility.h"

#include <string>
#include <vector>

// small button and medium/large button should be two seperate, non-inherited classes

struct ButtonConfig
{
  std::string name;
  Event event;
  int size;
};

#define SMALL 0
#define MEDIUM 1
#define LARGE 2

class MenuButton
{
public:
  MenuButton(Event action, std::string textureName, sf::Vector2f pos);
  // MenuButton(const MenuButton& button);

  virtual void Update() = 0;
  virtual void Render(sf::RenderWindow* win) const;

  void Move(sf::Vector2f offset);

  void Click() const;

  void ToggleHighlight();
  
protected:
  sf::Sprite button;
  sf::Text displayName;

  Event action;

  bool isHighlighted = false;
};

#define SMALL_TEXT_OFFSET Utility::gameScale * sf::Vector2f(0.0f, -0.5f)

class SmallButton : public MenuButton
{
public:
  SmallButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
};

#define MEDIUM_TEXT_OFFSET Utility::gameScale * sf::Vector2f(-2.5f, 6.0f)

class MediumButton : public MenuButton
{
public:
  MediumButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
};

#define LARGE_TEXT_OFFSET Utility::gameScale * sf::Vector2f(-2.5f, 5.0f)

class LargeButton : public MenuButton
{
public:
  LargeButton(sf::Vector2f pos, std::string name, Event action);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;
  // buttonimage
};

// class ButtonImage

#endif