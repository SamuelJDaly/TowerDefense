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
Updated: 19 JUN 2025

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
			The menu state is not yet implemented. It will contain a gui based system to allow for starting the game, and changing game options.
		Game:
			The Game state is where the actual play happens. It contains the map, the towers, the hostiles, and all the other gameplay related components.
		Editor:
			The Editor state allows for the creation of levels without hard coding or manually editing a text file. This includes setting textures for the level,
			creating paths for the enemies, and setting up the spawn schedule for the rounds.
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
	//# Data
	//Camera
	sf::RenderWindow* window;
	sf::View view_playField;
	sf::View view_gui;
	sf::Vector2f viewSize_playField = { 1280,720 };
	sf::Vector2f viewSize_gui = { 1280,720 };
	sf::FloatRect viewport_playField = {0,0,1,1};
	sf::FloatRect viewport_gui = {0,0,1,1};

	sf::FloatRect cameraBounds = {-100,-100,600,600}; //How much can the camera offset
	float panSpeed = 300;
	float zoomSpeed = .03;
	float currZoom = 1;
	sf::Vector2f zoomBounds = {1.5,.70};

	float towerScale_pallete = 1.5;
	float towerScale_playfield = 1.25;

	//Gui
	Gui* gui;
	std::vector<Tower> pallete; //Tower Selection
	int palleteColumns = 2;
	int palletePadding = 20; //Px between pallete entries
	sf::Vector2f palletePos;
	sf::Vector2f palleteSize = {1,1};
	sf::Vector2f palleteRatio = {.15,1}; //What proportion of the screen pallete takes up
	float palleteEntrySize = 50; //How big the pallete options are
	bool isPalletePicked = false;
	Tower* palletePick = nullptr;

	//Gameplay
	Map* map;
	TileMap* tileMap;
	std::vector<Hostile*> hostiles;
	std::vector<Tower*> towers;
	std::vector<Projectile*> projectiles;
	std::stack<Round*> rounds;
	Tower* ctrlTower = nullptr;

	int hp = 10;
	

	//textureHandler (base class)

	
	

	//Util
	void initGui();
	void initView();
	void initMap();
	void initHostiles();
	void initTest();

public:
	//Constructor and Destructor
	State_Game(TextureHandler* textureHandler, sf::RenderWindow* window);
	~State_Game();

	//Primary Functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void palleteSelect();
	void palleteDeselect();
	
	void updateCollision();
	void updateTargeting();
	void updateCamera(float dt); //zoom is handled in polling function :(
	void update(float dt);

	void drawPallete(sf::RenderWindow &win);
	void draw(sf::RenderWindow &win);

	
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



//##########################	MAP EDITOR 	#############################################
class State_Editor : public Gamestate {
private:
	//## Data
	sf::RenderWindow* window;
	Gui* gui;
	sf::Texture* activeTileset = nullptr;
	sf::Font* font;

	//Camera
	sf::View view_map;
	sf::View view_gui;
	sf::Vector2f viewSize_map = { 1280,720 };
	sf::Vector2f viewSize_gui = { 1280,720 };
	sf::FloatRect viewport_map = { .25,0,1,.2};
	sf::FloatRect viewport_gui = { 0,0,1,1 };

	sf::FloatRect cameraBounds = { -100,-100,600,600 }; //How much can the camera offset
	float panSpeed = 300;
	float zoomSpeed = .03;
	float currZoom = 1;
	sf::Vector2f zoomBounds = { 1.5,.70 };

	//Gui
	float leftPanelRatio = .25; //What fraction of view does left panel cover (x axis)
	float bottomPanelRatio = .2; //(y axis)

	//Texture Select
	Spritesheet spritesheet;
	std::vector<sf::Sprite> pallete;
	int palleteColumns = 3;
	int numTextures = 0;
	sf::Vector2f palletePos = {0,0};
	sf::Vector2f palleteSize = { 1,1 };
	sf::Vector2f palleteRatio = {.75,.75};
	sf::RectangleShape palleteBorder;
	int palleteSelect = -1;
	sf::RectangleShape selectBorder;
	bool isPainting = false;

	//Shortcuts
	bool isPress_ctrl = false;
	bool isPress_z = false;

	//Map
	sf::FloatRect mapBoundry;
	sf::Vector2i mapSize = { 10,10 };
	float tileSize = 50;
	sf::Image blankImage;
	sf::Texture* blankTexture;
	std::vector<std::vector<int>> map;
	std::vector<std::vector<int>> prevMap;
	std::vector<std::vector<sf::Sprite>> mapDisplay;
	std::vector<std::vector<sf::Sprite>> prevMapDisplay;
	sf::Color gridColor = sf::Color::Black;
	std::vector<sf::Vertex> grid_horizontal;
	std::vector<sf::Vertex> grid_vertical;
	bool isChanged = false;

	

	//## Util
	void initGui();
	void initCamera();
	void initTest();
	void initMap();

public:
	//Constructor and Destructor
	State_Editor(TextureHandler* textureHandler, sf::RenderWindow* window);
	~State_Editor();

	//Primary Functions
	void loadPallete(int txSize, std::string filepath);

	void saveMap(std::string filepath);

	void poll(sf::RenderWindow& win, sf::Event& event);

	void updateCamera(float dt);
	void updatePainting();
	void update(float dt);

	void drawMap(sf::RenderWindow& win);
	void drawPallete(sf::RenderWindow &win);
	void draw(sf::RenderWindow& win);
};