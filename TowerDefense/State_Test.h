#pragma once
#include "Gamestate.h"

class State_Test : public Gamestate
{
private:
	//## Data

	//## Util


public:
	//## Constructor and Destructor

	//## Primary Functions
	void Poll(sf::RenderWindow& win, std::optional<sf::Event> event);
	void Update(float dt);
	void Draw(sf::RenderWindow& win);

};

