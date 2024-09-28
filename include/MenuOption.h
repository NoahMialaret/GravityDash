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
  struct Toggle
  {
    bool init;
  };
  struct Range
  {
    int init;
    int min;
    int max;
  };
  struct Selection
  {
    int initIndex;
    std::vector<std::string>* selections;
  };
  struct Control
  {
    sf::Keyboard::Key init;
  };
  enum class Type
  {
    toggle,
    range,
    selection,
    control
  };

  std::string name;
  Event event;
  Type type;
  union
  {
    Toggle toggle;
    Range range;
    Selection selection;
    Control control;
  };
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
  ToggleOption(std::string name, Event action, float yPos, OptionConfig::Toggle config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetY(float newY) override;
  void Move(float offsetY) override;

private:
  bool toggle = false;
  sf::Sprite toggleSprite;
};

class RangeOption : public MenuOption
{
public:
  RangeOption(std::string name, Event action, float yPos, OptionConfig::Range config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetY(float newY) override;
  void Move(float offsetY) override;

private:
  int value = 0;
  int min = 0;
  int max = 0;
  sf::Text displayRange;
};

class SelectionOption : public MenuOption
{
public:
  SelectionOption(std::string name, Event action, float yPos, OptionConfig::Selection config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetY(float newY) override;
  void Move(float offsetY) override;

private:
  int index;
  std::vector<std::string> selections;
  sf::Text displaySelection;
};

// class ControlOption
// {

// };

#endif