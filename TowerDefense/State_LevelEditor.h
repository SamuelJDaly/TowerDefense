#pragma once
#include "Gamestate.h"

class State_LevelEditor : public Gamestate
{
private:
	//Data

	//Util

public:
	//Constructor and Destructor

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow &win);
};

