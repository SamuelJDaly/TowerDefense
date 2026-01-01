#include "Engine.h"

void Engine::initWindow()
{
	win = new sf::RenderWindow;
	win->create(sf::VideoMode(1280, 720), "Tower Defense");
}

void Engine::initTextures()
{
	textureHandler = new TextureHandler();

	textureHandler->addTexture("transparent", "resource/tex/transparent.png");

	textureHandler->addTexture("panel_test", "resource/tex/panel_test.png");
	textureHandler->addTexture("bg_test", "resource/tex/bg_0.png");
	textureHandler->addTexture("hostile_0", "resource/tex/hostile_0.png");
	textureHandler->addTexture("tower_0", "resource/tex/tower_0.png");
	textureHandler->addTexture("projectile_0", "resource/tex/projectile_0.png");
	textureHandler->addTexture("tileset_0", "resource/tex/tileset_0.png");
	textureHandler->addTexture("panel_0", "resource/tex/panel_0.png");
	textureHandler->addTexture("panel_1", "resource/tex/panel_1.png");
	textureHandler->addTexture("panel_brn", "resource/tex/panel_brn.png");
	textureHandler->addTexture("panel_stone", "resource/tex/panel_stone.png");
	textureHandler->addTexture("panel_girder", "resource/tex/panel_girder.png");
	textureHandler->addTexture("panel_bevel", "resource/tex/panel_bevelGrey.png");
	textureHandler->addTexture("panel_simpleWhite", "resource/tex/panel_simpleWhite.png");
	textureHandler->addTexture("tileset_dither", "resource/tex/tileset_dither.png");
	textureHandler->addTexture("tileset_1bit", "resource/tex/tileset_1bit.png");
	textureHandler->addTexture("tab_simple", "resource/tex/tab_simple.png");

	textureHandler->addTexture("tower_1", "resource/tex/tower_1.png");
	textureHandler->addTexture("tower_2", "resource/tex/tower_2.png");
	textureHandler->addTexture("tower_cannon", "resource/tex/tower_cannon.png");
	textureHandler->addTexture("projectile_1", "resource/tex/projectile_1.png");
	textureHandler->addTexture("projectile_2", "resource/tex/projectile_2.png");

	textureHandler->addTexture("btn_save", "resource/tex/btn_save.png");
	textureHandler->addTexture("btn_plus", "resource/tex/btn_plus.png");
	textureHandler->addTexture("btn_minus", "resource/tex/btn_minus.png");
	textureHandler->addTexture("btn_palette", "resource/tex/btn_palette.png");
	textureHandler->addTexture("btn_blank", "resource/tex/btn_blank.png");
	textureHandler->addTexture("btn_blank_sq", "resource/tex/btn_blank_sq.png");
	textureHandler->addTexture("btn_blank_sq_ol", "resource/tex/btn_blank_sq_ol.png");
	textureHandler->addTexture("btn_blank_sq_tst", "resource/tex/btn_blank_sq_tst.png");
	textureHandler->addTexture("btn_tst_odd", "resource/tex/btn_test_odd.png");

	textureHandler->addTexture("bg_menu", "resource/tex/bg_menu.png");
}

void Engine::initState()
{
	//currState = new State_Game(textureHandler, this->win);
	//currState = new State_Editor(textureHandler, this->win);
	currState = new State_Menu(textureHandler, this->win);
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
	//## Update delta time
	deltaTime = mainClock.getElapsedTime().asSeconds();
	mainClock.restart();

	//# Handle Polled Events
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

	//## Handle per frame events

	//Update Current State
	currState->update(deltaTime);

	//State Transition Logic
	if (currState->getFinished()) {
		//Transition to next state
		switch (currState->getNextState()) {
		case En_Gamestate::MENU:
			//De allocate last state
			delete currState;
			//Allocate new state
			currState = new State_Menu(textureHandler, this->win);
			
			break;
		case En_Gamestate::GAME:
			//De allocate last state
			delete currState;
			//Allocate new state
			currState = new State_Game(textureHandler, this->win);

			break;
		case En_Gamestate::EDITOR:
			//De allocate last state
			delete currState;
			//Allocate new state
			currState = new State_Editor(textureHandler, this->win);

			break;
		case En_Gamestate::END:
			win->close();
			isRunning = false;
			break;
		default:
			std::cerr << "Invalid State Transition" << std::endl;
			win->close();
			isRunning = false;
			break;
		}
	}
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
