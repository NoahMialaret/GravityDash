#include "MenuInterface.h"

MenuInterface::MenuInterface(Event menuReturn)
  :
  menuReturn(menuReturn)
{}

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

  if (!move)
    return;

  int newPos = curPos + move;

  if (newPos < 0)
    newPos = buttons.size() - 1;
  else if (newPos >= buttons.size())
    newPos = 0;

  buttons[curPos].get()->ToggleHighlight();
  buttons[newPos].get()->ToggleHighlight();
  curPos = newPos; 
}

void ListInterface::Render(sf::RenderWindow *win) const
{
  for (auto& b : buttons)
    b.get()->Render(win);
}

GameEndInterface::GameEndInterface(std::vector<ButtonConfig> &configs, Event menuReturn, Event::GameStats stats)
  :
  ListInterface(configs, menuReturn)
{
  for (auto& b : buttons)
  {
    b.get()->Move({4.0f * SCALED_DIM, 0});
  }

  float yPos = - Utility::gameScale * (40.0f + (stats.cycles == -1 ? 0.0f : 7.0f)) / 2.0f;

  Utility::InitText(displayTitle, Textures::large, "results", {-6.5f * SCALED_DIM, yPos - SCALED_DIM}, {0, 0.0f}, {255, 229, 181});
  displayTitle.setOutlineColor({173, 103, 78});
  displayTitle.setOutlineThickness(Utility::gameScale);

  underline.setFillColor({173, 103, 78});
  underline.setSize(sf::Vector2f(displayTitle.getLocalBounds().width / Utility::gameScale + 6, 1.0f));
  underline.setScale(DEFAULT_SCALE);
  underline.setPosition(displayTitle.getPosition() + Utility::gameScale * sf::Vector2f(-3.0f, 17.0f));

  float offset = 7 * Utility::gameScale;

  sf::Text text;
  Utility::InitText(text, Textures::small, "jumps - " + std::to_string(stats.jumps), {-6.0f * SCALED_DIM, yPos + 14.0f * Utility::gameScale}, {0, 0.0f}, {255, 229, 181});
  text.setOutlineColor({173, 103, 78});
  text.setOutlineThickness(Utility::gameScale);

  this->stats.push_back(text);

  text.setString("hits - " + std::to_string(stats.hits));
  text.move({0.0f, offset});
  this->stats.push_back(text);
  text.setString("specials - " + std::to_string(stats.specials));
  text.move({0.0f, offset});
  this->stats.push_back(text);
  text.setString("3+ combos - " + std::to_string(stats.combos));
  text.move({0.0f, offset});
  this->stats.push_back(text);

  if (stats.cycles == -1)
    return;

  text.setString("cycles - " + std::to_string(stats.cycles));
  text.move({0.0f, offset});
  this->stats.push_back(text);

}

void GameEndInterface::Render(sf::RenderWindow *win) const
{
  ListInterface::Render(win);
  win->draw(displayTitle);
  win->draw(underline);
  for (auto& s : stats)
    win->draw(s);
}



OptionsInterface::OptionsInterface(std::vector<std::pair<std::string, std::vector<OptionConfig>>>& configs, Event menuReturn)
  :
  MenuInterface(menuReturn),
  bezier(Utility::curves[(int)Curve::easeIn])
{
  origin = 0.0f;

  float yPos = 0.0f;
  for (auto& c : configs)
  {
    subLists.push_back(std::make_unique<OptionsSubList>(c.first, c.second, &origin, yPos));
    yPos += (c.second.size() + 2) * SCALED_DIM;
  }

  subLists[0].get()->GoTo(0);
  timer = 0.0f;
  start = origin;
  end = - MenuOption::curHighlight->GetOffset();
}

OptionsInterface::~OptionsInterface()
{
  MenuOption::curHighlight = nullptr;
}

