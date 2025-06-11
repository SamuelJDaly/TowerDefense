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
Updated: 11 JUN 2025

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


	Currently there are two states:
		Menu:
			The menu state is not yet implemented. It will contain a gui based system to allow for starting the game, and changing game options.
		Game:
			The Game state is where the actual play happens. It contains the map, the towers, the hostiles, and all the other gameplay related components.
*/


//##########################	BASE CLASS	#########################################
class Gamestate
{
protected:
	//Common Data
	TextureHandler* textureHandler =  nullptr;

public:
	//Common Functions

	//Virtual Functions
	virtual void update(float dt) = 0;
	virtual void poll(sf::RenderWindow& win, sf::Event& event) = 0;
	virtual void draw(sf::RenderWindow &win) = 0;
};


//##########################	GAME	#############################################
class State_Game : public Gamestate {
private:
	//Data
	Map* map;
	TileMap* tileMap;
	std::vector<Hostile*> hostiles;
	std::vector<Tower*> towers;
	std::vector<Projectile*> projectiles;
	std::stack<Round*> rounds;

	//textureHandler (base class)

	Tower* ctrlTower = nullptr;

	int hp = 10;
	

	//Util
	void initMap();
	void initHostiles();
	void initTest();

public:
	//Constructor and Destructor
	State_Game(TextureHandler* textureHandler);
	~State_Game();

	//Primary Functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(float dt);
	void draw(sf::RenderWindow &win);

	void updateCollision();
	void updateTargeting();
};


//##########################	MENU 	#############################################

class State_Menu : public Gamestate {
private:
	//Data
	Gui* gui;
	sf::Font font;

	//Util
	void initGui();

public:
	//Constructor and Destructor
	State_Menu(TextureHandler* textureHandler);
	~State_Menu();

	//Primary Functions
	void update(float dt);
	void poll(sf::RenderWindow& win, sf::Event& event);
	void draw(sf::RenderWindow &win);

};