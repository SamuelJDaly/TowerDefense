#include "Util.h"


float dist(sf::Vector2f posA, sf::Vector2f posB)
{
	//sqrt(~x^2 + ~y^2)
	return std::sqrtf(powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2));
}

sf::Vector2f diff(sf::Vector2f A, sf::Vector2f B)
{
	return {B.x - A.x, B.y - A.y};
}

float radToDeg(float radians)
{
	
	return (radians * 180 / PI);
}

float degToRad(float degrees)
{
	return (degrees * PI) / 180;
}

std::istream& operator>>(std::istream& is, Node& obj)
{
	//Get pos
	is >> obj.pos.x >> obj.pos.y;

	//Get type
	int rawType;
	is >> rawType;
	auto type = static_cast<NodeType>(rawType);

	obj.type = type;
	return is;
}
