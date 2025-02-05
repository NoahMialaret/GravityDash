#include "MenuInterface.h"

MenuInterface::MenuInterface(Event menuReturn)
  :
  menuReturn(menuReturn)
{}

void MenuInterface::Update()
{  
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
    PUSH_EVENT(menuReturn);
}


GridInterface::GridInterface(int initialHighlight, std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  MenuInterface(menuReturn),
  curButton(initialHighlight)
{
  assert(configs.size() > 0);

  const float padding = 0.5f * SPRITE_DIM;
  float horiPos = -padding; // The horizontal position of the next column of buttons

  for (int i = 0; i < (int)configs.size(); i++)
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
      if (i + 1 < (int)configs.size() && configs[i + 1].size == MEDIUM)
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
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    buttons[grid[curButton]].Click();
    return;
  }
  MenuInterface::Update();

  int xMove = Settings::GetInstance()->IsActionClicked(Controls::Action::right) 
              - Settings::GetInstance()->IsActionClicked(Controls::Action::left);
  int yMove = Settings::GetInstance()->IsActionClicked(Controls::Action::down) 
              - Settings::GetInstance()->IsActionClicked(Controls::Action::up);

  // Returns if no movement is happening, or movement would be out-of-bounds
  if ((!xMove && !yMove) 
      || (curButton <= 1 && xMove < 0) 
      || (curButton >= (int)grid.size() - 2 && xMove > 0)
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


VerticalInterface::VerticalInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  MenuInterface(menuReturn)
{
  StaticButton button(configs[0]);
  float offset = button.GetHeight() + 2.0f;
  float pos = 0.5f * offset * (float)(configs.size() - 1);

  for (auto& c : configs)
  {
    buttons.push_back({c, centre + sf::Vector2f(pos, -pos)});
    pos -= offset;
  }

  assert(buttons.size() <= 6);

  buttons[curButton].ToggleHighlight();
}

void VerticalInterface::Update()
{
  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::select))
  {
    buttons[curButton].Click();
    return;
  }

  MenuInterface::Update();

  int move = Settings::GetInstance()->IsActionClicked(Controls::Action::down) 
             - Settings::GetInstance()->IsActionClicked(Controls::Action::up);

  if (!move || buttons.size() == 1)
    return;

  int nextButton = curButton + move;

  if (nextButton < 0)
    nextButton = buttons.size() - 1;
  else if (nextButton >= (int)buttons.size())
    nextButton = 0;

  buttons[curButton].ToggleHighlight();
  buttons[nextButton].ToggleHighlight();
  curButton = nextButton; 
}

void VerticalInterface::Render(sf::RenderWindow* win) const
{
  for (auto& b : buttons)
    b.Render(win);
}


GameEndInterface::GameEndInterface(std::vector<StaticButtonInit>& configs, Event menuReturn, sf::Vector2f centre)
  :
  VerticalInterface(configs, menuReturn, centre)
{
  for (auto& b : buttons)
    b.Move({4.0f * SPRITE_DIM, 0});

  float yPos = centre.y - (40.0f + (GameStats::localStats.timeBoosts == -1 ? 0.0f : 7.0f)) / 2.0f;

  Utility::InitText(displayTitle, LARGE_FONT, "results", 
                    {centre.x - 6.5f * SPRITE_DIM, yPos - SPRITE_DIM}, 
                    {0.0f, 0.0f}, {255, 229, 181});
  displayTitle.drawable.setOutlineColor({173, 103, 78});
  displayTitle.drawable.setOutlineThickness(1.0f);

  underline.setFillColor({173, 103, 78});
  underline.setSize(sf::Vector2f(displayTitle.drawable.getLocalBounds().width + 6, 1.0f));
  underline.setPosition(displayTitle.drawable.getPosition() + sf::Vector2f(-3.0f, 17.0f));

  float offset = 7;

  Text text;
  Utility::InitText(text, SMALL_FONT, "jumps - " + std::to_string(GameStats::localStats.jumps), {centre.x - 6.0f * SPRITE_DIM, yPos + 14.0f}, {0, 0.0f}, {255, 229, 181});
  text.drawable.setOutlineColor({173, 103, 78});
  text.drawable.setOutlineThickness(1.0f);

  this->stats.push_back(text);

  text.drawable.setString("hits - " + std::to_string(GameStats::localStats.hits));
  text.drawable.move({0.0f, offset});
  this->stats.push_back(text);
  text.drawable.setString("specials - " + std::to_string(GameStats::localStats.specials));
  text.drawable.move({0.0f, offset});
  this->stats.push_back(text);
  text.drawable.setString("3+ combos - " + std::to_string(GameStats::localStats.combos));
  text.drawable.move({0.0f, offset});
  this->stats.push_back(text);

  if (GameStats::localStats.timeBoosts == -1)
    return;

  text.drawable.setString("cycles - " + std::to_string(GameStats::localStats.timeBoosts));
  text.drawable.move({0.0f, offset});
  this->stats.push_back(text);
}

