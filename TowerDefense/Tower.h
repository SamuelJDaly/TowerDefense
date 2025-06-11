#pragma once
#include <SFML/Graphics.hpp>
#include "Util.h"

/*
Project: Tower Defense, Tower System
Created: 26 MAY 2025
Updated: 26 MAY 2025

Description:
	This file contains the Tower System, which is one of the main ways the player interacts with the game. As of this version, towers
	fire projectiles as actual seperate objects with their own stats. The system will need to be expanded to accomodate more damage delivery
	methods than the projectile, such as aoe damage, beam damage, etc...

*/

//###########	PROJECTILE	#####################
class Projectile {
private:
	//Data
	sf::Sprite graph;

	bool isDead = false;

	sf::Vector2f velocity = { 0,0 }; //px / s

	DamageType dmgType = DamageType::NORMAL;
	float damage = 1;
	float speed = 100;

	sf::Vector2f spawn = { 0,0 };
	float range = 100; //in px

public:
	//Constructor and Destructor
	Projectile();
	~Projectile();

	//Primary functions
	void update(float dt);
	void draw(sf::RenderWindow &win);
	void setTexture(sf::Texture* texture);
	
	void die();
	void setSpawn(sf::Vector2f newSpawn);
	void setVelocity(sf::Vector2f newVelocity);
	void setTarget(sf::Vector2f target);
	void setRange(float newRange);
	void setPosition(sf::Vector2f newPos);
	void setDamage(float newDamage);
	void setSpeed(float newSpeed);
	
	float getSpeed();
	bool getDead();
	sf::FloatRect getBounds();
	float getDamage();
	DamageType getDamageType();
	
};


//###########	TOWER	#########################
class Tower
{
private:
	//Data
	sf::Sprite graph;

	float cooldownThreshold = 1; //in s
	float cooldownTimer = 0;
	bool canFire = true;

	Projectile projectileType;
	sf::Vector2f target = {0,0};

	float range = 100;
	bool doDrawRange = false;


public:
	//Constructor and Destructor
	Tower();
	~Tower();

	//Primary functions
	void update(float dt);
	void draw(sf::RenderWindow& win);

	void setTexture(sf::Texture* texture);
	void setProjectile(Projectile archetype);

	void setPosition(sf::Vector2f newPosition);

	void setTarget(sf::Vector2f newTarget);
	void fire(std::vector<Projectile*> &projectileList);
	void setOverlayColor(sf::Color col);
	void setCooldown(float newCooldown);

	void setDrawRange(bool state);

	bool contains(sf::Vector2f pos);
	bool inRange(sf::Vector2f pos);

};

