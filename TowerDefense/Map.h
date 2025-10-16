#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "TextureHandler.h"
#include "Util.h"

/*
Project: Tower Defense, Map System
Created: 26 MAY 2025
Updated: 23 JUN 2025

Description:
	This file contains the map system. The original idea was to use a simple map with a single background image, which was called the "Map".
	In order to facilitate other systems this has been replaced with a tilemap system. The "Tilemap" is a 2d array of tiles
	(which are basically sprites and a few stats). It also contains a path, which is mostly just a linked list of coordinates. This path
	is used by the hostile system to tell enemies where to move. Right now the system only supports singly linked paths, ie no branching.
	It might be worth implementing branching, and multiple paths.
*/

//###################	TILE		#######################
class Tile {
private:
	//Data
	sf::Sprite sprite;
	bool solid = false;
	int type = -1;

public:
	//Constructor and Destructor
	Tile();
	~Tile();


	//Primary Functions
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	void setTexture(sf::Texture* texture);
	void setTextureRect(sf::IntRect rect);
	void setType(int newType);
	void setSize(float size);
	void setPosition(sf::Vector2f pos);
	int getType();
};


//##################	MAP		#######################
class TileMap
{
private:
	//Data
	std::vector<std::vector<Tile*>*> tilemap;
	Spritesheet tileset;
	int width = 0;
	int height = 0;
	int tileSize = 16;
	int textureSize = 1;

	bool hasBackground = false;
	sf::Sprite background;

	Node* pathHead = nullptr;
	bool doDrawNodes = true;

	std::string tileSetPath = "";
	std::string tileSetName = "";

	//Util


public:
	//Constructor and Destructor
	TileMap();
	~TileMap();

	//Primary Functions
	void update(float deltaTime);
	void draw(sf::RenderWindow& window);

	void loadFromFile(std::string filepath, TextureHandler &textureHandler);
	void writeToFile(std::string filepath);
	void setTileset(Spritesheet newSet);
	void refreshTilemap();
	void modTile(int x, int y, int type);
	void resize(int newWidth, int newHeight);
	int getType(int x, int y);

	void setTilesize(float newSize);

	void setBackground(sf::Texture* texture, sf::Vector2f size);

	void loadPath(std::string filepath);
	Node* getPath();
};

