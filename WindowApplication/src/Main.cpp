#include <iostream>
#include <SFML/Graphics.hpp>
#include "App.h"

int main() {

	//INIT GAME ENGINE

	srand(unsigned(time(NULL)));

	App app;

	// GAME LOOP
	while (app.isRunning())
	{
		// UPDATE
		app.update();
		app.update();
		app.update();

		// RENDER
		app.render();
	}

	// END
	return 0;

	system("PAUSE");
	return 0;
}