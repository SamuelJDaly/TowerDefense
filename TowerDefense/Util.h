#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>

#define PI 3.14159

#define MODIFIER_WEAKNESS 0.25;
#define MODIFIER_RESISTANCE 0.25;

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

//##################	NODE	###################
struct Node {
	sf::Vector2f pos;
	sf::Vector2f offSet = { 0,0 };
	NodeType type = NodeType::PATH;
	Node* next = nullptr;
	Node* last = nullptr;

};

std::istream& operator>>(std::istream& is, Node& obj);

