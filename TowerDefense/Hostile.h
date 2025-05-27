#pragma once
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <stack>
#include <SFML/Graphics.hpp>
#include "Util.h"

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
};