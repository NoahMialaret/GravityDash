#ifndef MENUCONFIG_H
#define MENUCONFIG_H

#include "SFML/Graphics.hpp"

#include "Event.h"
#include "Textures.h"
#include "Utility.h"

#include <string>
#include <vector>

#define CONFIG_MENU_MARGIN 5 * SCALED_DIM

struct OptionConfig
{
  std::string name;
  Event event;
  // int size;
  // union
  // {
  //   bool initBool
  //   int initInt;
  //   std::string initStr;
  // }
};

class MenuOption
{
public:
  MenuOption(std::string name, Event action, float yPos);

  virtual void Update();
  virtual void Render(sf::RenderWindow* win) const;

  virtual void SetY(float newY);
  virtual void Move(float offsetY);

  void ToggleHighlight();
  
protected:
  sf::Text displayName;

  Event action;

  bool isHighlighted = false;
};

class ToggleOption : public MenuOption
{
public:
  ToggleOption(std::string name, Event action, float yPos, bool init);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetY(float newY) override;
  void Move(float offsetY) override;

private:
  bool toggle = false;
  sf::Sprite toggleSprite;

};

class MenuOptionRange
{

};

class MenuOptionSelection
{

};

class MenuOptionControl
{

};

#endif