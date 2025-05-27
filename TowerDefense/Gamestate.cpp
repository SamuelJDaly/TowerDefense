#include "Gamestate.h"




//##############################################################################################################
//				GAME
////##############################################################################################################

void State_Game::initMap()
{
	map = new Map(textureHandler);
	tileMap = new TileMap(textureHandler->lookup("tileset_0"));
	tileMap->loadFromFile("resource/map/map_0_tilemap.txt");
	tileMap->refreshTilemap();
	tileMap->loadPath("resource/map/map_0_path.txt");
}

void State_Game::initHostiles()
{
	//hostiles.push_back(new Hostile(textureHandler->lookup("hostile_0")));
	//hostiles.at(0)->setPath(tileMap->getPath());
	//hostiles.at(0)->resetNode();
	//hostiles.at(0)->setSize({12,12});
}

void State_Game::initTest()
{
	towers.push_back(new Tower());

	towers.at(0)->setTexture(textureHandler->lookup("tower_0"));
	towers.back()->setCooldown(.25);

	Projectile temp;
	temp.setTexture(textureHandler->lookup("projectile_0"));
	temp.setRange(600);
	temp.setDamage(1);
	temp.setSpeed(200);

	towers.at(0)->setProjectile(temp);
	towers.at(0)->setPosition({200, 70});

	towers.push_back(new Tower());

	towers.at(1)->setTexture(textureHandler->lookup("tower_0"));

	towers.at(1)->setProjectile(temp);
	towers.at(1)->setPosition({ 300, 300 });

	Hostile testHostile;
	testHostile.setTexture(textureHandler->lookup("hostile_0"));
	testHostile.setPath(tileMap->getPath());
	testHostile.setSize({ 12,12 });
	

	Round* testRound = new Round();
	testRound->addAtlasEntry("test", testHostile);
	testRound->loadFromFile("resource/rounds/round_0.txt");
	testRound->start();

	rounds.push(testRound);
}

State_Game::State_Game(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;
	
	this->initMap();
	this->initHostiles();
	this->initTest();
	
}

State_Game::~State_Game()
{
	delete map;

	delete tileMap;

	for (auto i : hostiles) {
		delete i;
	}

	for (auto i : projectiles) {
		delete i;
	}

	for (auto i : towers) {
		delete i;
	}
}

void State_Game::poll(sf::RenderWindow& win, sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonReleased) {
		if (event.key.code == sf::Mouse::Left) {
			//Select tower
			sf::Vector2f mousePos = { (float)(sf::Mouse::getPosition(win).x) , (float)(sf::Mouse::getPosition(win).y) };

			if (ctrlTower) {
				ctrlTower->setOverlayColor(sf::Color::White);
				ctrlTower->setDrawRange(false);
			}
			
			ctrlTower = nullptr;

			for (auto i : towers) {
				if (i->contains(mousePos)) {
					ctrlTower = i;
					ctrlTower->setOverlayColor({100,100,100});
					ctrlTower->setDrawRange(true);
					break;
				}
			}
		}

		if (event.key.code == sf::Mouse::Right) {
			//Fire tower
			if (!ctrlTower) {
				return;
			}
			sf::Vector2f mousePos = { (float)(sf::Mouse::getPosition(win).x) , (float)(sf::Mouse::getPosition(win).y)};
			
			ctrlTower->setTarget(mousePos);
			ctrlTower->fire(projectiles);
		}
	}


	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Space) {
			hostiles.push_back(new Hostile());
			hostiles.back()->setTexture(textureHandler->lookup("hostile_0"));
			hostiles.back()->setPath(tileMap->getPath());
			hostiles.back()->setSize({12,12});
			hostiles.back()->resetNode();
		}
	}

}

void State_Game::update(float dt)
{
	map->update(dt);
	tileMap->update(dt);
	
	//Update current round
	rounds.top()->update(dt);

	if (rounds.top()->getSpawnState()) {
		rounds.top()->spawn(hostiles);
	}

	//Update hostiles
	auto hosIt = hostiles.begin();
	for (auto i : hostiles) {
		//Apply damage if path end reached
		if (i->getPathDone()) {
			hp -= i->getHp();
			std::cout << "HP: " << hp << std::endl;
		}
		//Delete it if dead
		if (i->getDead()) {
			delete hostiles.at(std::distance(hostiles.begin(), hosIt));
			hosIt = hostiles.erase(hosIt);
			continue;
		}

		//Update it if not
		hosIt++;
		i->update(dt);
	}

	//Update towers
	for (auto i : towers) {
		i->update(dt);
	}

	//Update projectiles
	auto projIt = projectiles.begin();

	for (auto i : projectiles) {
		//Delete it if dead
		if (i->getDead()) {
			delete projectiles.at(std::distance(projectiles.begin(), projIt));
			projIt = projectiles.erase(projIt);
			continue;
		}

		//Update it if not
		projIt++;
		i->update(dt);
	}


	//Collision
	updateCollision();
	updateTargeting();
}

void State_Game::draw(sf::RenderWindow& win)
{
	//map->draw(win);
	tileMap->draw(win);

	for (auto i : hostiles) {
		i->draw(win);
	}

	for (auto i : towers) {
		i->draw(win);
	}

	for (auto i : projectiles) {
		i->draw(win);
	}

	
}

void State_Game::updateCollision()
{
	if (hostiles.size() == 0 || projectiles.size() == 0) {
		return;
	}

	//Naive approach
	for (auto p : projectiles) {
		for (auto h : hostiles) {
			if (p->getBounds().intersects(h->getBounds())) {
				h->takeDamage(p->getDamage(), p->getDamageType());
				p->die();
			}
		}
	}
}

void State_Game::updateTargeting()
{
	for (auto i : hostiles) {
		for (auto j : towers) {
			if (j->inRange(i->getPos())) {
				j->setTarget(i->getPos());
				j->fire(projectiles);
			}
		}
	}
}

//##############################################################################################################
//				MENU
////##############################################################################################################

Gamestate_Menu::Gamestate_Menu()
{

}

Gamestate_Menu::~Gamestate_Menu()
{
}

void Gamestate_Menu::update(float dt)
{
}

void Gamestate_Menu::draw(sf::RenderWindow& win)
{
}
