#include "GridMenu.h"

sf::Texture GridMenu::buttonTex;

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
    if (!buttonTex.loadFromFile(filePath)) 
    {
        std::cout << "\tButton textures could not be loaded!\n";
    }
}

void GridMenu::Update()
{
    sf::Vector2i dir;

    dir.x = Utility::CheckInitialPress(sf::Keyboard::D) - Utility::CheckInitialPress(sf::Keyboard::A);
    dir.y = Utility::CheckInitialPress(sf::Keyboard::S) - Utility::CheckInitialPress(sf::Keyboard::W);

    if (dir == sf::Vector2i(0, 0))
    {
        return;
    }
    
    sf::Vector2i originalPos = selectorPos;

    bool foundButton = false;
    while (!foundButton)
    {
        if (!IsInBoundsAxis(selectorPos + dir).y)
        {
            selectorPos.y = dir.y > 0 ? -1 : GRID_HEIGHT;
            sf::IntRect buttonBounds = buttons[curButtonIndex].GetBounds();
            selectorPos.x += originalPos.x > buttonBounds.left + buttonBounds.width / 2 ? -1 : 1;
        }
        if (!IsInBoundsAxis(selectorPos + dir).x)
        {
            // sf::Vector2f topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
            // renderRect.setPosition(topLeft + SCALED_DIM * (sf::Vector2f)selectorPos);
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

    // sf::Vector2f topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
    // renderRect.setPosition(topLeft + SCALED_DIM * (sf::Vector2f)selectorPos);
}

void GridMenu::Render(sf::RenderWindow* win) const
{
    for (auto& b : buttons)
    {
        b.Render(win);
    }

    SubMenu::Render(win);

    //win->draw(renderRect);
}

void GridMenu::CreateButton(std::string name, sf::IntRect gridBounds)
{
    buttons.push_back(Button(buttonTex, name, gridBounds));

    if (curButtonIndex == -1)
    {
        curButtonIndex = 0;
        selectorPos = {gridBounds.left, gridBounds.top};
        buttons[curButtonIndex].ToggleHighlight();

        // sf::Vector2f topLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
        // sf::Vector2f relativePos(selectorPos.x * SCALED_DIM, selectorPos.y * SCALED_DIM);

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

bool GridMenu::IsInBounds(sf::Vector2i gridPos)
{
    return gridPos.x >= 0 && gridPos.x < GRID_WIDTH && gridPos.y >= 0 && gridPos.y < GRID_HEIGHT;
}

sf::Vector2i GridMenu::IsInBoundsAxis(sf::Vector2i gridPos)
{
    return {gridPos.x >= 0 && gridPos.x < GRID_WIDTH, gridPos.y >= 0 && gridPos.y < GRID_HEIGHT};
}

GridMenu::Button::Button(sf::Texture& tex, std::string buttonName, sf::IntRect gridBounds)
    :
    gridBounds(gridBounds)
{
    name.setString(buttonName);

    sf::Sprite temp;

    sf::Vector2f menuTopLeft(-GRID_WIDTH * (SCALED_DIM / 2.0f), -GRID_HEIGHT * (SCALED_DIM / 2.0f));
    sf::Vector2f relativeTopLeft(gridBounds.left * SCALED_DIM, gridBounds.top * SCALED_DIM);

    temp.setPosition(menuTopLeft + relativeTopLeft);
    temp.setScale(DEFAULT_SCALE);
    temp.setTexture(tex);

    sf::IntRect texRect(0, 0, Utility::spriteDim, Utility::spriteDim * gridBounds.height);
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

    temp.setTextureRect(texRect);

    sprites.push_back(temp);

    texRect.left += Utility::spriteDim;

    if (buttonName == "Quit" || buttonName == "Return")
    {
        temp.move({SCALED_DIM, 0});
        temp.setTextureRect(texRect);

        sprites.push_back(temp);

        return;
    }

    if (isLarge)
    {
        temp.move({SCALED_DIM, 0});
        temp.setTextureRect(texRect);

        sprites.push_back(temp);

        texRect.left += Utility::spriteDim;
    }

    for (int i = 1 + isLarge; i < gridBounds.width - 1 - isLarge; i++)
    {
        temp.move({SCALED_DIM, 0});

        temp.setTextureRect(texRect);

        sprites.push_back(temp);
    }

    temp.move({SCALED_DIM, 0});
    texRect.left += Utility::spriteDim;

    temp.setTextureRect(texRect);

    sprites.push_back(temp);

    if (isLarge)
    {
        temp.move({SCALED_DIM, 0});
        texRect.left += Utility::spriteDim;

        temp.setTextureRect(texRect);

        sprites.push_back(temp);
    }

    name.setFont(Utility::programFont);
    name.setCharacterSize(SCALED_DIM);
    name.setFillColor(sf::Color(173, 103, 78));
    name.setOrigin(name.getLocalBounds().width, name.getLocalBounds().top);
    sf::Vector2f pos(menuTopLeft.x + relativeTopLeft.x + gridBounds.width * SCALED_DIM - 2 * Utility::gameScale,
        menuTopLeft.y + relativeTopLeft.y + 2 * Utility::gameScale);
    name.setPosition(pos);
}

GridMenu::Button::Button(const Button& button)
{
    name = button.name;
    gridBounds = button.gridBounds;
    isHighlighted = button.isHighlighted;

    tex = button.tex;
    
    sprites = button.sprites;
}

bool GridMenu::Button::OccupiesGridPos(sf::Vector2i gridPos) const
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

    for (auto& s : sprites)
    {
        sf::IntRect texRect = s.getTextureRect();
        texRect.left += (8 * Utility::spriteDim) * (isHighlighted ? 1.0f : -1.0f);  
        s.setTextureRect(texRect); 
    }
}

std::string GridMenu::Button::GetName() const
{
    return name.getString();
}

sf::IntRect GridMenu::Button::GetBounds() const
{
    return gridBounds;
}

void GridMenu::Button::Render(sf::RenderWindow* win) const
{
    for (auto& s : sprites)
    {
        win->draw(s);
    }

    win->draw(name);
}
