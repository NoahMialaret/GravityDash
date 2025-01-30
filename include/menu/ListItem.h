#ifndef LISTITEM_H
#define LISTITEM_H

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

#define LIST_MARGIN (5 * SCALED_DIM)

// struct OptionConfig
// {
//   struct StaticText
//   {
//     std::string* text;
//   };
//   struct Toggle
//   {
//     bool init;
//   };
//   struct Range
//   {
//     int init;
//     int min;
//     int max;
//   };
//   struct Selection
//   {
//     int initIndex;
//     std::vector<std::string>* selections;
//   };
//   struct Control
//   {
//     sf::Keyboard::Key init;
//   };
//   enum class Type
//   {
//     stat,
//     toggle,
//     range,
//     selection,
//     control
//   };

//   std::string name;
//   Event event;
//   Type type;
//   union
//   {
//     StaticText statText;
//     Toggle toggle;
//     Range range;
//     Selection selection;
//     Control control;
//   };
// };

class Interactable
{
public:
  Interactable(int value);

  // Returns `true` when the interaction has finished
  virtual bool Update() = 0;
  virtual void Render(sf::RenderWindow* win) const = 0;

  int GetValue() const;

  virtual void SetPosition(sf::Vector2f& pos) = 0;

protected:
  int value = -1;
  int backup = -1;
};

class Static : public Interactable
{
public:
  Static(int value);

  // Returns `true` immediately as there is no interaction to be had
  bool Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetPosition(sf::Vector2f& pos) override;

private:
  sf::Text staticText;
};

class Toggle : public Interactable
{
public:
  Toggle(int isToggled);

  bool Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetPosition(sf::Vector2f& pos) override;

private:
  sf::Sprite toggleSprite;
};

class Range : public Interactable
{
public:
  Range(int value, int min, int max);

  bool Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetPosition(sf::Vector2f& pos) override;

private:
  int min = -1;
  int max = -1;
  sf::Text rangeText;
};

class Selection : public Interactable
{
public:
  Selection(int index, std::vector<std::string>& selections);

  bool Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetPosition(sf::Vector2f& pos) override;

private:
  std::vector<std::string> selections;
  sf::Text selectionText;
};

class Control : public Interactable
{
public:
  Control(int keyCode);

  bool Update() override;
  void Render(sf::RenderWindow* win) const override;

  void SetPosition(sf::Vector2f& pos) override;

private:
  RoundedRect keyBg;
  sf::Text controlText;
};

class ListItem
{
public:
  ListItem(std::string name, float vertOffset, Interactable* interactable);

  bool Update();
  void Render(sf::RenderWindow* win) const;

  bool IsActive() const;

  void SetPosition(sf::Vector2f pos);
  float GetVerticalOffset() const;

private:
  std::unique_ptr<Interactable> interactable = nullptr;

  Event event;

  sf::Text displayName;

  bool isActive = false;

  float vertOffset = 0.0f;

};

// `ListItem` is a base class which is used to represent an user configurable list menu item 
// class ListItem
// {
// public:
//   // Constructs ListItem with its identifier/display name
//   ListItem(std::string name);

//   virtual void Update();
//   virtual void Render(sf::RenderWindow* win) const;

//   void ToggleHighlight();
//   bool IsActive();

//   void SetOffset(float offset);
//   virtual void SetPosition(sf::Vector2f pos);

// // public:
// //   static ListItem* curHighlight;
  
// protected:
//   sf::Text displayName;
//   // static RoundedRect highlightBg;

//   bool isHighlighted = false;

//   Event action;

//   float vertOffset = 0.0f;

//   bool isActive = false;
// };

// class StaticText : public ListItem
// {
// public:
//   StaticText(std::string name, std::string text);

//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// private:
//   sf::Text staticText;
// };

// class ToggleOption : public ListItem
// {
// public:
//   ToggleOption(std::string name, bool init);

//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// private:
//   bool toggle = false;
//   sf::Sprite toggleSprite;
// };

// class RangeOption : public ListItem
// {
// public:
//   RangeOption(std::string name, int val, int min, int max);

//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// private:
//   int value = 0;
//   int min = 0;
//   int max = 0;
//   sf::Text displayRange;
// };

// class SelectionOption : public ListItem
// {
// public:
//   SelectionOption(std::string name, int index, std::vector<std::string> selections);

//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// private:
//   int index;
//   std::vector<std::string> selections;
//   sf::Text selectionDisplay;
// };

// class ControlOption : public ListItem
// {
// public:
//   ControlOption(std::string name, sf::Keyboard::Key key);

//   void Update() override;
//   void Render(sf::RenderWindow* win) const override;

// private:
//   RoundedRect keyBg;
//   sf::Text curKey;
// };

#endif