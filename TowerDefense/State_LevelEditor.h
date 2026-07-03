#pragma once
#include "Gamestate.h"


/*
//Common Data
	TextureHandler* textureHandler = nullptr;
	sf::RenderWindow* window;
	bool isFinished = false;
	En_Gamestate nextState = En_Gamestate::END;

public:
	//Common Functions
	bool getFinished();
	En_Gamestate getNextState();

	//Virtual Functions
	virtual void update(float dt) = 0;
	virtual void poll(sf::RenderWindow& win, sf::Event& event) = 0;
	virtual void draw(sf::RenderWindow& win) = 0;
*/

class State_LevelEditor : public Gamestate
{
private:
	//Data

	//Util

public:
	//Constructor and Destructor

	//Primary Functions
	void update(float dt);
	void poll(sf::RenderWindow& win, std::optional<sf::Event> event);
	void draw(sf::RenderWindow &win);
};

