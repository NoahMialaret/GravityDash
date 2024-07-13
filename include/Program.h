#ifndef PROGRAM_H
#define PROGRAM_H

#include <SFML/Graphics.hpp>

#include "Event.h"
#include "Game.h"
#include "Particle.h"
#include "MainMenu.h"
#include "Utility.h"

#include "TitleSequence.h"

#include <algorithm>
#include <iostream>

// Program object handles most of the programs's main proccesses and objects,
// and is responsible for checking events, updating, and rendering
class Program 
{
public:
	// Represents the current state of the program
	enum class State 
	{
		notRunning,
    titleSequence,
		startMenu,
		gameplay,
		paused
	};	

public:
	Program() = delete;
	// Program constructor that takes the title to be displayed on the window
	Program(const char* name);
	// Destructor closes the window and frees up dynamic memory created from the Particle class
  ~Program();

	// Handles SFML events
	void HandleEvents();
	// Handles program logic
	void Update();
	// Handles graphics rendering
	void Render();

	// Returns the current game state
	Program::State GetCurState() const;

private:
	// Sets the game state to "notRunning"
	void ProgramExit();

private:
	//SFML Objects --------------------------------------------------------
  sf::RenderWindow window;	// The window of the program 
	sf::View mainView; 			// Represents the region of the gameworld that should be displayed onto the window

	//Program Variables ---------------------------------------------------
	State curState = State::notRunning; // The current state of the game

	//Program Objects ----------------------------------------------------- 
	std::unique_ptr<Game> game = nullptr;
	std::unique_ptr<MainMenu> mainMenu = nullptr;
  std::unique_ptr<TitleSequence> title = nullptr;
};

#endif