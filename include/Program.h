#ifndef PROGRAM_H
#define PROGRAM_H

#include <SFML/Graphics.hpp>

#include "Clock.h"
#include "Event.h"
#include "GameManager.h"
#include "Keyboard.h"
#include "Menu.h"
#include "ParticleManager.h"
#include "AssetManager.h"
#include "Utility.h"
#include "ProgramSettings.h"

#include <algorithm>
#include <iostream>

// `Program` handles the programs's main window, proccesses, and objects,
//   and is responsible for checking events, updating, and rendering objects
class Program 
{
public:
	// Represents the current state of the program
	enum class State 
	{
		notRunning,
    mainMenu,
		gameplay,
		paused
	};	

public:
	Program() = delete;
	// Constructs the Program's window and set's its title to `name`, 
  //   as well as loading assets and program critical objects
	Program(const char* name);
	// Destructor closes the window and frees up dynamic memory, including those made by singletons
  ~Program();

	// Handles Program events
	void ProcessEvents();
	// Handles Program logic
	void Update();
	// Handles graphics rendering
	void Render();

	// Returns the current program state
	Program::State GetCurState() const;

private:
	//SFML Objects --------------------------------------------------------
  sf::RenderWindow window;	// The window of the program 
	sf::View mainView; 			  // Represents the region of the gameworld that should be displayed

	//Program Variables ---------------------------------------------------
	State curState = State::notRunning; // The current state of the game

	//Program Objects ----------------------------------------------------- 
	std::unique_ptr<GameManager> gameManager = nullptr; // The object which handles game logic
	std::unique_ptr<Menu> menu = nullptr;               // The object which handles menu logic
};

#endif