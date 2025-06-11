#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include "Util.h"

/*
Project: Tower Defense, Hostile System
Created: 26 MAY 2025
Updated: 26 MAY 2025

Description:
	This file contains the Hostile system. This system implements the enemies that the player must defeat in order to stay alive.
	The main parts of this sytem are the "Hostile" and the "Round". The Hostile is exactly what it sounds like, a sprite and a
	group of stats. The round is a list of times and a list of hostiles. When the current time value passes the current hostile
	is spawned, and the list is advanced.
*/

//######################	HOSTILE		############################

class Hostile
{
private:
	//Data
	sf::Texture* texture;
	sf::Sprite graph;
	sf::Vector2f scale = {1,1};
	sf::Vector2f size = { 1,1 };

	float speed = 30;
	float hp = 10;
	bool isDead = false;
	bool isPathDone = false;
	std::vector<DamageType> resistance;
	std::vector<DamageType>weakness;

	sf::Vector2f target;
	float targetThreshold = .1; //How close graph has to be to reach target

	Node* pathHead = nullptr;
	Node* currNode = nullptr;

public:
	//Constructor and Destructor
	Hostile();
	~Hostile();

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow& win);

	void setTexture(sf::Texture* texture);
	void setTarget(sf::Vector2f newTarget);
	void setPath(Node* path);
	void setPos(sf::Vector2f newPos);
	void resetNode();
	void takeDamage(float amount, DamageType type);
	void Die();
	void setSize(sf::Vector2f newSize);
	void setScale(sf::Vector2f newScale);

	bool getDead();
	sf::FloatRect getBounds();
	sf::Vector2f getPos();
	bool getPathDone();
	int getHp();
};

//######################	ROUND		############################
/*
The round is basically a scheduling system for spawning hostiles at given times.
This is accomplished by keeping track of two lists:
	1. A list of hostiles to spawn
	2. The timing between each spawn
*/

class Round {
private:
	//Data
	std::stack<float> timing;
	std::stack<Hostile*> hostiles;
	std::unordered_map<std::string, Hostile> atlas;
	float timer = 0;
	bool isRunning = false;
	bool isDone = false;
	bool doSpawn = false;



public:
	//Constructor and Destructor
	Round();
	~Round();

	//Primary functions
	void start();
	void update(float dt);
	void spawn(std::vector<Hostile*> &list);
	//void fLoadAtlas(std::string filepath); //NOT IMPLEMENTED
	void addAtlasEntry(std::string name, Hostile type);
	void loadFromFile(std::string filepath);

	bool getSpawnState();
	bool getDone();
};