void GameEndInterface::Render(sf::RenderWindow* win) const
{
  VerticalInterface::Render(win);
  Utility::RenderTextWithScale(win, displayTitle, nullptr);
  Utility::RenderRectWithScale(win, underline, nullptr);
  for (auto& s : stats)
    Utility::RenderTextWithScale(win, s, nullptr);
}



Header::Header(std::string text, float vertOffset)
  :
  vertOffset(vertOffset)
{
  Utility::InitText(displayTitle, LARGE_FONT, text, ZERO_VECTOR);

  float width = displayTitle.drawable.getLocalBounds().width + 4;

  overline.setFillColor({173, 103, 78});
  overline.setSize(sf::Vector2f(width, 1.0f));
  overline.setOrigin({width / 2.0f, 5.0f});

  underline.setFillColor({173, 103, 78});
  underline.setSize(sf::Vector2f(width, 1.0f));
  underline.setOrigin({width / 2.0f, - 4.0f});
}

void Header::Render(sf::RenderWindow* win) const
{
  Utility::RenderTextWithScale(win, displayTitle, nullptr);
  Utility::RenderRectWithScale(win, overline, nullptr);
  Utility::RenderRectWithScale(win, underline, nullptr);
}

void Header::SetPosition(sf::Vector2f pos)
{
  pos.y += vertOffset - 1.0f;

  underline.setPosition(pos);
  overline.setPosition(pos);

  pos.y += - SPRITE_DIM - 1.0f;
  displayTitle.drawable.setPosition(pos);
}



ListInterface::ListInterface(std::vector<std::pair<std::string, Interactable*>>& inters, std::vector<std::pair<int, std::string>>& headers, Event menuReturn)
  :
  MenuInterface(menuReturn),
  translation(&origin),
  highlight({{0,0}, {2.0f * LIST_MARGIN + 2.0f, 6.0f}, sf::Color(245, 204, 164)})
{
  origin = ZERO_VECTOR;

  float offset = 0;

  int headerI = 0;

  for (int i = 0; i < (int)inters.size(); i++)
  {
    if (headerI < (int)headers.size() && headers[headerI].first == i)
    {
      offset += SPRITE_DIM;
      this->headers.push_back(Header(headers[headerI].second, offset));
      offset += SPRITE_DIM;
      headerI++;
    }

    list.push_back(ListItem(inters[i].first, offset, inters[i].second));
    offset += SPRITE_DIM;
  }

  origin.y = -list[curIndex].GetVerticalOffset();
  UpdateAllPositions();
}

void ListInterface::Update()
{
  if (translation.Update())
    UpdateAllPositions();

  if (list[curIndex].Update())
  {
    highlight.SetColour(sf::Color(230, 176, 138));
    return;
  }

  if (Settings::GetInstance()->IsActionOnInitialClick(Controls::Action::escape))
    PUSH_EVENT(menuReturn);

  highlight.SetColour(sf::Color(245, 204, 164));

  int move = Settings::GetInstance()->IsActionClicked(Controls::Action::down)  
              - Settings::GetInstance()->IsActionClicked(Controls::Action::up);

  if (!move)
    return;

  curIndex += move;
  if (curIndex < 0)
    curIndex = list.size() - 1;
  else if (curIndex >= (int)list.size())
    curIndex = 0;

  translation.Clear();
  translation.Push(EASE_IN_CURVE, 250, origin, {origin.x, -list[curIndex].GetVerticalOffset()});
}

void ListInterface::Render(sf::RenderWindow* win) const
{
  highlight.Render(win);

  for (auto& item : list)
    item.Render(win);
  
  for (auto& header : headers)
    header.Render(win);
}

void ListInterface::UpdateAllPositions()
{
  for (auto& item : list)
    item.SetPosition(origin);
  
  for (auto& header : headers)
    header.SetPosition(origin);

  highlight.SetVertical(origin.y + list[curIndex].GetVerticalOffset());
}