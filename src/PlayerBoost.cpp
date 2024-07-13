#include "PlayerBoost.h"
sf::Texture PlayerBoost::tex;

PlayerBoost::PlayerBoost(int numTabs, sf::Vector2f topRight)
{

  for (int i = 0; i < numTabs; i++)
  {
    sf::Sprite temp;

    temp.setTexture(tex);
    temp.setScale(DEFAULT_SCALE);
    temp.setPosition(topRight + (float)i * sf::Vector2f(0.0f, (1.0f + temp.getTextureRect().height) * Utility::gameScale));
    temp.setOrigin(sf::Vector2f(temp.getTextureRect().width, 0.0f));

    sf::RectangleShape tempRect;

    tempRect.setPosition(temp.getPosition() + Utility::gameScale * sf::Vector2f(0.0f, 1.0f));
    tempRect.setScale(sf::Vector2f(-1.0f, 1.0f));
    tempRect.setFillColor(sf::Color(255, 229, 181));

    temp.move(sf::Vector2f(3.0f * Utility::gameScale, 0.0f));

    tabs.push_back(temp);
    tabTimer.push_back(tempRect);
  }
}

void PlayerBoost::Update()
{
  if (boostAmount <= 0 || IsFull())
  {
    lastFrameTime = CUR_TIME;
    return;
  }

  timer -= CUR_TIME - lastFrameTime;
  lastFrameTime = CUR_TIME;

  if (timer <= 0)
  {
    Decrement();
    return;
  }


  int width = (timer / 1000) + 1;

  tabTimer[boostAmount - 1].setSize(Utility::gameScale * sf::Vector2f((float)width, 4.0f));
}

void PlayerBoost::Render(sf::RenderWindow *win) const
{
  for (int i = 0; i < tabs.size(); i++)
  {
    win->draw(tabs[i]);
    win->draw(tabTimer[i]);
  }
}

void PlayerBoost::Increment()
{
  if (IsFull())
  {
    return;
  }

  tabTimer[boostAmount].setSize(Utility::gameScale * sf::Vector2f(4.0f, 4.0f));
  tabs[boostAmount].move(sf::Vector2f(- 3.0f * Utility::gameScale, 0.0f));
  timer = 4000;

  if (boostAmount > 0)
  {
    tabTimer[boostAmount - 1].setSize(Utility::gameScale * sf::Vector2f(4.0f, 4.0f));    
  }

  boostAmount++;
}

void PlayerBoost::Decrement()
{
  if (boostAmount <= 0)
  {
    return;
  }
  
  timer = 4000;

  tabTimer[boostAmount - 1].setSize(ZERO_VECTOR);
  tabs[boostAmount - 1].move(sf::Vector2f(3.0f * Utility::gameScale, 0.0f));
  boostAmount--;
}

void PlayerBoost::Clear()
{
  for (int i = 0; i < boostAmount; i++)
  {
    tabTimer[i].setSize(ZERO_VECTOR);
    tabs[i].move(sf::Vector2f(3.0f * Utility::gameScale, 0.0f));
  }
  boostAmount = 0;
}

bool PlayerBoost::IsFull() const
{
  return boostAmount >= tabs.size();
}
