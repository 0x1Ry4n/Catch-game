#include "Headers/Game.h"
#include <iostream>

int main()
{
	// Init srand
	srand(static_cast<unsigned>(time(NULL)));
#if defined(_DEBUG)
	std::cout << "Debug console" << std::endl;
#endif
	Game game;

	while (game.running())
	{
		// Update
		game.update();

		// Render
		game.render();
	}

	return 0;
}
