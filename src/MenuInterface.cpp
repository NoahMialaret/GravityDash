#include "MenuInterface.h"

MenuInterface::MenuInterface(Event menuReturn)
  :
  menuReturn(menuReturn)
{}

// GridInterface::GridInterface(Layout layout)
//   :
//   MenuInterface()
// {
//   switch (layout)
//   {
//   case Layout::main:
//     buttons.push_back(std::make_unique<MediumButton>("stats"));
//     break;

//   case Layout::play:
  
//   default:
//     break;
//   }
//   // if button is medium, pushback, if button is large, pushback * 2
// }

GridInterface::GridInterface(int startPos, std::vector<ButtonConfig>& configs, Event menuReturn)
  :
  MenuInterface(menuReturn),
  curPos(startPos)
{
  assert (configs.size() > 0);

  float hori = 0;
  float padding = SCALED_DIM * (configs[0].size == 1 ? 2.0f : 2.5f);
  float width = -0.5f; // The amount of sprite space the buttons take up (including inbetween)

  for (int i = 0; i < configs.size(); i++)
  {
    buttonPos.push_back(i); 

    if (configs[i].size == 1)
    {
      width += 4.5f;
      buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{hori, -GRID_VERT}, configs[i].name, configs[i].event));
      if (i + 1 < configs.size() && configs[i + 1].size == 1)
      {
        i++;
        buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{hori, GRID_VERT}, configs[i].name, configs[i].event));
        buttonPos.push_back(i);
      }
      else
        buttonPos.push_back(-1);
      hori += GRID_HORI;
    }
    else
    {
      width += 5.5f;

      if (i != 0)
        hori += 0.5f * SCALED_DIM;

      buttonPos.push_back(i);
      buttons.push_back(std::make_unique<LargeButton>(sf::Vector2f{hori, 0}, configs[i].name, configs[i].event));
      
      hori += GRID_HORI + 0.5f * SCALED_DIM;
    }
  }

  for (auto& button : buttons)
    button.get()->Move(sf::Vector2f(padding - (width * SCALED_DIM) / 2, 0));

  assert (buttonPos.size() <= 6);

  buttons[buttonPos[startPos]].get()->ToggleHighlight();
}

void GridInterface::Update()
{
  if (Utility::CheckInitialPress(sf::Keyboard::Space))
  {
    buttons[buttonPos[curPos]].get()->Click();
  }
  else if (Utility::CheckInitialPress(sf::Keyboard::Escape))
  {
    Utility::events.push_back(menuReturn);
  }

  int xMove = Utility::CheckInitialPress(sf::Keyboard::D) - Utility::CheckInitialPress(sf::Keyboard::A);
  int yMove = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);

  if ((!xMove && !yMove) ||
      (curPos <= 1 && xMove < 0) ||
      (curPos >= buttonPos.size() - 2 && xMove > 0) ||
      (curPos % 2 == 0 && yMove < 0) ||
      (curPos % 2 == 1 && yMove > 0)) // If no movement is happening, or movement would be out-of-bounds
    return;

  int newPos = curPos + yMove + xMove * 2;

  if (buttonPos[newPos] == -1)
    newPos--;

  if (buttonPos[curPos] != buttonPos[newPos]) // If it is a different button
  {
    buttons[buttonPos[curPos]].get()->ToggleHighlight();
    buttons[buttonPos[newPos]].get()->ToggleHighlight();
  }

  curPos = newPos; 
}

void GridInterface::Render(sf::RenderWindow* win) const
{
  for (auto& b : buttons)
    b.get()->Render(win);
}

// PauseInterface::PauseInterface()
//   :
//   Menu()
// {
//   sf::Vector2f startPos(0.0f, -100.0f);
//   sf::Vector2f offset(0.0f, 50.0f);

//   buttons.push_back(std::make_unique<SmallButton>("Continue"));
//   buttons.push_back(std::make_unique<SmallButton>("Restart"));
//   buttons.push_back(std::make_unique<SmallButton>("Settings"));
//   buttons.push_back(std::make_unique<SmallButton>("Exit"));
//   buttons[0].get()->ToggleHighlight();
//   curButtonIndex = 0;
// }
