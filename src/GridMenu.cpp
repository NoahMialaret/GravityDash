#include "GridMenu.hpp"

Texture GridMenu::buttonTex;

GridMenu::GridMenu(std::string name, int numButtons)
    : 
    SubMenu(name)
{
  buttons.reserve(numButtons);
  // renderRect.setSize({SCALED_DIM, SCALED_DIM});
  // renderRect.setOutlineThickness(-Utility::gameScale);
  // renderRect.setOutlineColor(sf::Color(200, 0, 0));
}

void GridMenu::SetTexture(const char* filePath)
{
  buttonTex.LoadFromFile(filePath);
}

void GridMenu::Update()
{
  iVec2 dir;

  dir.x = Utility::CheckInitialPress(GLFW_KEY_D) - Utility::CheckInitialPress(GLFW_KEY_A);
  dir.y = Utility::CheckInitialPress(GLFW_KEY_S) - Utility::CheckInitialPress(GLFW_KEY_W);

  if (dir == iVec2(0, 0))
  {
    return;
  }

  iVec2 originalPos = selectorPos;

  bool foundButton = false;
  while (!foundButton)
  {
    if (!IsInBoundsAxis(selectorPos + dir).y)
    {
      selectorPos.y = dir.y > 0 ? -1 : GRID_HEIGHT;
      IRect buttonBounds = buttons[curButtonIndex].GetBounds();
      selectorPos.x += originalPos.x > buttonBounds.left + buttonBounds.width / 2 ? -1 : 1;
    }
    if (!IsInBoundsAxis(selectorPos + dir).x)
    {
      // fVec2 topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
      // renderRect.setPosition(topLeft + SCALED_DIM * (fVec2)selectorPos);
      return;
    }

    selectorPos += dir;

    for (int i = 0; i < buttons.size(); i++)
    {
      if (i == curButtonIndex || !buttons[i].OccupiesGridPos(selectorPos))
      {
        continue;
      }

      buttons[curButtonIndex].ToggleHighlight();
      curButtonIndex = i;
      buttons[curButtonIndex].ToggleHighlight();

      foundButton = true;

      break;
    }
  }

  // fVec2 topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
  // renderRect.setPosition(topLeft + SCALED_DIM * (fVec2)selectorPos);
}

void GridMenu::Render() const
{
  for (auto &b : buttons)
  {
    b.Render();
  }

  SubMenu::Render();

  // win->draw(renderRect);
}

void GridMenu::CreateButton(std::string name, IRect gridBounds)
{
  buttons.push_back(Button(buttonTex, name, gridBounds));

  if (curButtonIndex == -1)
  {
    curButtonIndex = 0;
    selectorPos = {gridBounds.left, gridBounds.top};
    buttons[curButtonIndex].ToggleHighlight();

    // fVec2 topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
    // fVec2 relativePos(selectorPos.x * SCALED_DIM, selectorPos.y * SCALED_DIM);

    // renderRect.setPosition(topLeft + relativePos);
  }
}

std::string GridMenu::Select() const
{
  if (curButtonIndex == -1)
  {
    std::cout << "ERROR: There is no button selected!\n";
    return "";
  }

  std::cout << "Selecting the " << buttons[curButtonIndex].GetName() << " button.\n";

  return buttons[curButtonIndex].GetName();
}

bool GridMenu::IsInBounds(iVec2 gridPos)
{
  return gridPos.x >= 0 && gridPos.x < GRID_WIDTH && gridPos.y >= 0 && gridPos.y < GRID_HEIGHT;
}

iVec2 GridMenu::IsInBoundsAxis(iVec2 gridPos)
{
  return {gridPos.x >= 0 && gridPos.x < GRID_WIDTH, gridPos.y >= 0 && gridPos.y < GRID_HEIGHT};
}

