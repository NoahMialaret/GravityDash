#include "ConfigMenuType.h"

ConfigMenuType::ConfigMenuType(std::string name, std::vector<std::string> tabNames)
  :
  SubMenu("")
{
  int numTabs = tabNames.size();
  elements.resize(numTabs);
  // TODO: Use Camera class to determine top of screen
  // TODO: Support more than 4 tabs on one screen

  if (!tabTex.loadFromFile("assets/Tabs.png")) 
  {
    std::cout << "\tTab textures could not be loaded!\n";
  }

  if (!toggleTex.loadFromFile("assets/Toggle.png")) 
  {
    std::cout << "\tToggle texture could not be loaded!\n";
  }
  if (!arrowTex.loadFromFile("assets/Arrow.png")) 
  {
    std::cout << "\tArrow texture could not be loaded!\n";
  }

  tabButton.setTexture(tabTex);
  tabButton.setScale(DEFAULT_SCALE);
  tabButton.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), TAB_TEX_SIZE));
  tabButton.setOrigin(sf::Vector2f((float)TAB_TEX_SIZE.x / 2, float(TAB_TEX_SIZE.y - 1) / 2));

  float offset = (numTabs - 1) * 4 * SCALED_DIM / 2;
  tabButton.setPosition(sf::Vector2f(- offset, -220.0f));

  sf::Vector2f textPos = tabButton.getPosition();
  for (auto name : tabNames)
  {
    sf::Text newText(name, Utility::programFont);
    newText.setCharacterSize(SCALED_DIM);
    newText.setFillColor(sf::Color(173, 103, 78));
    newText.setPosition(textPos);

    sf::FloatRect textBounds = newText.getLocalBounds();
    newText.setOrigin(sf::Vector2f(textBounds.left + textBounds.width / 2, textBounds.top + textBounds.height / 2));

    textPos += sf::Vector2f((TAB_TEX_SIZE.x - Utility::spriteDim + 1) * Utility::gameScale , 0.0f);

    tabTexts.push_back(newText);
  }

  tabTexts[0].move(sf::Vector2f(0.0f, Utility::gameScale));
}

// void ConfigMenuType::AddElement(std::unique_ptr<Element>& element, int tabIndex)
// {
//   float offset = 12.0f * Utility::gameScale * elements[tabIndex].size();
//   element.get()->Move(offset);
//   elements[tabIndex].push_back(std::move(element));
// }

void ConfigMenuType::Update()
{
  int elementMove = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);
  if (elementMove != 0 && elementIndex + elementMove >= 0 && elementIndex + elementMove < elements[tabIndex].size())
  {
    float offset = 12.0f * Utility::gameScale * elementMove;
    for (auto& e : elements[tabIndex])
    {
      e.get()->Move(-offset);
    }
    elementIndex += elementMove;
  }  

  int tabMove = Utility::CheckInitialPress(sf::Keyboard::E) - Utility::CheckInitialPress(sf::Keyboard::Q);
  if (tabMove != 0 && tabIndex + tabMove >= 0 && tabIndex + tabMove < tabTexts.size())
  {
    tabTexts[tabIndex].move(sf::Vector2f(0.0f, - Utility::gameScale));
    float offset = 12.0f * Utility::gameScale * elementIndex;
    for (auto& e : elements[tabIndex])
    {
      e.get()->Move(offset);
    }
    tabIndex += tabMove;
    tabTexts[tabIndex].move(sf::Vector2f(0.0f, Utility::gameScale));
    elementIndex = 0;
  }

  elements[tabIndex][elementIndex].get()->Update();
  // if tabs are switched
  // move all elements in current tab by - SCROLL_VALUE * elementIndex to reset position
}

void ConfigMenuType::Render(sf::RenderWindow* win) const
{
  sf::Sprite tabSprite(tabButton);

  for (int i = std::max(elementIndex - 2, 0); i < std::min(elementIndex + 3, (int)elements[tabIndex].size()); i++)
  {
    elements[tabIndex][i].get()->Render(win);
  }

  for (size_t i = 0; i < tabTexts.size(); i++)
  {
    if (i == tabIndex)
    {
      sf::IntRect newTexRect = tabSprite.getTextureRect();
      newTexRect.top += TAB_TEX_SIZE.y;
      tabSprite.setTextureRect(newTexRect);

    }
    
    win->draw(tabSprite);
    win->draw(tabTexts[i]);

    if (i == tabIndex)
    {
      tabSprite.setTextureRect(tabButton.getTextureRect());
    }
    tabSprite.move(sf::Vector2f((TAB_TEX_SIZE.x - Utility::spriteDim + 1) * Utility::gameScale , 0.0f));
  }


  SubMenu::Render(win);
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
  text.setString(name);
  text.setFont(Utility::programFont);
  text.setCharacterSize(SCALED_DIM);
  text.setFillColor(sf::Color(173, 103, 78));
  text.setPosition(sf::Vector2f(-50 * Utility::gameScale, 0.0f));

  sf::FloatRect textBounds = text.getLocalBounds();
  text.setOrigin(sf::Vector2f(textBounds.left, textBounds.top + textBounds.height / 2));
}

