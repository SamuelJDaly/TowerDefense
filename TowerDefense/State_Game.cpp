#include "State_Game.h"

//################################################################################################################
//				STATE: GAME
////##############################################################################################################

void State_Game::initGui()
{
	//GUI
	gui = new Gui();

	paletteSize = { viewSize_gui.x * paletteRatio.x, viewSize_gui.y * paletteRatio.y };
	palettePos = { viewSize_gui.x - paletteSize.x, 0 };

	Widget_Panel* panel = new Widget_Panel();
	panel->setTexture(textureHandler->lookup("panel_simpleWhite"));
	panel->setSize(paletteSize);

	panel->setPosition(palettePos);
	panel->setLayer(0);

	gui->addWidget(panel);

	//PALLETE

	Tower towerOne;
	Tower towerTwo;

	towerOne.setTexture(textureHandler->lookup("tower_1"));
	towerOne.setCooldown(.25f);

	towerTwo.setTexture(textureHandler->lookup("tower_2"));
	towerTwo.setCooldown(.1f);

	Projectile temp, tempTwo;
	temp.setTexture(textureHandler->lookup("projectile_2"));
	temp.setRange(600);
	temp.setDamage(4);
	temp.setSpeed(200);

	tempTwo.setTexture(textureHandler->lookup("projectile_1"));
	tempTwo.setRange(300);
	tempTwo.setDamage(1);
	tempTwo.setSpeed(250);

	towerOne.setProjectile(temp);
	towerTwo.setProjectile(tempTwo);
	towerOne.setScale(towerScale_palette);
	towerTwo.setScale(towerScale_palette);
	towerOne.setPosition(palettePos);
	towerTwo.setPosition(palettePos);

	palette.push_back(towerOne);
	palette.push_back(towerTwo);


	int row = 0;
	int col = 0;

	for (size_t i = 0; i < palette.size(); i++) {
		if (col >= paletteColumns) {
			row++;
			col = 0;
		}

		palette.at(i).move({ 50,50 });
		palette.at(i).move({ (paletteEntrySize * col), (paletteEntrySize * row) });

		if (col > 0) {
			palette.at(i).move({ (float)palettePadding, 0 });
		}

		if (row > 0) {
			palette.at(i).move({ 0, (float)palettePadding });
		}

		col++;
	}

}

void State_Game::initView()
{
	view_playField.setCenter(viewSize_playField.x / 2, viewSize_playField.y / 2);
	view_playField.setSize(viewSize_playField);
	view_gui.setSize(viewSize_gui);
	view_gui.setCenter(viewSize_gui.x / 2, viewSize_gui.y / 2);

	view_playField.setViewport(viewport_playField);
	view_gui.setViewport(viewport_gui);
}

