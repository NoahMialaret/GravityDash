#define STB_IMAGE_IMPLEMENTATION

#include "Texture.hpp"

#include "Program.h"
#include "Utility.h"

int main()
{
	Program program("GravDash");

	//Main program loop
	while (program.GetCurState() != Program::State::notRunning)
	{
		//float nextFrameStart = (float)CUR_TIME + (1000.0f / Utility::targetFrameRate);

		program.HandleEvents();
		program.Update();
		program.Render();

		//while (nextFrameStart > (float)CUR_TIME);
	}

	return 0;
}

/* To do:
	PRIORITY:
    Redo how world is rendered to players and entites are drawn under the world borders,
      and world can have some decoratvie sprites outside the border
    Change player so it calculates points instead of entites
    player should be stunned and knocked back when hit, and flash while invinciblew
    Dedicated timer graphic (likely class as well)

	Code
		Fully comment implemented class headers
		Subdivide "Utility" class into seperate, more specific, classes
			Utility
			Event
			Keyboard (maybe)
			Debug
			ProgramSettings
		Camera class
		GridMenu should not do out of bounds selector tests if the selector never left the origina button (e.g. pressing up on play button)
		Support more than 4 tabs in configMenuType
		Proper framerate independant updating and timings
		Player should hold the combo score and points linked list
		Player should be a polymorphic class for cpu and human
		functionality of some classes should be split following SOLID
			Player should instead be a class representing the player, with the controllable aspects
			... seperated into a seperate "controllable" class
		Seperate player controls into seperate class to allow easier interfacing for keyboard/controller

	Finish main menu
		List menu with options (either bool or a range)
		Scores menu
			Leaderboard menu and IO (from json) for the various modes
			Historty of achievements/medals, tracked during gameplay
			Game stats, such as distance run, deaths, jumps, num of "super scores (10+ in a row)"
		Custom game config, users can choose things like spawn rates, transitions, and game speed
		Ability to hold keys to quickly go through element list
    Player select menu

	Gameplay
		Tutorial segment on first start, and option to enter tutorial from the menu
		Various world transitions
		Ability for users to create custom transitions
		Ability for users to save custom rulesets
		Other world elements such as platforms
		More entities, such as jump blockers
		Player "super move"
		Proper ending screen with name entry
		Time attack or rush mode
		Proper cpu behaviour
		Multiplayer support (2 - keyboard, 4 - controller)
			Players will be assigned different colours
			Versus mode
				Score based (w/ timer)
				Elimination mode (score allows for different items) 
			Co-op mode

	World Transitions
		Short and wide spike dodging

 	Visuals / Aestetics
 		Intro/title sequence
		Shaders for different colour schemes, button shadows, and different play characters
		More player animations and effects
		Proper sprites for game entities and the surrounding world
		Menu transitions
		Full screen support
		The coloured strips shown during multiplayer will change size depending on who has the point lead

*/	