GridMenu::Button::Button(Texture& tex, std::string buttonName, IRect gridBounds)
  : 
  gridBounds(gridBounds),
  name(buttonName)
{
  // name.setString(buttonName);

  Sprite temp;

  fVec2 menuTopLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
  fVec2 relativeTopLeft(gridBounds.left * SCALED_DIM, gridBounds.top * SCALED_DIM);

  temp.SetTranslation(menuTopLeft + relativeTopLeft);
  temp.SetScale(DEFAULT_SCALE);
  temp.SetTexture(&tex);

  IRect texRect(0, 0, Utility::spriteDim, Utility::spriteDim * gridBounds.height);
  texRect.top = gridBounds.height == 3 ? 0 : 3 * Utility::spriteDim;

  bool isLarge = (gridBounds.width >= 6) && (gridBounds.height == 3);

  if (isLarge || buttonName == "Quit")
  {
    texRect.left = 3 * Utility::spriteDim;
  }
  else if (buttonName == "Return")
  {
    texRect.left = 5 * Utility::spriteDim;
  }

  temp.SetTextureBounds(texRect);

  sprites.push_back(temp);

  texRect.left += Utility::spriteDim;

  if (buttonName == "Quit" || buttonName == "Return")
  {
    temp.Translate({SCALED_DIM, 0});
    temp.SetTextureBounds(texRect);

    sprites.push_back(temp);

    return;
  }

  if (isLarge)
  {
    temp.Translate({SCALED_DIM, 0});
    temp.SetTextureBounds(texRect);

    sprites.push_back(temp);

    texRect.left += Utility::spriteDim;
  }

  for (int i = 1 + isLarge; i < gridBounds.width - 1 - isLarge; i++)
  {
    temp.Translate({SCALED_DIM, 0});

    temp.SetTextureBounds(texRect);

    sprites.push_back(temp);
  }

  temp.Translate({SCALED_DIM, 0});
  texRect.left += Utility::spriteDim;

  temp.SetTextureBounds(texRect);

  sprites.push_back(temp);

  if (isLarge)
  {
    temp.Translate({SCALED_DIM, 0});
    texRect.left += Utility::spriteDim;

    temp.SetTextureBounds(texRect);

    sprites.push_back(temp);
  }

  // name.setFont(Utility::programFont);
  // name.setCharacterSize(SCALED_DIM);
  // name.setFillColor(sf::Color(173, 103, 78));
  // name.setOrigin(name.getLocalBounds().width, name.getLocalBounds().top);
  // fVec2 pos(menuTopLeft.x + relativeTopLeft.x + gridBounds.width * SCALED_DIM - 2 * Utility::gameScale,
  //           menuTopLeft.y + relativeTopLeft.y + 2 * Utility::gameScale);
  // name.setPosition(pos);
}

GridMenu::Button::Button(const Button &button)
{
  name = button.name;
  gridBounds = button.gridBounds;
  isHighlighted = button.isHighlighted;

  sprites = button.sprites;
}

bool GridMenu::Button::OccupiesGridPos(iVec2 gridPos) const
{
  if (gridPos.x < gridBounds.left || gridPos.x >= gridBounds.left + gridBounds.width)
  {
    return false;
  }
  if (gridPos.y < gridBounds.top || gridPos.y >= gridBounds.top + gridBounds.height)
  {
    return false;
  }

  return true;
}

void GridMenu::Button::ToggleHighlight()
{
  isHighlighted = !isHighlighted;

  for (auto &s : sprites)
  {
    IRect texRect = s.GetTextureBounds();
    texRect.left += (8 * Utility::spriteDim) * (isHighlighted ? 1.0f : -1.0f);
    s.SetTextureBounds(texRect);
  }
}

std::string GridMenu::Button::GetName() const
{
  return name;
}

IRect GridMenu::Button::GetBounds() const
{
  return gridBounds;
}

void GridMenu::Button::Render() const
{
  for (auto &s : sprites)
  {
    s.Render();;
  }

  //win->draw(name);
}
