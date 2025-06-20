#include "Gamestate.h"




//################################################################################################################
//				GAME
////##############################################################################################################

void State_Game::initGui()
{
	//GUI
	gui = new Gui();
	
	palleteSize = { viewSize_gui.x * palleteRatio.x, viewSize_gui.y*palleteRatio.y };
	palletePos = {viewSize_gui.x - palleteSize.x, 0};

	Widget_Panel* panel = new Widget_Panel();
	panel->setTexture(textureHandler->lookup("panel_girder"));
	panel->setSize(palleteSize);

	panel->setPosition(palletePos);
	panel->setLayer(0);

	gui->addWidget(panel);

	//PALLETE

	Tower towerOne;
	Tower towerTwo;

	towerOne.setTexture(textureHandler->lookup("tower_1"));
	towerOne.setCooldown(.25);

	towerTwo.setTexture(textureHandler->lookup("tower_2"));
	towerTwo.setCooldown(.1);

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
	towerOne.setScale(towerScale_pallete);
	towerTwo.setScale(towerScale_pallete);
	towerOne.setPosition(palletePos);
	towerTwo.setPosition(palletePos);

	pallete.push_back(towerOne);
	pallete.push_back(towerTwo);


	int row = 0;
	int col = 0;

	for (size_t i = 0; i < pallete.size(); i++) {
		if (col >= palleteColumns) {
			row++;
			col = 0;
		}

		pallete.at(i).move({50,50});
		pallete.at(i).move({(palleteEntrySize*col), (palleteEntrySize*row)});

		if (col > 0) {
			pallete.at(i).move({ (float)palletePadding, 0 });
		}

		if (row > 0) {
			pallete.at(i).move({0, (float)palletePadding});
		}

		col++;
	}

}

void State_Game::initView()
{
	view_playField.setCenter(viewSize_playField.x/2, viewSize_playField.y/2);
	view_playField.setSize(viewSize_playField);
	view_gui.setSize(viewSize_gui);
	view_gui.setCenter(viewSize_gui.x / 2, viewSize_gui.y / 2);

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

	towers.back()->setTexture(textureHandler->lookup("tower_1"));
	towers.back()->setCooldown(.25);
	towers.back()->setScale(towerScale_playfield);

	Projectile temp;
	temp.setTexture(textureHandler->lookup("projectile_1"));
	temp.setRange(600);
	temp.setDamage(1);
	temp.setSpeed(200);
	temp.setSize({ 4,4 });

	towers.back()->setProjectile(temp);
	towers.back()->setPosition({200, 70});

	towers.push_back(new Tower());

	towers.back()->setTexture(textureHandler->lookup("tower_2"));

	temp.setDamage(5);
	temp.setTexture(textureHandler->lookup("projectile_2"));
	temp.setSize({8,2});
	towers.back()->setProjectile(temp);
	towers.back()->setPosition({300, 300});
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
				palleteDeselect();
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
					ctrlTower->setOverlayColor({100,100,100});
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
				palleteSelect();
			}
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

void State_Game::palleteSelect()
{
	//Check location and create a copy if
	auto pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f pos = window->mapPixelToCoords(pixelPos, view_gui);
	for (size_t i = 0; i < pallete.size(); i++) {
		if (pallete.at(i).contains(pos)) {
			palletePick = new Tower(pallete.at(i));
			palletePick->setScale(towerScale_playfield);
			
			isPalletePicked = true;
			break;
		}
	}
}

void State_Game::palleteDeselect()
{
	//Check release location
	towers.push_back(palletePick);

	palletePick = nullptr;
	isPalletePicked = false;
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

	//Update pallete selection (ie follow mouse)
	if (isPalletePicked && palletePick) {
		auto pixelPos = sf::Mouse::getPosition(*window);
		sf::Vector2f pos = window->mapPixelToCoords(pixelPos, view_playField);

		palletePick->setPosition(pos);
	}


	//Other
	gui->update(dt);
	updateCamera(dt);
	updateCollision();
	updateTargeting();
}

