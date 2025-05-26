#include "Engine.h"

int main(int argc, char** argv) {
	Engine engine;


	while (engine.getRunning()) {
		engine.update();
		engine.draw();
	}

	return 0;
}