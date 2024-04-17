#ifndef CONFIG_MENU_TYPE_HPP
#define CONFIG_MENU_TYPE_HPP

#define TAB_TEX_SIZE iVec2(39, 10)
#define SCROLL_OFFSET fVec2(20.0f, -20.0f)

#include "Sprite.hpp"
#include "SubMenu.hpp"
#include "Texture.hpp"
#include "Utility.hpp"

#include <iostream>
#include <map>

// Used for menus that show multiple lists of elements whose value can be changed
class ConfigMenuType : public SubMenu
{
protected:
  class Element
  {
  public:
    Element() = default;
    virtual ~Element() = default;
    Element(std::string name, int initValue);
    // Element(const Element& button);

    virtual void Move(float moveAmount);

    // void ToggleHighlight();
    std::string GetName() const;
    virtual int GetValue() const;
    // Handles updating the value of this element if it has been selected
    virtual void Update() = 0;

    virtual void Render() const;

  protected:
    // sf::Text text; // The name of the button to be displayed
    int value = 0; // The value of the setting/config this element represents

    bool isHighlighted = false;
  };

  class Toggle : public Element
  {
  public:
    Toggle(std::string name, bool isToggled, float offset, Texture* tex);

    void Move(float moveAmount) override;

    void Update() override;
    void Render() const override;

  private:
    Sprite toggleSprite;
  };

  class Range : public Element
  {
  public:
    Range(std::string name, int value, float offset, Texture* tex);

    void Move(float moveAmount) override;

    void Update() override;
    void Render() const override;

  private:
    Sprite arrowSpriteLeft;
    Sprite arrowSpriteRight;
    //sf::Text valueText;
    // arrow button sprites + extra sf::text for value
  };

  class Select : public Element
  {
  };

  // class numInput

  class KeyBinding : public Element
  {
  };

public:
  ConfigMenuType(std::string name, std::vector<std::string> tabNames);
  // void AddElement(std::unique_ptr<Element>& element, int tabIndex);

  void Update() override; // will have events for if settings are saved, exited, etc
  void Render() const override;

  std::map<std::string, int> GetConfigs() const;

protected:
  void CreateToggle(std::string name, bool isSelected, int tabIndex);
  void CreateRange(std::string name, int value, int tabIndex);

private:
  int tabIndex = 0;
  int elementIndex = 0;

  //std::vector<sf::Text> tabTexts;
  std::vector<std::string> tabTexts;
  Texture tabTex;
  Sprite tabButton;

  Texture toggleTex;
  Texture arrowTex;

  std::vector<std::vector<std::unique_ptr<Element>>> elements;
};

#endif