void State_Game::drawPallete(sf::RenderWindow &win)
{
	for (size_t i = 0; i < pallete.size(); i++) {
		pallete.at(i).draw(win);
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
	if (isPalletePicked && palletePick) {
		palletePick->draw(win);
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


//####################################################################################################################
//			EDITOR
//####################################################################################################################

void State_Editor::initGui() {
	//Gui
	gui = new Gui();
	font = new sf::Font();

	if (!font->loadFromFile("resource/font/jmhtype.ttf")) {
		return;
	}

	//Left Panel
	Widget_Panel* leftPanel = new Widget_Panel();
	leftPanel->setTexture(textureHandler->lookup("panel_bevel"));
	leftPanel->setSize({ viewSize_gui.x * leftPanelRatio, 1*viewSize_gui.y});

	//Bottom Panel
	Widget_Panel* bottomPanel = new Widget_Panel();
	bottomPanel->setTexture(textureHandler->lookup("panel_bevel"));
	
	sf::Vector2f bottomPanelSize = {viewSize_gui.x-leftPanel->getSize().x, viewSize_gui.y * bottomPanelRatio};
	bottomPanel->setSize(bottomPanelSize);
	bottomPanel->setPosition({ leftPanel->getSize().x,   viewSize_gui.y - bottomPanelSize.y});

	//Text box
	Widget_Textbox* textBox = new Widget_Textbox();
	textBox->setFont(font);
	textBox->setLayer(2);
	textBox->setPos({10,10});
	textBox->setText("Test");
	textBox->setSize({200,20});

	//Add to gui
	gui->addWidget(leftPanel);
	gui->addWidget(bottomPanel);
	gui->addWidget(textBox);


	//Pallete setup
	palleteSize.x = palleteRatio.x * leftPanel->getSize().x;
	palleteSize.y = palleteRatio.y * leftPanel->getSize().y;

	palletePos.x = .5 * (leftPanel->getSize().x - palleteSize.x) + leftPanel->getPos().x;
	palletePos.y = .5 * (leftPanel->getSize().y - palleteSize.y) + leftPanel->getPos().y;


	palleteBorder.setFillColor(sf::Color::Transparent);
	palleteBorder.setOutlineColor(sf::Color::Black);
	palleteBorder.setOutlineThickness(1);
	palleteBorder.setPosition({ palletePos.x - 1, palletePos.y - 1});
	palleteBorder.setSize({palleteSize.x + 2, palleteSize.y + 2});

	selectBorder.setFillColor(sf::Color::Transparent);
	selectBorder.setOutlineColor(sf::Color::Green);
	selectBorder.setOutlineThickness(1);


}

void State_Editor::initCamera()
{
	//Map
	viewSize_map = {(float)window->getSize().x - (leftPanelRatio * (float)window->getSize().x), (float)window->getSize().y - ((float)window->getSize().y * bottomPanelRatio)};
	viewport_map = {leftPanelRatio,0,1-leftPanelRatio,1-bottomPanelRatio};

	view_map.setCenter(viewSize_map.x / 2, viewSize_map.y / 2);
	view_map.setSize(viewSize_map);
	view_map.setViewport(viewport_map);
	

	//Gui
	view_gui.setSize(viewSize_gui);
	view_gui.setCenter(viewSize_gui.x / 2, viewSize_gui.y / 2);
	view_gui.setViewport(viewport_gui);
}

void State_Editor::initTest()
{
	this->loadPallete(16,"resource/tex/tileset_0.png");
}

void State_Editor::initMap()
{
	float scale = tileSize / blankTexture->getSize().x;

	//Map
	for (int i = 0; i < mapSize.y;i++) {
		std::vector<int> dataRow;
		std::vector<sf::Sprite> displayRow;
		for (int j = 0; j < mapSize.x;j++) {
			dataRow.push_back(-1);
			sf::Sprite curr;
			curr.setTexture(*blankTexture);
			curr.setScale(scale,scale);
			curr.setPosition({j*tileSize, i*tileSize});

			displayRow.push_back(curr);
		}
		map.push_back(dataRow);
		mapDisplay.push_back(displayRow);
	}

	//Grid
	for (int i = 0; i <= mapSize.x; i++) {
		sf::Vertex a;
		sf::Vertex b;
		a.color = gridColor;
		b.color = gridColor;

		a.position = { i * tileSize , 0};
		b.position = { i * tileSize, tileSize * mapSize.y };

		grid_vertical.push_back(a);
		grid_vertical.push_back(b);
	}

	for (int i = 0; i <= mapSize.y; i++) {
		sf::Vertex a;
		sf::Vertex b;
		a.color = gridColor;
		b.color = gridColor;

		a.position = {0, i *tileSize};
		b.position = {tileSize * mapSize.x, i * tileSize };

		grid_horizontal.push_back(a);
		grid_horizontal.push_back(b);
	}
}

State_Editor::State_Editor(TextureHandler* textureHandler, sf::RenderWindow* window)
{
	//Create blank texture
	blankImage.create(1,1,sf::Color::White);

	blankTexture = new sf::Texture();
	blankTexture->loadFromImage(blankImage);


	//Init State
	this->window = window;
	this->textureHandler = textureHandler;
	this->initCamera();
	this->initGui();
	this->initTest();
	this->initMap();
}

State_Editor::~State_Editor()
{
	delete gui;
	delete font;
	delete blankTexture;
}

void State_Editor::loadPallete(int txSize, std::string filepath)
{
	//This function sets up the pallete vector by texturing, scaling, and positioning the sprites
	spritesheet.setTextureSize(txSize, txSize);
	spritesheet.fload(filepath);
	

	//Loop through spritesheet textures
	int row = 0;
	int col = 0;
	for (int i = 0; i < spritesheet.getNumTextures(); i++) {
		sf::Sprite curr;

		//Texturing
		curr.setTexture(*spritesheet.getTexture());
		curr.setTextureRect(spritesheet.getRect(i));

		//Scaling
		float colX = palleteSize.x / palleteColumns; //col width = width of pallete / num columns

		float scale = colX / curr.getTextureRect().width; //scale = width of column / width of texture

		curr.setScale(scale, scale);

		//Positioning
		float posX = palletePos.x + (colX * col);
		float posY = palletePos.y + (colX * row); //Bc we are encforcing square tiles we just reuse colX
		curr.setPosition(posX,posY);

		//Iterate row and column
		col++;
		if (col >= palleteColumns) {
			col = 0;
			row++;
		}

		//Add to pallete
		pallete.push_back(curr);
	}

	//Set up selection border
	selectBorder.setSize({ (float)pallete.at(0).getGlobalBounds().width, (float)pallete.at(0).getGlobalBounds().height });

}

void State_Editor::saveMap(std::string filepath)
{
	std::ofstream outFile;

	outFile.open(filepath);

	if (!outFile.is_open()) {
		std::cout << "Could not open file: " << filepath << std::endl;
		return;
	}

	//Write size
	outFile << mapSize.x << mapSize.y << "\n";

	//Write tiles
	for (int i = 0; i < mapSize.y; i++) {
		for (int j = 0; j < mapSize.x; j++) {
			outFile << map.at(i).at(j);
		}
		outFile << "\n";
	}
}

void State_Editor::poll(sf::RenderWindow& win, sf::Event& event) {
	if (event.type == sf::Event::MouseButtonReleased) {
		// Get click coords
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
		sf::Vector2f mapPos = win.mapPixelToCoords(pixelPos, view_map);
		sf::Vector2f guiPos = win.mapPixelToCoords(pixelPos, view_gui);
		

		if (event.key.code == sf::Mouse::Left) {
			palleteSelect = -1;
			for (size_t i = 0; i < pallete.size(); i++) {
				if (pallete.at(i).getGlobalBounds().contains(guiPos)) {
					palleteSelect = i;
					selectBorder.setPosition(pallete.at(i).getPosition());
					break;
				}
			}
		}

		
	}

	if (event.type == sf::Event::MouseButtonPressed) {
		// Get click coords
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
		sf::Vector2f mapPos = win.mapPixelToCoords(pixelPos, view_map);
		sf::Vector2f guiPos = win.mapPixelToCoords(pixelPos, view_gui);
		if (event.key.code == sf::Mouse::Right) {
			for (int i = 0; i < mapSize.y; i++) {
				for (int j = 0; j < mapSize.x; j++) {
					if (mapDisplay.at(i).at(j).getGlobalBounds().contains(mapPos)) {
						if (palleteSelect < 0 || palleteSelect >= spritesheet.getNumTextures()) {

						}
						else {
							float scale = tileSize / spritesheet.getRect(palleteSelect).width;
							map.at(i).at(j) = palleteSelect;
							mapDisplay.at(i).at(j).setTexture(*spritesheet.getTexture());
							mapDisplay.at(i).at(j).setTextureRect(spritesheet.getRect(palleteSelect));
							mapDisplay.at(i).at(j).setScale(scale, scale);
						}
					}
				}
			}
		}
	}

	gui->poll(win, event);
}

void State_Editor::updateCamera(float dt)
{
	//Up
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		if (view_map.getCenter().y >= cameraBounds.top) {
			view_map.move({ 0,-1 * panSpeed * dt });
		}
	}

	//Down
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		if (view_map.getCenter().y <= cameraBounds.height) {
			view_map.move({ 0,panSpeed * dt });
		}
	}
	//Left
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		if (view_map.getCenter().x >= cameraBounds.left) {
			view_map.move({ -1 * panSpeed * dt,0 });
		}
	}
	//Right
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		if (view_map.getCenter().x <= cameraBounds.width) {
			view_map.move({ panSpeed * dt,0 });
		}
	}
}

