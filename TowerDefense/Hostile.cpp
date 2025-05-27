#include "Hostile.h"

Hostile::Hostile()
{
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

	//Check if path is done
	if (!currNode) {
		isPathDone = true;
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

void Hostile::setTexture(sf::Texture* texture)
{
	this->texture = texture;
	graph.setTexture(*texture);
	graph.setOrigin({ (float)(texture->getSize().x / 2.0), (float)(texture->getSize().y / 2.0) });
	size = (sf::Vector2f)texture->getSize();
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

bool Hostile::getPathDone()
{
	return isPathDone;
}

int Hostile::getHp()
{
	return hp;
}


sf::FloatRect Hostile::getBounds()
{
	return graph.getGlobalBounds();
}

sf::Vector2f Hostile::getPos()
{
	return graph.getPosition();
}

Round::Round()
{
}

Round::~Round()
{
	//Free any remaining memory in hostiles stack.
	while (!hostiles.empty()) {
		delete hostiles.top();
		hostiles.pop();
	}
}

void Round::start()
{
	isRunning = true;
}

void Round::update(float dt)
{
	if (!isRunning || timing.empty()) {
		return;
	}

	timer += dt;

	if (timer >= timing.top()) {
		timer = 0;
		doSpawn = true;
		timing.pop();
	}

}

void Round::spawn(std::vector<Hostile*> &list)
{
	//NOTE:
	// Any hostile exported to the given list WILL NOT BE FREED by the destructor of this class.
	// It is the responsibility of the owner of that list to free said memory.
	// (Any hostiles remaining in the hostiles stack at the time of the destructor call will be freed).

	if (hostiles.empty()) {
		return;
	}

	list.push_back(hostiles.top());
	hostiles.pop();
	doSpawn = false;
}

void Round::addAtlasEntry(std::string name, Hostile type)
{
	atlas.insert({ name, type });
}

void Round::loadFromFile(std::string filepath)
{
	//## Vars
	std::ifstream inFile;
	float time = 0;
	std::string type = "";



	//## Open file
	inFile.open(filepath);

	if (!inFile.is_open()) {
		std::cout << "Could not open file: " << filepath << std::endl;
		return;
	}

	//## Read File
	while (inFile >> time >> type) {
		auto it = atlas.find(type);

		if (it == atlas.end()) {
			std::cout << "Hostile type not found: " << type << std::endl;
			return;
		}

		Hostile* temp = new Hostile(atlas.at(type));
		temp->resetNode();

		
		timing.push(time);
		hostiles.push(temp);
	}

	//## Close file
	inFile.close();

	if (hostiles.size() != timing.size()) {
		std::cout << "Round loaded. Cardinality mismatch." << std::endl;
	}

	return;

}

bool Round::getSpawnState()
{
	return doSpawn;
}