void State_Game::initMap()
{
	tileMap = new TileMap();
	//tileMap->setTileset(textureHandler->lookup("tileset_dither"));
	tileMap->loadFromFile("resource/map/editor_tilemap.txt", *textureHandler);
	tileMap->refreshTilemap();
	tileMap->loadPath("resource/map/editor_path.txt");
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

	Projectile temp;
	temp.setTexture(textureHandler->lookup("projectile_1"));
	temp.setRange(600);
	temp.setDamage(7);
	temp.setSpeed(200);
	temp.setSize({ 7,7 });

	towers.push_back(new Tower());

	towers.back()->setTexture(textureHandler->lookup("tower_cannon"));
	towers.back()->setCooldown(1);
	towers.back()->setScale(towerScale_playfield);



	towers.back()->setProjectile(temp);
	towers.back()->setPosition({ 200, 20 });

	towers.push_back(new Tower());

	towers.back()->setTexture(textureHandler->lookup("tower_2"));

	temp.setDamage(5);
	temp.setTexture(textureHandler->lookup("projectile_2"));
	temp.setSize({ 8,2 });
	towers.back()->setProjectile(temp);
	towers.back()->setPosition({ 300, 300 });
	towers.back()->setScale(towerScale_playfield);

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

State_Game::State_Game(TextureHandler* textureHandler, sf::RenderWindow* window)
{
	this->textureHandler = textureHandler;
	this->window = window;

	if (!window) {
		std::cout << "Invalid window!!!" << std::endl;
		abort();
	}

	this->initGui();
	this->initView();
	this->initMap();
	this->initHostiles();
	this->initTest();

}

State_Game::~State_Game()
{

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
	//Button Release
	if (event.type == sf::Event::MouseButtonReleased) {
		// Get click coords
		// get the current mouse position in the window
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);

		// convert it to world coordinates
		sf::Vector2f worldPos = win.mapPixelToCoords(pixelPos, view_playField);
		sf::Vector2f guiPos = win.mapPixelToCoords(pixelPos, view_gui);


		//## Left Click
		if (event.key.code == sf::Mouse::Left) {

			if (isPalletePicked) {
				paletteDeselect();
			}


			//Tower Selection

			if (ctrlTower) {
				ctrlTower->setOverlayColor(sf::Color::White);
				ctrlTower->setDrawRange(false);
			}

			ctrlTower = nullptr;

			for (auto i : towers) {
				if (i->contains(worldPos)) {
					ctrlTower = i;
					ctrlTower->setOverlayColor({ 100,100,100 });
					ctrlTower->setDrawRange(true);
					break;
				}
			}
		}


		//## Right Click
		if (event.key.code == sf::Mouse::Right) {
			//Fire tower
			if (!ctrlTower) {
				return;
			}

			ctrlTower->setTarget(worldPos);
			ctrlTower->fire(projectiles);
		}
	}

	//Button press
	if (event.type == sf::Event::MouseButtonPressed) {
		if (event.key.code == sf::Mouse::Left) {
			if (!isPalletePicked) {
				paletteSelect();
			}
		}
	}


	//Scroll Wheel
	if (event.type == sf::Event::MouseWheelMoved) {
		float zoom = currZoom - (zoomSpeed * event.mouseWheel.delta);

		if (zoom < zoomBounds.x && zoom >= zoomBounds.y) {

			view_playField.zoom(1 / currZoom);
			currZoom = zoom;
			view_playField.zoom(currZoom);
		}

	}

	//###	KEYBOARD
	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::Space) {
			hostiles.push_back(new Hostile());
			hostiles.back()->setTexture(textureHandler->lookup("hostile_0"));
			hostiles.back()->setPath(tileMap->getPath());
			hostiles.back()->setSize({ 12,12 });
			hostiles.back()->resetNode();
		}
	}

}

void State_Game::paletteSelect()
{
	//Check location and create a copy if
	auto pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f pos = window->mapPixelToCoords(pixelPos, view_gui);
	for (size_t i = 0; i < palette.size(); i++) {
		if (palette.at(i).contains(pos)) {
			palettePick = new Tower(palette.at(i));
			palettePick->setScale(towerScale_playfield);

			isPalletePicked = true;
			break;
		}
	}
}

void State_Game::paletteDeselect()
{
	//Check release location
	towers.push_back(palettePick);

	palettePick = nullptr;
	isPalletePicked = false;
}

void State_Game::update(float dt)
{
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

	//Update palette selection (ie follow mouse)
	if (isPalletePicked && palettePick) {
		auto pixelPos = sf::Mouse::getPosition(*window);
		sf::Vector2f pos = window->mapPixelToCoords(pixelPos, view_playField);

		palettePick->setPosition(pos);
	}


	//Other
	gui->update(dt);
	updateCamera(dt);
	updateCollision();
	updateTargeting();
}

void State_Game::drawPallete(sf::RenderWindow& win)
{
	for (size_t i = 0; i < palette.size(); i++) {
		palette.at(i).draw(win);
	}


}

void State_Game::draw(sf::RenderWindow& win)
{
	//## Play Field
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

	//Pallete pick
	if (isPalletePicked && palettePick) {
		palettePick->draw(win);
	}

	//## GUI
	win.setView(view_gui);

	gui->draw(win);
	drawPallete(win);

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