void State_Editor::update(float dt) {
	gui->update(dt);
	updateCamera(dt);
}

void State_Editor::drawMap(sf::RenderWindow& win)
{
	//Tiles
	for (size_t i = 0; i < mapDisplay.size(); i++) {
		for (size_t j = 0; j < mapDisplay.at(i).size(); j++) {
			win.draw(mapDisplay.at(i).at(j));
		}
	}

	//Grid
	for (size_t i = 0; i < grid_horizontal.size(); i+=2) {
		sf::Vertex gridLine[2] = {grid_horizontal.at(i), grid_horizontal.at(i+1)};
		win.draw(gridLine, 2, sf::Lines);
	}

	for (size_t i = 0; i < grid_vertical.size(); i += 2) {
		sf::Vertex gridLine[2] = { grid_vertical.at(i), grid_vertical.at(i + 1) };
		win.draw(gridLine, 2, sf::Lines);
	}
}

void State_Editor::drawPallete(sf::RenderWindow& win) {
	for (size_t i = 0; i < pallete.size(); i++) {
		win.draw(pallete.at(i));
	}

	if (palleteSelect >= 0) {
		win.draw(selectBorder);
	}

	win.draw(palleteBorder);
}


void State_Editor::draw(sf::RenderWindow& win) {
	//Draw map
	win.setView(view_map);
	this->drawMap(win);


	//Draw Gui
	win.setView(view_gui);
	gui->draw(win);
	this->drawPallete(win);
}