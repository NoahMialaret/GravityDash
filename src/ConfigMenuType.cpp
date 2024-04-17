#include "ConfigMenuType.hpp"

ConfigMenuType::ConfigMenuType(std::string name, std::vector<std::string> tabNames)
  :
  SubMenu(""),
  tabTexts(tabNames)
{
  int numTabs = tabNames.size();
  elements.resize(numTabs);
  // TODO: Use Camera class to determine top of screen
  // TODO: Support more than 4 tabs on one screen

  tabTex.LoadFromFile("assets/Tabs.png");
  toggleTex.LoadFromFile("assets/Toggle.png");
  arrowTex.LoadFromFile("assets/Arrow.png");

  tabButton.SetTexture(&tabTex);
  tabButton.SetScale(DEFAULT_SCALE);
  tabButton.SetTextureBounds(IRect(iVec2(0, 0), TAB_TEX_SIZE));
  tabButton.SetOrigin(fVec2((float)TAB_TEX_SIZE.x / 2, float(TAB_TEX_SIZE.y - 1) / 2));

  float offset = (numTabs - 1) * 4 * SCALED_DIM / 2;
  tabButton.SetTranslation(fVec2(- offset, -220.0f));

  // fVec2 textPos = tabButton.GetTranslation();
  // for (auto name : tabNames)
  // {
  //   sf::Text newText(name, Utility::programFont);
  //   newText.setCharacterSize(SCALED_DIM);
  //   newText.setFillColor(sf::Color(173, 103, 78));
  //   newText.setPosition(textPos);

  //   sf::FloatRect textBounds = newText.getLocalBounds();
  //   newText.setOrigin(fVec2(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2));

  //   textPos += fVec2((TAB_TEX_SIZE.x - Utility::spriteDim + 1) * Utility::gameScale , 0.0f);

  //   tabTexts.push_back(newText);
  // }

  // tabTexts[0].move(fVec2(0.0f, Utility::gameScale));
}

// void ConfigMenuType::AddElement(std::unique_ptr<Element>& element, int tabIndex)
// {
//   float offset = 12.0f * Utility::gameScale * elements[tabIndex].size();
//   element.get()->Move(offset);
//   elements[tabIndex].push_back(std::move(element));
// }

void ConfigMenuType::Update()
{
  int elementMove = Utility::CheckInitialPress(GLFW_KEY_S) - Utility::CheckInitialPress(GLFW_KEY_W);
  if (elementMove != 0 && elementIndex + elementMove >= 0 && elementIndex + elementMove < elements[tabIndex].size())
  {
    float offset = 12.0f * Utility::gameScale * elementMove;
    for (auto& e : elements[tabIndex])
    {
      e.get()->Move(-offset);
    }
    elementIndex += elementMove;
  }  

  int tabMove = Utility::CheckInitialPress(GLFW_KEY_E) - Utility::CheckInitialPress(GLFW_KEY_Q);
  if (tabMove != 0 && tabIndex + tabMove >= 0 && tabIndex + tabMove < tabTexts.size())
  {
    // tabTexts[tabIndex].move(fVec2(0.0f, - Utility::gameScale));
    float offset = 12.0f * Utility::gameScale * elementIndex;
    for (auto& e : elements[tabIndex])
    {
      e.get()->Move(offset);
    }
    tabIndex += tabMove;
    // tabTexts[tabIndex].move(fVec2(0.0f, Utility::gameScale));
    elementIndex = 0;
  }

  elements[tabIndex][elementIndex].get()->Update();
  // if tabs are switched
  // move all elements in current tab by - SCROLL_VALUE * elementIndex to reset position
}

void ConfigMenuType::Render() const
{
  Sprite tabSprite(tabButton);

  for (int i = std::max(elementIndex - 2, 0); i < std::min(elementIndex + 3, (int)elements[tabIndex].size()); i++)
  {
    elements[tabIndex][i].get()->Render();
  }

  for (size_t i = 0; i < tabTexts.size(); i++)
  {
    if (i == tabIndex)
    {
      IRect newTexRect = tabSprite.GetTextureBounds();
      newTexRect.top += TAB_TEX_SIZE.y;
      tabSprite.SetTextureBounds(newTexRect);

    }
    
    tabSprite.Render();
    // win->draw(tabTexts[i]);

    if (i == tabIndex)
    {
      tabSprite.SetTextureBounds(tabButton.GetTextureBounds());
    }
    tabSprite.Translate(fVec2((TAB_TEX_SIZE.x - Utility::spriteDim + 1) * Utility::gameScale , 0.0f));
  }


  SubMenu::Render();
}

std::map<std::string, int> ConfigMenuType::GetConfigs() const
{
  std::map<std::string, int> config;
  for (auto& tab : elements)
  {
    for (auto& element : tab)
    {
      config.emplace(element.get()->GetName(), element.get()->GetValue());
    }
  }

  return config;
}

void ConfigMenuType::CreateToggle(std::string name, bool isToggled, int tabIndex)
{
  float offset = 12.0f * Utility::gameScale * elements[tabIndex].size();
  elements[tabIndex].push_back(std::make_unique<Toggle>(name, isToggled, offset, &toggleTex));
}