void ConfigMenuType::Element::Move(float moveAmount)
{
  text.move(sf::Vector2f(- moveAmount, moveAmount));
}

std::string ConfigMenuType::Element::GetName() const
{
  return text.getString();
}

int ConfigMenuType::Element::GetValue() const
{
  return value;
}

void ConfigMenuType::Element::Render(sf::RenderWindow* win) const
{
  win->draw(text);
}



ConfigMenuType::Toggle::Toggle(std::string name, bool isToggled, float offset, sf::Texture* tex)
  :
  Element(name, (int)isToggled)
{
  toggleSprite.setTexture(*tex);
  toggleSprite.setScale(DEFAULT_SCALE);
  toggleSprite.setPosition(sf::Vector2f(50 * Utility::gameScale, 0.0f));
  toggleSprite.setTextureRect(sf::IntRect(sf::Vector2i(isToggled * 8, 0), sf::Vector2i(8, 5)));
  toggleSprite.setOrigin(sf::Vector2f(8, 2.5));
  Move(offset);
}

void ConfigMenuType::Toggle::Move(float moveAmount)
{
  toggleSprite.move(sf::Vector2f(- moveAmount, moveAmount));
  Element::Move(moveAmount);
}

void ConfigMenuType::Toggle::Update()
{
  if (value == 0 && Utility::CheckInitialPress(sf::Keyboard::D))
  {
    value = 1;
    toggleSprite.setTextureRect(sf::IntRect(sf::Vector2i(8, 0), sf::Vector2i(8, 5)));
  }
  else if (value == 1 && Utility::CheckInitialPress(sf::Keyboard::A))
  {
    value = 0;
    toggleSprite.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(8, 5)));
  }
}

void ConfigMenuType::Toggle::Render(sf::RenderWindow* win) const
{
  win->draw(toggleSprite);
  Element::Render(win);
}




ConfigMenuType::Range::Range(std::string name, int value, float offset, sf::Texture *tex)
  :
  Element(name, value)
{
  arrowSpriteRight.setTexture(*tex);
  arrowSpriteRight.setScale(DEFAULT_SCALE);
  arrowSpriteRight.setPosition(sf::Vector2f(50 * Utility::gameScale, 0.0f));
  arrowSpriteRight.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(4, 5)));
  arrowSpriteRight.setOrigin(sf::Vector2f(4, 2.5));

  arrowSpriteLeft.setTexture(*tex);
  arrowSpriteLeft.setScale(sf::Vector2f(-Utility::gameScale, Utility::gameScale));
  arrowSpriteLeft.setPosition(sf::Vector2f(41 * Utility::gameScale, 0.0f));
  arrowSpriteLeft.setTextureRect(sf::IntRect(sf::Vector2i(0, 0), sf::Vector2i(4, 5)));
  arrowSpriteLeft.setOrigin(sf::Vector2f(0, 2.5));

  valueText.setString(Utility::IntToString(value));
  valueText.setFont(Utility::programFont);
  valueText.setCharacterSize(SCALED_DIM);
  valueText.setFillColor(sf::Color(173, 103, 78));
  valueText.setPosition(sf::Vector2f(45 * Utility::gameScale, 0.0f));

  sf::FloatRect textBounds = valueText.getLocalBounds();
  valueText.setOrigin(sf::Vector2f(textBounds.left + textBounds.width, textBounds.top + textBounds.height / 2));

  Move(offset);
}

void ConfigMenuType::Range::Move(float moveAmount)
{
  arrowSpriteRight.move(sf::Vector2f(- moveAmount, moveAmount));
  arrowSpriteLeft.move(sf::Vector2f(- moveAmount, moveAmount));
  valueText.move(sf::Vector2f(- moveAmount, moveAmount));
  Element::Move(moveAmount);
}

void ConfigMenuType::Range::Update()
{
  if (value < 9 && Utility::CheckInitialPress(sf::Keyboard::D))
  {
    value++;
    valueText.setString(Utility::IntToString(value));
  }
  else if (value > 0 && Utility::CheckInitialPress(sf::Keyboard::A))
  {
    value--;
    valueText.setString(Utility::IntToString(value));
  }  
}

void ConfigMenuType::Range::Render(sf::RenderWindow *win) const
{
  win->draw(arrowSpriteRight);
  win->draw(arrowSpriteLeft);
  win->draw(valueText);
  Element::Render(win);
}
