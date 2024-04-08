#include "tempMenu.h"

Title::Title()
    :
    GridMenu("GravDash", 5)
{
    CreateButton("Play", {0, 0, 8, 3});
    CreateButton("Null", {9, 0, 5, 3});
    CreateButton("Scores", {0, 4, 5, 2});
    CreateButton("Options", {6, 4, 5, 2});
    CreateButton("Quit", {12, 4, 2, 2});
}

std::unique_ptr<SubMenu> Title::ClickButton()
{
    std::string button = Select();

    if (button == "Play")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<GameSelection>();
        return newMenu;
    }
    if (button == "Null")
    {}
    if (button == "Scores")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<ScoresMenu>();
        return newMenu;
    }
    if (button == "Options")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<Options>();
        return newMenu;
    }
    if (button == "Quit")
    {
        Event newEvent;
        newEvent.type = Event::Type::programClose;
        Utility::events.push_back(newEvent);
    }
    return nullptr;
}

std::unique_ptr<SubMenu> Title::Return()
{
    Event newEvent;
    newEvent.type = Event::Type::programClose;
    Utility::events.push_back(newEvent);
    return nullptr;
}

GameSelection::GameSelection()
    :
    GridMenu("Game Selection", 5)
{
    CreateButton("Normal", {0, 0, 6, 3});
    CreateButton("Simple", {8, 0, 6, 3});
    CreateButton("Multi", {0, 4, 5, 2});
    CreateButton("Custom", {6, 4, 5, 2});
    CreateButton("Return", {12, 4, 2, 2});
}

std::unique_ptr<SubMenu> GameSelection::ClickButton()
{
    std::string button = Select();

    if (button == "Normal")
    {
        Event newEvent;
        newEvent.type = Event::Type::loadNewGame;
        Utility::events.push_back(newEvent);
    }
    if (button == "Simple")
    {
        Event newEvent;
        newEvent.type = Event::Type::loadNewGame;
        Utility::events.push_back(newEvent);
    }
    if (button == "Multi")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<MultiplayerMenu>();
        return newMenu;
    }
    if (button == "Custom")
    {
        // list menu
    }
    if (button == "Return")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<Title>();
        return newMenu;
    }
    return nullptr;
}

std::unique_ptr<SubMenu> GameSelection::Return()
{
    std::unique_ptr<SubMenu> newMenu = std::make_unique<Title>();
    return newMenu;
}



MultiplayerMenu::MultiplayerMenu()
    :
    GridMenu("Multiplayer", 5)
{
    CreateButton("Co-op", {0, 0, 6, 3});
    CreateButton("Versus", {8, 0, 6, 3});
    CreateButton("Null", {0, 4, 5, 2});
    CreateButton("Custom", {6, 4, 5, 2});
    CreateButton("Return", {12, 4, 2, 2});
}

std::unique_ptr<SubMenu> MultiplayerMenu::ClickButton()
{
    std::string button = Select();

    if (button == "Co-op")
    {
        Event newEvent;
        newEvent.type = Event::Type::loadNewGame;
        Utility::events.push_back(newEvent);

        gameConfig.numCharacters = 2;
    }
    if (button == "Versus")
    {
    }
    if (button == "Null")
    {
    }
    if (button == "Custom")
    {
        // list menu
    }
    if (button == "Return")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<GameSelection>();
        return newMenu;
    }
    return nullptr;
}

std::unique_ptr<SubMenu> MultiplayerMenu::Return()
{
    std::unique_ptr<SubMenu> newMenu = std::make_unique<GameSelection>();
    return newMenu;
}

// -----------------------------

ScoresMenu::ScoresMenu()
    :
    GridMenu("Scores", 4)
{
    auto func = [this](){
    CreateButton("Leaderboards", {0, 0, 14, 3});
    CreateButton("Stats", {0, 4, 5, 2});
    CreateButton("Medals", {6, 4, 5, 2});
    CreateButton("Return", {12, 4, 2, 2});};

    func();
}

std::unique_ptr<SubMenu> ScoresMenu::ClickButton()
{
    std::string button = Select();

    if (button == "Leaderboards")
    {
    }
    if (button == "Stats")
    {
    }
    if (button == "Medals")
    {
    }
    if (button == "Return")
    {
        std::unique_ptr<SubMenu> newMenu = std::make_unique<Title>();
        return newMenu;
    }
    return nullptr;
}

std::unique_ptr<SubMenu> ScoresMenu::Return()
{
    std::unique_ptr<SubMenu> newMenu = std::make_unique<Title>();
    return newMenu;
}

Options::Options()
    :
    ConfigMenuType("Options", std::vector<std::string>{"Hello", "Video", "Keys", "Misc."})
{
    Event newEvent;
    newEvent.type = Event::Type::cameraUp;
    Utility::events.push_back(newEvent);

    for (int i = 0; i < 4; i++)
    {
        CreateToggle("1st Toggle " + Utility::IntToString(i), true, i);
        CreateToggle("2nd Toggle " + Utility::IntToString(i), false, i);
        CreateToggle("3rd Toggle " + Utility::IntToString(i), false, i);
        CreateRange("4th Range " + Utility::IntToString(i), 4, i);
        CreateToggle("5th Toggle " + Utility::IntToString(i), true, i);
        CreateToggle("6th Toggle " + Utility::IntToString(i), false, i);
        CreateRange("7th Range " + Utility::IntToString(i), 9, i);
        CreateToggle("8th Toggle " + Utility::IntToString(i), true, i);
    }
}

std::unique_ptr<SubMenu> Options::ClickButton()
{
  return std::unique_ptr<SubMenu>();
}

std::unique_ptr<SubMenu> Options::Return()
{
    Event newEvent;
    //newEvent.type = Event::Type::settingsChanged;
    //Utility::events.push_back(newEvent);
    std::unique_ptr<SubMenu> newMenu = std::make_unique<Title>();
    return newMenu;
}