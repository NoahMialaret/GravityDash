#ifndef MENUCONFIG_H
#define MENUCONFIG_H

#include "SFML/Graphics.hpp"

#include "Controls.h"
#include "Event.h"
#include "AssetManager.h"
#include "Utility.h"
#include "RoundedRect.h"
#include "ProgramSettings.h"
#include "Keyboard.h"

#include <string>
#include <vector>

#define CONFIG_MENU_MARGIN 5 * SCALED_DIM

struct OptionConfig
{
  struct StaticText
  {
    std::string* text;
  };
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
    stat,
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
    StaticText statText;
    Toggle toggle;
    Range range;
    Selection selection;
    Control control;
  };
};

class ListItem
{
public:
  ListItem(std::string name, Event action, float* origin, float offset);

  virtual void Update();
  virtual void Render(sf::RenderWindow* win) const;

  void SetHighlight();

  bool IsActive();

  float GetOffset() const;

public:
  static ListItem* curHighlight;
  
protected:
  sf::Text displayName;
  static RoundedRect highlightBg;

  bool isHighlighted = false;

  Event action;

  float vertOffset;
  float* origin;

  bool isActive = false;
};

class StaticText : public ListItem
{
public:
  StaticText(std::string name, Event action, float* origin, float offset, OptionConfig::StaticText config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  sf::Text staticText;
};

class ToggleOption : public ListItem
{
public:
  ToggleOption(std::string name, Event action, float* origin, float offset, OptionConfig::Toggle config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  bool toggle = false;
  sf::Sprite toggleSprite;
};

class RangeOption : public ListItem
{
public:
  RangeOption(std::string name, Event action, float* origin, float offset, OptionConfig::Range config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int value = 0;
  int min = 0;
  int max = 0;
  sf::Text displayRange;
};

class SelectionOption : public ListItem
{
public:
  SelectionOption(std::string name, Event action, float* origin, float offset, OptionConfig::Selection config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  int index;
  std::vector<std::string> selections;
  sf::Text displaySelection;
};

class ControlOption : public ListItem
{
public:
  ControlOption(std::string name, Event action, float* origin, float offset, OptionConfig::Control config);

  void Update() override;
  void Render(sf::RenderWindow* win) const override;

private:
  RoundedRect keyBg;
  sf::Text curKey;
};

#endif