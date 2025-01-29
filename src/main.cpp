#include "Program.h"

int main()
{
	Program program("Gravity Dash");

	//Main program loop
	while (program.GetCurState() != Program::State::notRunning)
	{
		program.ProcessEvents();
		program.Update();
		program.Render();
	}

	return 0;
}