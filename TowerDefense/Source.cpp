#include "Engine.h"


/*
Project: Tower Defense, Driver code
Created: 26 MAY 2025
Updated: 26 MAY 2025

Description:
	This file contains the driver code for the engine.

*/

int main(int argc, char** argv) {
	Engine engine;


	while (engine.getRunning()) {
		engine.update();
		engine.draw();
	}

	return 0;
}