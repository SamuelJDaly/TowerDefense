#pragma once
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include "Tower.h"
#include "Hostile.h"
#include "Map.h"

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

class Gamestate_Menu : public Gamestate {
private:
	//Data


	//Util

public:
	//Constructor and Destructor
	Gamestate_Menu();
	~Gamestate_Menu();

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow &win);

};