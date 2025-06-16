#include "Gamestate.h"




//##############################################################################################################
//				GAME
////##############################################################################################################

void State_Game::initGui()
{
	gui = new Gui();


	Widget_Panel* panel = new Widget_Panel();
	panel->setTexture(textureHandler->lookup("panel_stone"));
	panel->setSize({ 500,500 });
	panel->setPosition({20,20});
	panel->setLayer(0);

	gui->addWidget(panel);
}

void State_Game::initView()
{
	view_playField.setCenter(viewSize_playField.x/2, viewSize_playField.y/2);
	view_playField.setSize(viewSize_playField);
	//view_gui.setSize(viewSize_gui);

	view_playField.setViewport(viewport_playField);
	view_gui.setViewport(viewport_gui);
}

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

	temp.setDamage(5);
	towers.at(1)->setProjectile(temp);
	towers.at(1)->setPosition({ 300, 300 });
	

	Hostile testHostile;
	testHostile.setTexture(textureHandler->lookup("hostile_0"));
	testHostile.setPath(tileMap->getPath());
	testHostile.setSize({ 12,12 });
	

	Round* testRound = new Round();
	testRound->addAtlasEntry("test", testHostile);
	testRound->loadFromFile("resource/rounds/round_0.txt");

	rounds.push(testRound);
	rounds.top()->start();
}

State_Game::State_Game(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;
	
	this->initGui();
	this->initView();
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

	delete gui;
}

void State_Game::poll(sf::RenderWindow& win, sf::Event& event)
{
	//###	MOUSE
	//Button
	if (event.type == sf::Event::MouseButtonReleased) {
		// Get click coords
		// get the current mouse position in the window
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);

		// convert it to world coordinates
		sf::Vector2f worldPos = win.mapPixelToCoords(pixelPos, view_playField);


		if (event.key.code == sf::Mouse::Left) {
			//Select tower

			if (ctrlTower) {
				ctrlTower->setOverlayColor(sf::Color::White);
				ctrlTower->setDrawRange(false);
			}
			
			ctrlTower = nullptr;

			for (auto i : towers) {
				if (i->contains(worldPos)) {
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
			
			ctrlTower->setTarget(worldPos);
			ctrlTower->fire(projectiles);
		}
	}


	//Scroll Wheel
	if (event.type == sf::Event::MouseWheelMoved) {
		float zoom = currZoom - (zoomSpeed * event.mouseWheel.delta);
		
		if (zoom < zoomBounds.x && zoom >= zoomBounds.y) {
			
			view_playField.zoom(1/currZoom);
			currZoom = zoom;
			view_playField.zoom(currZoom);
		}

		std::cout << currZoom << std::endl;
		
	}

	//###	KEYBOARD
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
	if (!rounds.empty()) {
		rounds.top()->update(dt);
		if (rounds.top()->getSpawnState()) {
			rounds.top()->spawn(hostiles);
		}
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


	//Other
	gui->update(dt);
	updateCamera(dt);
	updateCollision();
	updateTargeting();
}

void State_Game::draw(sf::RenderWindow& win)
{
	//# Play Field
	win.setView(view_playField);

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

	//# GUI
	win.setView(view_gui);

	//gui->draw(win);
	
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

void State_Game::updateCamera(float dt)
{
	//Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (view_playField.getCenter().y >= cameraBounds.top) {
			view_playField.move({ 0,-1 * panSpeed * dt });
		}
	}

	//Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (view_playField.getCenter().y <= cameraBounds.height) {
			view_playField.move({ 0,panSpeed * dt });
		}
	}
	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (view_playField.getCenter().x >= cameraBounds.left) {
			view_playField.move({ -1 * panSpeed * dt,0 });
		}
	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (view_playField.getCenter().x <= cameraBounds.width) {
			view_playField.move({ panSpeed * dt,0 });
		}
	}
}

//##############################################################################################################
//				MENU
////##############################################################################################################

void State_Menu::initGui()
{
	gui = new Gui();

	Widget_Panel* panel = new Widget_Panel();
	panel->setTexture(textureHandler->lookup("panel_0"));
	panel->setSize({100,100});
	panel->setLayer(0);

	Widget_Panel* panel2 = new Widget_Panel();
	panel2->setTexture(textureHandler->lookup("panel_1"));
	panel2->setSize({ 100,100 });
	panel2->setLayer(2);
	panel2->setPosition({50,50});

	Widget_Label* label = new Widget_Label();
	label->setFont(&font);
	label->setText("Test");
	label->setLayer(3);


	gui->addWidget(panel);
	gui->addWidget(panel2);
	gui->addWidget(label);
	

}

State_Menu::State_Menu(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;
	if (!font.loadFromFile("resource/font/jmhtype.ttf")) {
		
	}
	this->initGui();
}

State_Menu::~State_Menu()
{
	delete gui;
}

void State_Menu::poll(sf::RenderWindow& win, sf::Event& event)
{

}

void State_Menu::update(float dt)
{
	gui->update(dt);
}

void State_Menu::draw(sf::RenderWindow& win)
{
	gui->draw(win);
}
