#include "Tower.h"

Projectile::Projectile()
{
}

Projectile::~Projectile()
{
}

void Projectile::update(float dt)
{
	//Check for death
	if (dist(graph.getPosition(), spawn) >= range) {
		this->die();
		return;
	}

	//Move
	//std::cout << "Moving at: " << velocity.x << ", " << velocity.y << std::endl;
	graph.move(velocity.x * dt, velocity.y * dt);
}

void Projectile::draw(sf::RenderWindow& win)
{
	win.draw(graph);
}

void Projectile::setTexture(sf::Texture* texture)
{
	graph.setTexture(*texture);	
	graph.setTextureRect({0,0,(int)texture->getSize().x, (int)texture->getSize().y});
	graph.setOrigin(graph.getGlobalBounds().width / 2.f, graph.getGlobalBounds().height / 2.f);
}

void Projectile::die()
{
	isDead = true;
}

void Projectile::setSpawn(sf::Vector2f newSpawn)
{
	spawn = newSpawn;
}

void Projectile::setVelocity(sf::Vector2f newVelocity)
{
	velocity = newVelocity;
}

void Projectile::setTarget(sf::Vector2f target)
{
	//Get angle
	sf::Vector2f posDiff = diff(spawn, target);
	float theta = atan2f(posDiff.y, posDiff.x);
	graph.setRotation(radToDeg(theta));

	//Get components
	velocity.x = speed * cosf(theta);
	velocity.y = speed * sinf(theta);
}

void Projectile::setRange(float newRange)
{
	range = newRange;
}

void Projectile::setPosition(sf::Vector2f newPos)
{
	graph.setPosition(newPos);
}

void Projectile::setDamage(float newDamage)
{
	damage = newDamage;
}

void Projectile::setSpeed(float newSpeed)
{
	speed = newSpeed;
}

void Projectile::setSize(sf::Vector2f newSize) {
	float scaleX = newSize.x / graph.getGlobalBounds().width;
	float scaleY = newSize.y / graph.getGlobalBounds().height;

	graph.setScale(scaleX, scaleY);
}


float Projectile::getSpeed()
{
	return speed;
}

bool Projectile::getDead()
{
	return isDead;
}

sf::FloatRect Projectile::getBounds()
{
	return graph.getGlobalBounds();
}

float Projectile::getDamage()
{
	return damage;
}

DamageType Projectile::getDamageType()
{
	return dmgType;
}

Tower::Tower()
{
}

Tower::~Tower()
{
}

void Tower::update(float dt)
{
	//Update cooldown timer
	if (!canFire) {
		cooldownTimer += dt;
		if (cooldownTimer >= cooldownThreshold) {
			cooldownTimer = 0;
			canFire = true;
		}
	}

	

}

void Tower::draw(sf::RenderWindow& win)
{
	win.draw(graph);

	if (doDrawRange) {
		sf::CircleShape circle;
		circle.setRadius(range);
		circle.setOrigin(range, range);
		circle.setPosition(graph.getPosition());
		circle.setFillColor({0,200,0,20});
		circle.setOutlineColor({0,255,0});
		circle.setOutlineThickness(1);

		win.draw(circle);
	}
}

void Tower::setTexture(sf::Texture* texture)
{
	graph.setTexture(*texture);
	graph.setOrigin(texture->getSize().x / 2, texture->getSize().y / 2);

	size = {graph.getGlobalBounds().width, graph.getGlobalBounds().height};
}

void Tower::setProjectile(Projectile archetype)
{
	projectileType = archetype;
}

void Tower::setPosition(sf::Vector2f newPosition)
{
	graph.setPosition(newPosition);
}

sf::Vector2f Tower::getPosition()
{
	return graph.getPosition();
}

void Tower::move(sf::Vector2f offset)
{
	graph.move(offset);
}

void Tower::setSize(sf::Vector2f newSize) {
	size = newSize;

	float scaleX = size.x / graph.getGlobalBounds().width;
	float scaleY = size.y / graph.getGlobalBounds().height;

	graph.setScale(scaleX, scaleY);

}

void Tower::setScale(float scale)
{
	auto baseSize = graph.getTexture()->getSize();

	sf::Vector2f newSize = {(float)(baseSize.x * scale), (float)(baseSize.y * scale)};
	size = newSize;
	float scaleX = size.x / baseSize.x;
	float scaleY = size.y / baseSize.y;

	graph.setScale(scaleX, scaleY);
}

sf::Vector2f Tower::getSize()
{
	return size;
}

void Tower::setTarget(sf::Vector2f newTarget)
{
	this->target = newTarget;
}

void Tower::fire(std::vector<Projectile*> &projectileList)
{
	if (!canFire) {
		return;
	}

	//## Create new projectile
	Projectile* temp = new Projectile(projectileType);
	temp->setSpawn(graph.getPosition());
	temp->setPosition(graph.getPosition());
	temp->setTarget(target);

	//## Add projectile to given list (so its lifecycle can be managed)
	projectileList.push_back(temp);

	canFire = false;
}

void Tower::setOverlayColor(sf::Color col)
{
	graph.setColor(col);
}

void Tower::setCooldown(float newCooldown)
{
	cooldownThreshold = newCooldown;
}

void Tower::setDrawRange(bool state)
{
	doDrawRange = state;
}

bool Tower::contains(sf::Vector2f pos)
{
	if (graph.getGlobalBounds().contains(pos)) {
		return true;
	}

	return false;
}

bool Tower::inRange(sf::Vector2f pos)
{
	if (dist(pos,graph.getPosition()) <= range) {
		return true;
	}

	return false;
}
