#pragma once
#pragma once
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Hostile.h"
#include "Map.h"
#include "Gui.h"

/*
Project: Tower Defense, Gamestate System
Created: 26 MAY 2025
Updated: 3 JUL 2026

Description:
	This file contains the Gamestate System. This is what actually ties the various systems (tower, hostile, map, etc...) together and manages their interactions.
	These states have some common program loop functions that are called by the engine class once per frame:
		Poll:
			This is where less time dependant event logic happens, like window closures, button clicks, etc...

		Update:
			This is where per-frame logic happens. Events that need to happen once per frame for smooth movement, etc are called here. This includes things
			like collision, movement, etc...

		Draw:
			This is where any draw calls happen.


	Currently there are three states:
		Menu:
			The menu state is not yet implemented. It will contain the main menu from which users can navigate to certain other states (Game, Editor, Options...)
		Game:
			The Game state is where the actual play happens. It contains the map, the towers, the hostiles, and all the other gameplay related components.
		Editor:
			The Editor state allows for the creation of levels without hard coding or manually editing a text file. This includes setting textures for the level,
			creating paths for the enemies, and setting up the spawn schedule for the rounds.
*/

//##########################	STATE ENUMS	#########################################
enum class En_Gamestate {
	MENU, GAME, EDITOR, TEST, END
};

//##########################	BASE CLASS	#########################################
class Gamestate
{
protected:
	//Common Data
	TextureHandler* textureHandler = nullptr;
	sf::RenderWindow* window;
	bool isFinished = false;
	En_Gamestate nextState = En_Gamestate::END;
	sf::Texture defaultTexture;
	sf::Font defaultFont;

public:
	//Common Functions
	bool getFinished();
	En_Gamestate getNextState();

	//Virtual Functions
	virtual void update(float dt) = 0;
	virtual void poll(sf::RenderWindow& win, std::optional<sf::Event> event) = 0;
	virtual void draw(sf::RenderWindow& win) = 0;
};