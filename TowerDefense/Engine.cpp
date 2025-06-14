#include "Engine.h"

void Engine::initWindow()
{
	win = new sf::RenderWindow;
	win->create(sf::VideoMode(500, 500), "Tower Defense");
}

void Engine::initTextures()
{
	textureHandler = new TextureHandler();

	textureHandler->addTexture("bg_test", "resource/tex/bg_0.png");
	textureHandler->addTexture("hostile_0", "resource/tex/hostile_0.png");
	textureHandler->addTexture("tower_0", "resource/tex/tower_0.png");
	textureHandler->addTexture("projectile_0", "resource/tex/projectile_0.png");
	textureHandler->addTexture("tileset_0", "resource/tex/tileset_0.png");
	textureHandler->addTexture("panel_0", "resource/tex/panel_0.png");
	textureHandler->addTexture("panel_1", "resource/tex/panel_1.png");
	textureHandler->addTexture("panel_brn", "resource/tex/panel_brn.png");
	textureHandler->addTexture("panel_stone", "resource/tex/panel_stone.png");
}

void Engine::initState()
{
	currState = new State_Game(textureHandler);
	//currState = new State_Menu(textureHandler);
}

Engine::Engine()
{
	this->initWindow();
	this->initTextures();
	this->initState();
}

Engine::~Engine()
{
	delete currState;
	delete textureHandler;
	delete win;
}

void Engine::update()
{
	//Update delta time
	deltaTime = mainClock.getElapsedTime().asSeconds();
	mainClock.restart();

	//Handle Polled Events
	while (win->pollEvent(event)) {
		//Window closure
		if (event.type == sf::Event::Closed) {
			win->close();
			isRunning = false;
		}

		if (event.type == sf::Event::MouseButtonReleased) {
			if (event.key.code == sf::Mouse::Right) {
				auto pos = sf::Mouse::getPosition(*win);

				std::cout << "(" << pos.x << ", " << pos.y << ")" << std::endl;
			}
		}

		currState->poll(*win, event);
	}

	//Handle per frame events
	currState->update(deltaTime);

}

void Engine::draw()
{
	if (!currState) {
		isRunning = false;
		return;
	}

	win->clear();

	currState->draw(*win);

	win->display();
}

bool Engine::getRunning()
{
	return isRunning;
}
