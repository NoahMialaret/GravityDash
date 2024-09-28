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
    Event::events.push_back(menuReturn);
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

ListInterface::ListInterface(std::vector<ButtonConfig>& configs, Event menuReturn)
  :
  MenuInterface(menuReturn)
{
  assert (configs.size() > 0);

  float offset = Utility::gameScale * (Textures::textures.at("small_button").getSize().y + 2);
  float pos = offset * (configs.size() - 1) / 2;

  for (auto& c : configs)
  {
    buttons.push_back(std::make_unique<SmallButton>(sf::Vector2f(pos, -pos), c.name, c.event));
    pos -= offset;
  }

  assert (buttons.size() <= 6);

  buttons[curPos].get()->ToggleHighlight();
}

void ListInterface::Update()
{
  if (Utility::CheckInitialPress(sf::Keyboard::Space))
  {
    buttons[curPos].get()->Click();
  }
  else if (Utility::CheckInitialPress(sf::Keyboard::Escape))
  {
    Event::events.push_back(menuReturn);
    curPos = 0;
  }

  int move = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);

  int newPos = curPos + move;

  if (!move || newPos < 0 || newPos >= buttons.size())
    return;

  buttons[curPos].get()->ToggleHighlight();
  buttons[newPos].get()->ToggleHighlight();
  curPos = newPos; 
}

void ListInterface::Render(sf::RenderWindow *win) const
{
  for (auto& b : buttons)
    b.get()->Render(win);
}

OptionsInterface::OptionsInterface(std::vector<OptionConfig> configs, Event menuReturn)
  :
  MenuInterface(menuReturn)
{
  assert (configs.size() > 0);

  // float offset = Utility::gameScale * (Textures::textures.at("small_button").getSize().y + 2);
  float yPos = 0;

  for (auto& c : configs)
  {
    switch (c.type)
    {
    case OptionConfig::Type::toggle:
      options.push_back(std::make_unique<ToggleOption>(c.name, c.event, yPos, c.toggle));
      break;
    case OptionConfig::Type::range:
      options.push_back(std::make_unique<RangeOption>(c.name, c.event, yPos, c.range));
      break;
    case OptionConfig::Type::selection:
      options.push_back(std::make_unique<SelectionOption>(c.name, c.event, yPos, c.selection));
      break;
    case OptionConfig::Type::control:
      // options.push_back(std::make_unique<ControlOption>(c.name, c.event, yPos, c.control));
      break;
    
    default:
      break;
    }
    yPos += SCALED_DIM;
  }

  options[curIndex].get()->ToggleHighlight();
}

void OptionsInterface::Update()
{

  if (Utility::CheckInitialPress(sf::Keyboard::Escape))
    Event::events.push_back(menuReturn);

  for (auto& o : options)
    o.get()->Update();

  int move = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);

  int newIndex = curIndex + move;

  if (!move || newIndex < 0 || newIndex >= options.size())
    return;

  options[curIndex].get()->ToggleHighlight();
  options[newIndex].get()->ToggleHighlight();
  curIndex = newIndex; 

  for (auto& o : options)
    o.get()->Move(-move * SCALED_DIM);
}

void OptionsInterface::Render(sf::RenderWindow* win) const
{
  for (auto& o : options)
    o.get()->Render(win);
}