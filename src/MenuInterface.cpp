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

GridInterface::GridInterface(std::vector<ButtonConfig>& configs, Event menuReturn)
  :
  MenuInterface(menuReturn)
{
  float hori = -GRID_HORI;
  
  for (int i = 0; i < configs.size(); i++)
  {
    buttonPos.push_back(i); 

    if (configs[i].size == 1)
    {
      buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{hori, -GRID_VERT}, configs[i].name, configs[i].event));
      i++;
      buttons.push_back(std::make_unique<MediumButton>(sf::Vector2f{hori, GRID_VERT}, configs[i].name, configs[i].event));
      hori += GRID_HORI;
    }
    else
    {
      for (auto& b : buttons)
        b.get()->Move(sf::Vector2f{-0.5f * SCALED_DIM, 0.0f});

      buttons.push_back(std::make_unique<LargeButton>(sf::Vector2f{hori, 0}, configs[i].name, configs[i].event));
      hori += GRID_HORI + 0.5 * SCALED_DIM;
    }

    buttonPos.push_back(i);
  }

  assert (buttonPos.size() == 6);

  buttons[buttonPos[curPos]].get()->ToggleHighlight();
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
      (curPos >= 4 && xMove > 0) ||
      (curPos % 2 == 0 && yMove < 0) ||
      (curPos % 2 == 1 && yMove > 0)) // If no movement is happening, or movement would be out-of-bounds
    return;

  int newPos = curPos + yMove + xMove * 2;

  if (buttonPos[curPos] == buttonPos[newPos]) // If it is the same button
  {
    curPos = newPos;
    return;
  }

  buttons[buttonPos[curPos]].get()->ToggleHighlight();
  buttons[buttonPos[newPos]].get()->ToggleHighlight();
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
