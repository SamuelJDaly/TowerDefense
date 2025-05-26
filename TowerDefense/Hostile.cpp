#include "Hostile.h"

Hostile::Hostile(sf::Texture* tex)
{
	texture = tex;
	graph.setTexture(*texture);
	graph.setOrigin({(float)(tex->getSize().x / 2.0), (float)(tex->getSize().y / 2.0) });
	size = (sf::Vector2f)tex->getSize();
}

Hostile::~Hostile()
{
}

void Hostile::update(float dt)
{
	//Death
	if (hp <= 0) {
		this->Die();
	}


	//## 1. Path determination (get target)
	//Calculate offset from target
	float xDiff = target.x - graph.getPosition().x;
	float yDiff = target.y - graph.getPosition().y;


	if (fabs(xDiff) <= targetThreshold && fabs(yDiff) <= targetThreshold && currNode) {
		//Then goal reached move to next node in path
		currNode = currNode->next;
		if (currNode) {
			target = currNode->pos;
		}
	}

	//## 2. Path Resolution (move towards target)

	//Calculate angle between current pos and target pos
	float theta = std::atan2f(yDiff,xDiff);
	
	//Move by speed * cos theta * dt, speed * sin theta * dt
	graph.move(speed * std::cosf(theta) * dt, speed * std::sinf(theta) * dt);
}

void Hostile::draw(sf::RenderWindow& win)
{
	win.draw(graph);
}

void Hostile::setTarget(sf::Vector2f newTarget)
{
	target = newTarget;
}

void Hostile::setScale(sf::Vector2f newScale)
{
	scale = newScale;
	size.x = scale.x * texture->getSize().x;
	size.y = scale.y * texture->getSize().y;
	graph.setScale(scale);
}

void Hostile::setSize(sf::Vector2f newSize)
{
	size = newSize;
	scale.x = size.x / texture->getSize().x;
	scale.y = size.y / texture->getSize().y;
	graph.setScale(scale);
}

void Hostile::setPath(Node* path)
{
	pathHead = path;
	currNode = path;
}

void Hostile::setPos(sf::Vector2f newPos)
{
	graph.setPosition(newPos);
}

void Hostile::resetNode()
{
	currNode = pathHead;
	graph.setPosition(currNode->pos);
	target = currNode->pos;
}

void Hostile::takeDamage(float amount, DamageType type)
{
	float base = amount;

	//Check for resistance
	for (auto i : resistance) {
		if (i == type) {
			amount -= base * MODIFIER_RESISTANCE;
			break;
		}
	}

	//Check for weakness
	for (auto i : weakness) {
		if (i == type) {
			amount += base * MODIFIER_WEAKNESS;
			break;
		}
	}

	hp -= amount;
}

void Hostile::Die()
{
	isDead = true;
}

bool Hostile::getDead()
{
	return isDead;
}

sf::FloatRect Hostile::getBounds()
{
	return graph.getGlobalBounds();
}