void ConfigMenuType::CreateRange(std::string name, int value, int tabIndex)
{
  value = std::clamp(value, 0, 9);
  float offset = 12.0f * Utility::gameScale * elements[tabIndex].size();
  elements[tabIndex].push_back(std::make_unique<Range>(name, value, offset, &arrowTex));
}

ConfigMenuType::Element::Element(std::string name, int initValue)
  :
  value(initValue)
{
  // text.setString(name);
  // text.setFont(Utility::programFont);
  // text.setCharacterSize(SCALED_DIM);
  // text.setFillColor(sf::Color(173, 103, 78));
  // text.setPosition(fVec2(-50 * Utility::gameScale, 0.0f));

  // sf::FloatRect textBounds = text.getLocalBounds();
  // text.setOrigin(fVec2(textBounds.left, textBounds.top + textBounds.height / 2));
}

void ConfigMenuType::Element::Move(float moveAmount)
{
  //text.move(fVec2(- moveAmount, moveAmount));
}

std::string ConfigMenuType::Element::GetName() const
{
  return NULL; //text.getString();
}

int ConfigMenuType::Element::GetValue() const
{
  return value;
}

void ConfigMenuType::Element::Render() const
{
  //win->draw(text);
}



ConfigMenuType::Toggle::Toggle(std::string name, bool isToggled, float offset, Texture* tex)
  :
  Element(name, (int)isToggled)
{
  toggleSprite.SetTexture(tex);
  toggleSprite.SetScale(DEFAULT_SCALE);
  toggleSprite.SetTranslation(fVec2(50 * Utility::gameScale, 0.0f));
  toggleSprite.SetTextureBounds(IRect(iVec2(isToggled * 8, 0), iVec2(8, 5)));
  toggleSprite.SetOrigin(fVec2(8, 2.5));
  Move(offset);
}

void ConfigMenuType::Toggle::Move(float moveAmount)
{
  toggleSprite.Translate(fVec2(- moveAmount, moveAmount));
  Element::Move(moveAmount);
}

void ConfigMenuType::Toggle::Update()
{
  if (value == 0 && Utility::CheckInitialPress(GLFW_KEY_D))
  {
    value = 1;
    toggleSprite.SetTextureBounds(IRect(iVec2(8, 0), iVec2(8, 5)));
  }
  else if (value == 1 && Utility::CheckInitialPress(GLFW_KEY_A))
  {
    value = 0;
    toggleSprite.SetTextureBounds(IRect(iVec2(0, 0), iVec2(8, 5)));
  }
}

void ConfigMenuType::Toggle::Render() const
{
  toggleSprite.Render();
  Element::Render();
}




ConfigMenuType::Range::Range(std::string name, int value, float offset, Texture* tex)
  :
  Element(name, value)
{
  arrowSpriteRight.SetTexture(tex);
  arrowSpriteRight.SetScale(DEFAULT_SCALE);
  arrowSpriteRight.SetTranslation(fVec2(50 * Utility::gameScale, 0.0f));
  arrowSpriteRight.SetTextureBounds(IRect(iVec2(0, 0), iVec2(4, 5)));
  arrowSpriteRight.SetOrigin(fVec2(4, 2.5));

  arrowSpriteLeft.SetTexture(tex);
  arrowSpriteLeft.SetScale(fVec2(-Utility::gameScale, Utility::gameScale));
  arrowSpriteLeft.SetTranslation(fVec2(41 * Utility::gameScale, 0.0f));
  arrowSpriteLeft.SetTextureBounds(IRect(iVec2(0, 0), iVec2(4, 5)));
  arrowSpriteLeft.SetOrigin(fVec2(0, 2.5));

  // valueText.setString(Utility::IntToString(value));
  // valueText.setFont(Utility::programFont);
  // valueText.setCharacterSize(SCALED_DIM);
  // valueText.setFillColor(sf::Color(173, 103, 78));
  // valueText.setPosition(fVec2(45 * Utility::gameScale, 0.0f));

  // FRect textBounds = valueText.getLocalBounds();
  // valueText.setOrigin(fVec2(textBounds.left + textBounds.width, textBounds.top + textBounds.height / 2));

  Move(offset);
}

void ConfigMenuType::Range::Move(float moveAmount)
{
  arrowSpriteRight.Translate(fVec2(- moveAmount, moveAmount));
  arrowSpriteLeft.Translate(fVec2(- moveAmount, moveAmount));
  // valueText.move(fVec2(- moveAmount, moveAmount));
  Element::Move(moveAmount);
}

void ConfigMenuType::Range::Update()
{
  if (value < 9 && Utility::CheckInitialPress(GLFW_KEY_D))
  {
    value++;
    //valueText.setString(Utility::IntToString(value));
  }
  else if (value > 0 && Utility::CheckInitialPress(GLFW_KEY_A))
  {
    value--;
    //valueText.setString(Utility::IntToString(value));
  }  
}

void ConfigMenuType::Range::Render() const
{
  arrowSpriteRight.Render();
  arrowSpriteLeft.Render();
  // win->draw(valueText);
  Element::Render();
}
