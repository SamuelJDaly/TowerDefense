#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "TextureHandler.h"
#include "Gamestate.h"


class Engine
{
private:
	//Data
	sf::RenderWindow* win;
	sf::Event event;
	sf::Clock mainClock;
	bool isRunning = true;
	TextureHandler* textureHandler;

	float deltaTime = 0;

	Gamestate* currState;

	//Util
	void initWindow();
	void initTextures();
	void initState();

public:
	//Constructor and Destructor
	Engine();
	~Engine();

	//Primary Functions
	void update();
	void draw();

	bool getRunning();
};

