#pragma once
#include "Gamestate.h"

class State_Game : public Gamestate {
private:
	//# Data
	//Camera
	sf::View view_playField;
	sf::View view_gui;
	sf::Vector2f viewSize_playField = { 1280,720 };
	sf::Vector2f viewSize_gui = { 1280,720 };
	sf::FloatRect viewport_playField = { 0,0,1,1 };
	sf::FloatRect viewport_gui = { 0,0,1,1 };

	sf::FloatRect cameraBounds = { -100,-100,600,600 }; //How much can the camera offset
	float panSpeed = 300.f;
	float zoomSpeed = .03f;
	float currZoom = 1.f;
	sf::Vector2f zoomBounds = { 1.5f,.70f };

	float towerScale_palette = 1.5f;
	float towerScale_playfield = 1.25f;

	//Gui
	Gui* gui;
	std::vector<Tower> palette; //Tower Selection
	int paletteColumns = 2;
	int palettePadding = 20; //Px between palette entries
	sf::Vector2f palettePos;
	sf::Vector2f paletteSize = { 1,1 };
	sf::Vector2f paletteRatio = { .15f,1.f }; //What proportion of the screen palette takes up
	float paletteEntrySize = 50.f; //How big the palette options are
	bool isPalletePicked = false;
	Tower* palettePick = nullptr;

	//Gameplay
	TileMap* tileMap;
	std::vector<Hostile*> hostiles;
	std::vector<Tower*> towers;
	std::vector<Projectile*> projectiles;
	std::stack<Round*> rounds;
	Tower* ctrlTower = nullptr;

	float hp = 10.f;


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
	void paletteSelect();
	void paletteDeselect();

	void updateCollision();
	void updateTargeting();
	void updateCamera(float dt); //zoom is handled in polling function :(
	void update(float dt);

	void drawPallete(sf::RenderWindow& win);
	void draw(sf::RenderWindow& win);


};


