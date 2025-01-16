#include "MenuInterface.h"

MenuInterface::MenuInterface(Event menuReturn)
  :
  menuReturn(menuReturn)
{}

void MenuInterface::Update()
{  
  Controls* controls = ProgramSettings::GetControls();
  if (controls->IsBindingOnInitialClick(Controls::Binding::escape))
    Event::events.push(menuReturn);
}


GridInterface::GridInterface(int initialHighlight, std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  MenuInterface(menuReturn),
  curButton(initialHighlight)
{
  assert(configs.size() > 0);

  const float padding = 0.5f * SCALED_DIM;
  float horiPos = -padding; // The horizontal position of the next column of buttons

  for (int i = 0; i < configs.size(); i++)
  {
    grid.push_back(i); 
    StaticButton button(configs[i]);

    horiPos += padding;
    if (i != 0)
      horiPos += 0.5f * button.GetWidth(); 

    button.Move({horiPos, configs[i].size != LARGE ? -GRID_VERT_POS : 0.0f});
    buttons.push_back(button);

    assert(configs[i].size != SMALL);
    if (configs[i].size == MEDIUM)
    {      
      // Checks if there is there another button that can be placed underneath
      if (i + 1 < configs.size() && configs[i + 1].size == MEDIUM)
      {
        i++;
        grid.push_back(i);
        button = StaticButton(configs[i]);
        button.Move({horiPos, GRID_VERT_POS});
        buttons.push_back(button);
      }
      else
        grid.push_back(-1);
    }
    else // LARGE
      grid.push_back(i);

    horiPos += 0.5f * button.GetWidth();
  }

  float offset = 0.5f * horiPos;
  offset -= 0.25f * buttons[0].GetWidth();

  // Centre the buttons to the middle of the screen
  for (auto& button : buttons)
    button.Move(centre + sf::Vector2f(-offset, 0));

  assert (grid.size() <= 6);

  buttons[grid[curButton]].ToggleHighlight();
}

void GridInterface::Update()
{
  Controls* controls = ProgramSettings::GetControls();
  if (controls->IsBindingOnInitialClick(Controls::Binding::select))
  {
    buttons[grid[curButton]].Click();
    return;
  }
  MenuInterface::Update();

  int xMove = controls->IsBindingClicked(Controls::Binding::right) 
              - controls->IsBindingClicked(Controls::Binding::left);
  int yMove = controls->IsBindingClicked(Controls::Binding::down) 
              - controls->IsBindingClicked(Controls::Binding::up);

  // Returns if no movement is happening, or movement would be out-of-bounds
  if ((!xMove && !yMove) 
      || (curButton <= 1 && xMove < 0) 
      || (curButton >= grid.size() - 2 && xMove > 0)
      || (curButton % 2 == 0 && yMove < 0) 
      || (curButton % 2 == 1 && yMove > 0))
    return;

  int nextButton = curButton + yMove + 2 * xMove;

  if (grid[nextButton] == -1)
    nextButton--;

  // Different button
  if (grid[curButton] != grid[nextButton])
  {
    buttons[grid[curButton]].ToggleHighlight();
    buttons[grid[nextButton]].ToggleHighlight();
  }

  curButton = nextButton; 
}

void GridInterface::Render(sf::RenderWindow* win) const
{
  for (auto& b : buttons)
    b.Render(win);
}


ListInterface::ListInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  MenuInterface(menuReturn)
{
  StaticButton button(configs[0]);
  float offset = button.GetHeight() + 2.0f * ProgramSettings::gameScale;
  float pos = 0.5f * offset * (configs.size() - 1);

  for (auto& c : configs)
  {
    buttons.push_back({c, centre + sf::Vector2f(pos, -pos)});
    pos -= offset;
  }

  assert(buttons.size() <= 6);

  buttons[curButton].ToggleHighlight();
}

void ListInterface::Update()
{
  Controls* controls = ProgramSettings::GetControls();
  if (controls->IsBindingOnInitialClick(Controls::Binding::select))
  {
    buttons[curButton].Click();
    return;
  }

  MenuInterface::Update();

  int move = controls->IsBindingClicked(Controls::Binding::down) 
             - controls->IsBindingClicked(Controls::Binding::up);

  if (!move || buttons.size() == 1)
    return;

  int nextButton = curButton + move;

  if (nextButton < 0)
    nextButton = buttons.size() - 1;
  else if (nextButton >= buttons.size())
    nextButton = 0;

  buttons[curButton].ToggleHighlight();
  buttons[nextButton].ToggleHighlight();
  curButton = nextButton; 
}

void ListInterface::Render(sf::RenderWindow* win) const
{
  for (auto& b : buttons)
    b.Render(win);
}


GameEndInterface::GameEndInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  ListInterface(configs, menuReturn, centre)
{
  for (auto& b : buttons)
    b.Move({4.0f * SCALED_DIM, 0});

  float yPos = centre.y - ProgramSettings::gameScale * (40.0f + (GameStats::localStats.timeBoosts == -1 ? 0.0f : 7.0f)) / 2.0f;

  Utility::InitText(displayTitle, Textures::large, "results", {centre.x - 6.5f * SCALED_DIM, yPos - SCALED_DIM}, {0, 0.0f}, {255, 229, 181});
  displayTitle.setOutlineColor({173, 103, 78});
  displayTitle.setOutlineThickness(ProgramSettings::gameScale);

  underline.setFillColor({173, 103, 78});
  underline.setSize(sf::Vector2f(displayTitle.getLocalBounds().width / ProgramSettings::gameScale + 6, 1.0f));
  underline.setScale(DEFAULT_SCALE);
  underline.setPosition(displayTitle.getPosition() + ProgramSettings::gameScale * sf::Vector2f(-3.0f, 17.0f));

  float offset = 7 * ProgramSettings::gameScale;

  sf::Text text;
  Utility::InitText(text, Textures::small, "jumps - " + std::to_string(GameStats::localStats.jumps), {centre.x - 6.0f * SCALED_DIM, yPos + 14.0f * ProgramSettings::gameScale}, {0, 0.0f}, {255, 229, 181});
  text.setOutlineColor({173, 103, 78});
  text.setOutlineThickness(ProgramSettings::gameScale);

  this->stats.push_back(text);

  text.setString("hits - " + std::to_string(GameStats::localStats.hits));
  text.move({0.0f, offset});
  this->stats.push_back(text);
  text.setString("specials - " + std::to_string(GameStats::localStats.specials));
  text.move({0.0f, offset});
  this->stats.push_back(text);
  text.setString("3+ combos - " + std::to_string(GameStats::localStats.combos));
  text.move({0.0f, offset});
  this->stats.push_back(text);

  if (GameStats::localStats.timeBoosts == -1)
    return;

  text.setString("cycles - " + std::to_string(GameStats::localStats.timeBoosts));
  text.move({0.0f, offset});
  this->stats.push_back(text);

}

void GameEndInterface::Render(sf::RenderWindow* win) const
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
  timer += DELTA_TIME;

  Controls* controls = ProgramSettings::GetControls();
  if (controls->IsBindingOnInitialClick(Controls::Binding::escape))
    Event::events.push(menuReturn);

  float percent = bezier.GetValue(timer / 250.0f);
  origin = (1.0f - percent) * start + percent * end;

  int move = controls->IsBindingClicked(Controls::Binding::down) - controls->IsBindingClicked(Controls::Binding::up);

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

  Utility::InitText(displayTitle, Textures::large, title, {0, *origin + yPos - SCALED_DIM - ProgramSettings::gameScale});

  float width = displayTitle.getLocalBounds().width / ProgramSettings::gameScale + 4;

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
  displayTitle.setPosition({0, *origin + vertOffset - SCALED_DIM - ProgramSettings::gameScale});
  overline.setPosition({0, *origin + vertOffset});
  underline.setPosition({0, *origin + vertOffset});

  for (auto& o : options)
    o.get()->Update();
}

void OptionsSubList::Render(sf::RenderWindow* win) const
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