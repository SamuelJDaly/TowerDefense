#pragma once
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>

#define PI 3.14159

#define MODIFIER_WEAKNESS 0.25;
#define MODIFIER_RESISTANCE 0.25;


/*
Project: Tower Defense, Utility Functions
Created: 26 MAY 2025
Updated: 19 JUN 2025

Description:
	This file contains various utility functions that are common to multiple systems.
*/


//#################		UTIL	###################
enum class NodeType {
	BEGIN, PATH, END
};

enum class DamageType {
	NORMAL, FIRE, ICE
};

float dist(sf::Vector2f posA, sf::Vector2f posB);

sf::Vector2f diff(sf::Vector2f A, sf::Vector2f B);

float radToDeg(float radians);
float degToRad(float degrees);

void printRectI(sf::IntRect targ);
void printRectF(sf::FloatRect targ);

//##################	NODE	###################
struct Node {
	sf::Vector2f pos;
	sf::Vector2f offSet = { 0,0 };
	NodeType type = NodeType::PATH;
	Node* next = nullptr;
	Node* last = nullptr;

};

std::istream& operator>>(std::istream& is, Node& obj);

Node* fLoadPath(std::string filepath);
void pathOffset(Node* head, sf::Vector2f offSet);


//##################	SPRITESHEET		###################
class Spritesheet {
private:
	//Data
	sf::Texture* sheet;
	bool isLocalTexture = 0; //Is the texture locally managed
	int numTextures = 0;
	sf::Vector2i textureSize = {1,1};
	std::vector<sf::IntRect> rects;

	//Util
	void slice();

public:
	//Constructor and Destructor
	Spritesheet();
	~Spritesheet();

	//Primary Functions
	int fload(std::string filepath); //Load from file
	void setTextureSize(sf::Vector2i newSize);
	void setTextureSize(int width, int height);
	void setTexture(sf::Texture* newTexture);
	void setTexture(sf::Texture* newTexture, sf::Vector2i newSize);

	sf::IntRect getRect(int idx);
	sf::IntRect getRect(int x, int y);
	std::vector<sf::IntRect> getRects();
	sf::Texture* getTexture();
	int getNumTextures();
};