void OptionsInterface::Update()
{
  timer += Clock::Delta();

  if (Utility::CheckInitialPress(sf::Keyboard::Escape))
    Event::events.push_back(menuReturn);

  float percent = bezier.GetValue(timer / 250.0f);
  origin = (1.0f - percent) * start + percent * end;

  int move = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);

  if (!move)
  {
    for (auto& l : subLists)
      l.get()->Update();
    return;
  }

  if (!subLists[curIndex].get()->Move(move))
  {
    curIndex += move;
    if (curIndex < 0)
      curIndex = subLists.size() - 1;
    else if (curIndex >= subLists.size())
      curIndex = 0;

    subLists[curIndex].get()->GoTo(move == 1 ? 0 : -1);
  }

  timer = 0.0f;
  start = origin;
  end = - MenuOption::curHighlight->GetOffset();
  
  for (auto& l : subLists)
    l.get()->Update();
}

void OptionsInterface::Render(sf::RenderWindow* win) const
{
  for (auto& l : subLists)
    l.get()->Render(win);
}

OptionsSubList::OptionsSubList(std::string& title, std::vector<OptionConfig>& configs, float* origin, float yPos)
  :
  origin(origin),
  vertOffset(yPos)
{
  assert (configs.size() > 0);

  Utility::InitText(displayTitle, Textures::large, title, {0, *origin + yPos - SCALED_DIM - Utility::gameScale});

  float width = displayTitle.getLocalBounds().width / Utility::gameScale + 4;

  overline.setFillColor({173, 103, 78});
  overline.setSize(sf::Vector2f(width, 1.0f));
  overline.setScale(DEFAULT_SCALE);
  overline.setPosition({0, *origin + yPos});
  overline.setOrigin({width / 2.0f, 5.0f});

  underline.setFillColor({173, 103, 78});
  underline.setSize(sf::Vector2f(width, 1.0f));
  underline.setScale(DEFAULT_SCALE);
  underline.setPosition({0, *origin + yPos});
  underline.setOrigin({width / 2.0f, - 4.0f});

  yPos += 1.25f * SCALED_DIM;

  for (auto& c : configs)
  {
    switch (c.type)
    {
    case OptionConfig::Type::stat:
      options.push_back(std::make_unique<StaticText>(c.name, c.event, origin, yPos, c.statText));
      break;
    case OptionConfig::Type::toggle:
      options.push_back(std::make_unique<ToggleOption>(c.name, c.event, origin, yPos, c.toggle));
      break;
    case OptionConfig::Type::range:
      options.push_back(std::make_unique<RangeOption>(c.name, c.event, origin, yPos, c.range));
      break;
    case OptionConfig::Type::selection:
      options.push_back(std::make_unique<SelectionOption>(c.name, c.event, origin, yPos, c.selection));
      break;
    case OptionConfig::Type::control:
      options.push_back(std::make_unique<ControlOption>(c.name, c.event, origin, yPos, c.control));
      break;
    
    default:
      break;
    }
    yPos += SCALED_DIM;
  }
}

void OptionsSubList::Update()
{
  displayTitle.setPosition({0, *origin + vertOffset - SCALED_DIM - Utility::gameScale});
  overline.setPosition({0, *origin + vertOffset});
  underline.setPosition({0, *origin + vertOffset});

  for (auto& o : options)
    o.get()->Update();
}

void OptionsSubList::Render(sf::RenderWindow *win) const
{
  win->draw(displayTitle);
  win->draw(overline);
  win->draw(underline);

  for (auto& o : options)
    o.get()->Render(win);
}

void OptionsSubList::GoTo(int index)
{
  curIndex = index;
  if (curIndex == -1)
    curIndex = options.size() - 1;
  options[curIndex].get()->SetHighlight();
}

bool OptionsSubList::Move(int move)
{
  if (curIndex + move < 0 || curIndex + move >= options.size())
    return false;

  curIndex += move;
  options[curIndex].get()->SetHighlight();

  return true;
}