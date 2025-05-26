#pragma once
#include <SFML/Graphics.hpp>
#include "Util.h"

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
	Hostile(sf::Texture* tex);
	~Hostile();

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow& win);
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
};