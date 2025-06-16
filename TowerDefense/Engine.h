#pragma once
#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>

#include "TextureHandler.h"
#include "Gamestate.h"

/*
Project: Tower Defense, Engine
Created: 26 MAY 2025
Updated: 16 JUN 2025

Description:
	This file contains the game "Engine" system. It functions as a state manager and a resource manager (textures, audio, etc...).
	
*/


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

