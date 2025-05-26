#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureHandler.h"
#include "Util.h"


//###################	TILE		#######################
class Tile {
private:
	//Data
	sf::Sprite sprite;
	bool solid = false;
	sf::Vector2i type = { -1,-1 };

public:
	//Constructor and Destructor
	Tile();
	~Tile();


	//Primary Functions
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	void setTexture(sf::Texture* texture);
	void setTextureRect(sf::IntRect rect);
	void setType(sf::Vector2i newType);
	void setSize(float size);
	void setPosition(sf::Vector2f pos);
	sf::Vector2i getType();
};

//###################	MAP		#######################
class Map
{
private:
	//Data
	TextureHandler* textureHandler = nullptr;
	sf::Sprite background;

	Node* pathHead = nullptr;

	bool doDrawNodes = true;

	//Util
	void drawNodes(sf::RenderWindow &win);

public:
	//Constructors and Destructors
	Map(TextureHandler* textureHandler);
	~Map();

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow &win);
	void loadPath(std::string filepath);
	Node* getPath();
};


//##################	TILEMAP	#######################
class TileMap
{
private:
	//Data
	std::vector<std::vector<Tile*>*> tilemap;
	sf::Texture* tileset;
	unsigned int width = 0;
	unsigned int height = 0;
	int tileSize = 16;

	bool hasBackground = false;
	sf::Sprite background;

	Node* pathHead = nullptr;
	bool doDrawNodes = true;

	//Util
	void DrawNodes(sf::RenderWindow &win);

public:
	//Constructor and Destructor
	TileMap(sf::Texture* initTileset);
	~TileMap();

	//Primary Functions
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	void loadFromFile(std::string filepath);
	void setTileset(sf::Texture* newTileset);
	void refreshTilemap();

	void setBackground(sf::Texture* texture, sf::Vector2f size);

	void loadPath(std::string filepath);
	Node* getPath();
};

