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
	panel->setTexture(textureHandler->lookup("panel_simpleWhite"));
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
	tileMap = new TileMap();
	tileMap->setTileset(textureHandler->lookup("tileset_dither"));
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
	towers.back()->setPosition({200, 20});

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


//########################################	UTIL
void State_Editor::initGui() {
	//## Gui
	gui = new Gui();
	font = new sf::Font();

	if (!font->loadFromFile("resource/font/roboto_regular.ttf")) {
		return;
	}

	//## Left Panel
	pnl_left = new Widget_Panel();
	pnl_left->setTexture(textureHandler->lookup("panel_bevel"));
	pnl_left->setSize({ viewSize_gui.x * leftPanelRatio, 1*viewSize_gui.y});

	//## Bottom Panel
	pnl_bottom = new Widget_Panel();
	pnl_bottom->setTexture(textureHandler->lookup("panel_bevel"));
	
	sf::Vector2f pnl_bottomSize = {viewSize_gui.x-pnl_left->getSize().x, viewSize_gui.y * bottomPanelRatio};
	pnl_bottom->setSize(pnl_bottomSize);
	bottomPanelPos = { pnl_left->getSize().x,   viewSize_gui.y - pnl_bottomSize.y };
	pnl_bottom->setPosition(bottomPanelPos);


	//## Save button
	btn_save = new Widget_Button();
	btn_save->setLayer(2);
	btn_save->setPosition(pnl_left->getPos());
	btn_save->move({ 20,20 });
	btn_save->setTexture(textureHandler->lookup("btn_save"));
	btn_save->setSize({20,20});

	//## Size buttons
	//X increase
	btn_SizeUpX = new Widget_Button();
	btn_SizeUpX->setLayer(2);
	btn_SizeUpX->setPosition(pnl_left->getPos());
	btn_SizeUpX->move({70, pnl_left->getSize().y - 60});
	btn_SizeUpX->setTexture(textureHandler->lookup("btn_plus"));
	btn_SizeUpX->setSize({ 20,20 });

	//X decrease
	btn_SizeDnX = new Widget_Button(*btn_SizeUpX);
	btn_SizeDnX->setTexture(textureHandler->lookup("btn_minus"));
	btn_SizeDnX->move({0,25});

	//Y increase
	btn_SizeUpY = new Widget_Button(*btn_SizeUpX);
	btn_SizeUpY->move({85,0});

	//Y decrease
	btn_SizeDnY = new Widget_Button(*btn_SizeDnX);
	btn_SizeDnY->move({85,0});

	//## Map size label
	label_mapSize = new Widget_Label();
	label_mapSize->setPosition(pnl_left->getPos());
	label_mapSize->move({ 100, pnl_left->getSize().y - 50 });
	label_mapSize->setFont(font);
	label_mapSize->setCharacterSize(14);
	label_mapSize->setText("10 x 10");
	

	//## Text box
	Widget_Textbox* textbox = new Widget_Textbox();
	textbox->setPos({ 40,20 });
	textbox->setFont(font);
	textbox->setCharacterSize(12);
	textbox->setSize({100,20});

	//## Add to gui
	gui->addWidget(pnl_left);
	gui->addWidget(pnl_bottom);
	gui->addWidget(btn_save);
	gui->addWidget(btn_SizeUpX);
	gui->addWidget(btn_SizeDnX);
	gui->addWidget(btn_SizeUpY);
	gui->addWidget(btn_SizeDnY);
	gui->addWidget(label_mapSize);
	gui->addWidget(textbox);
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
	this->loadPallete(16,"resource/tex/tileset_dither.png");
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

	//Boundry
	mapBoundry = {0,0,mapSize.x * tileSize, mapSize.y * tileSize};

}

void State_Editor::initPalleteTool()
{
	//Pallete setup
	palleteSize.x = palleteRatio.x * pnl_left->getSize().x;
	palleteSize.y = palleteRatio.y * pnl_left->getSize().y;

	palletePos.x = .5 * (pnl_left->getSize().x - palleteSize.x) + pnl_left->getPos().x;
	palletePos.y = .5 * (pnl_left->getSize().y - palleteSize.y) + pnl_left->getPos().y;


	palleteBorder.setFillColor(sf::Color::Transparent);
	palleteBorder.setOutlineColor(sf::Color::Black);
	palleteBorder.setOutlineThickness(1);
	palleteBorder.setPosition({ palletePos.x - 1, palletePos.y - 1 });
	palleteBorder.setSize({ palleteSize.x + 2, palleteSize.y + 2 });

	selectBorder.setFillColor(sf::Color::Transparent);
	selectBorder.setOutlineColor(sf::Color::Green);
	selectBorder.setOutlineThickness(1);
}

void State_Editor::initPathTool()
{
	nodeButton.setRadius(10);
	nodeButton.setFillColor(sf::Color::Blue);
	nodeButton.setPosition(bottomPanelPos);
	nodeButton.move(20,20);


	float radius = 10;
	nodeDisplay.setRadius(radius);
	nodeDisplay.setFillColor(sf::Color::Blue);
	nodeDisplay.setPosition(nodeButton.getPosition());
	nodeDisplay.setOrigin({ radius,radius });
}

void State_Editor::addNode(sf::Vector2f pos)
{
	//Create new node
	Node* newNode = new Node();
	newNode->pos = pos;
	newNode->next = nullptr;
	newNode->type = NodeType::BEGIN;

	if (!pathHead) {
		pathHead = newNode;
		return;
	}

	if (!pathEnd) {
		pathEnd = newNode;
		pathHead->next = newNode;
		newNode->last = pathHead;
		newNode->type = NodeType::END;
		return;
	}

	pathEnd->type = NodeType::PATH;
	newNode->type = NodeType::END;
	newNode->last = pathEnd;
	pathEnd->next = newNode;
	pathEnd = newNode;

	
	return;

}

void State_Editor::resizeMapX(int newX)
{
	//Increase
	if (newX > mapSize.x) {
		for (int i = 0; i < mapSize.y; i++) {
			for (int j = mapSize.x; j < newX; j++) {
				map.at(i).push_back(-1);

				sf::Sprite curr;
				curr.setTexture(*blankTexture);
				curr.setScale(tileSize / blankTexture->getSize().x, tileSize / blankTexture->getSize().y);
				curr.setPosition(tileSize*j, tileSize*i);
				mapDisplay.at(i).push_back(curr);
			}
		}
	}

	//Decrease
	if (newX < mapSize.x) {
		//Check for min size
		if (newX < 0) {
			return;
		}

		//Shrink
		for (int i = 0; i < mapSize.y; i++) {
			for (int j = newX; j < mapSize.x; j++) {
				map.at(i).pop_back();
				mapDisplay.at(i).pop_back();
			}
		}
	}

	//Store new size
	mapSize.x = newX;
	refreshGrid();
}

void State_Editor::resizeMapY(int newY)
{
	//Increase
	if (newY > mapSize.y) {
		for (int i = mapSize.y; i < newY; i++) {
			std::vector<int> dataRow;
			std::vector<sf::Sprite> displayRow;

			for (int j = 0; j < mapSize.x; j++) {
				dataRow.push_back(-1);
				sf::Sprite curr;
				curr.setTexture(*blankTexture);
				curr.setScale(tileSize / blankTexture->getSize().x, tileSize / blankTexture->getSize().y);
				curr.setPosition(tileSize * j, tileSize * i);
				displayRow.push_back(curr);
			}

			map.push_back(dataRow);
			mapDisplay.push_back(displayRow);
		}
	}

	//Decrease
	if (newY < mapSize.y) {
		//Check for min size
		if (newY < 0) {
			return;
		}

		//Shrink
		for (int i = newY; i < mapSize.y; i++) {
			map.pop_back();
			mapDisplay.pop_back();
		}
	}

	//Store new size
	mapSize.y = newY;
	refreshGrid();
}

void State_Editor::refreshGrid()
{
	//Clear
	grid_horizontal.clear();
	grid_vertical.clear();

	//Set up
	for (int i = 0; i <= mapSize.x; i++) {
		sf::Vertex a;
		sf::Vertex b;
		a.color = gridColor;
		b.color = gridColor;

		a.position = { i * tileSize , 0 };
		b.position = { i * tileSize, tileSize * mapSize.y };

		grid_vertical.push_back(a);
		grid_vertical.push_back(b);
	}

	for (int i = 0; i <= mapSize.y; i++) {
		sf::Vertex a;
		sf::Vertex b;
		a.color = gridColor;
		b.color = gridColor;

		a.position = { 0, i * tileSize };
		b.position = { tileSize * mapSize.x, i * tileSize };

		grid_horizontal.push_back(a);
		grid_horizontal.push_back(b);
	}

	//Boundry
	mapBoundry = { 0,0,mapSize.x * tileSize, mapSize.y * tileSize };
}

//########################################	CONSTRUCTORS AND DESTRUCTOR

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
	this->initMap();
	this->initPalleteTool();
	this->initTest();
	this->initPathTool();
}

State_Editor::~State_Editor()
{
	delete gui;
	delete font;
	delete blankTexture;

	//Delete path
	Node* curr = pathHead;
	Node* next = nullptr;

	while (curr) {
		//Store next
		next = curr->next;

		//Delete curr
		delete curr;

		//Iterate
		curr = next;
	}

}

//###############################################################################################	MISC

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
	outFile << mapSize.x << " " << mapSize.y << "\n";

	//Write tiles
	for (int i = 0; i < mapSize.y; i++) {
		for (int j = 0; j < mapSize.x; j++) {
			outFile << map.at(i).at(j) << " ";
		}
		outFile << "\n";
	}

	//Close outfile
	outFile.close();
}

void State_Editor::savePath(std::string filepath)
{
	if (!pathHead) {
		return;
	}
	fWritePath(filepath, pathHead);
}

void State_Editor::save()
{
	this->saveMap(filename + "_tilemap.txt");
	this->savePath(filename + "_path.txt");
}

//################################################################################	POLLING

void State_Editor::poll(sf::RenderWindow& win, sf::Event& event) {
	if (event.type == sf::Event::MouseButtonReleased) {
		// Get click coords
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
		sf::Vector2f mapPos = win.mapPixelToCoords(pixelPos, view_map);
		sf::Vector2f guiPos = win.mapPixelToCoords(pixelPos, view_gui);
		
		//## LEFT
		if (event.key.code == sf::Mouse::Left) {
			//Painting
			if (isPainting) {
				isPainting = false;
			}

			
			//Pallete select
			if (palleteBorder.getGlobalBounds().contains(guiPos)) {
				nodePlace = false;
				palleteSelect = -1;
				for (size_t i = 0; i < pallete.size(); i++) {
					if (pallete.at(i).getGlobalBounds().contains(guiPos)) {
						palleteSelect = (int)i;
						selectBorder.setPosition(pallete.at(i).getPosition());
						break;
					}
				}
			}

			//Path Tool select
			if (nodePlace && !nodeButton.getGlobalBounds().contains(guiPos)) {
				nodePlace = false;
			}

			if (!nodePlace && nodeButton.getGlobalBounds().contains(guiPos)) {
				nodePlace = true;
				isPainting = false;
			}

			
			//Resize buttons
			if (btn_SizeDnX->getState() == ButtonState::PRESS) {
				//Then reduce size
				resizeMapX(mapSize.x-1);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
			}

			if (btn_SizeUpX->getState() == ButtonState::PRESS) {
				//Then increase size
				resizeMapX(mapSize.x+1);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
			}

			if (btn_SizeDnY->getState() == ButtonState::PRESS) {
				//Then reduce size
				resizeMapY(mapSize.y-1);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
			}

			if (btn_SizeUpY->getState() == ButtonState::PRESS) {
				//Then increase size
				resizeMapY(mapSize.y+1);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
			}


			

		}

		//## RIGHT
		if (event.key.code == sf::Mouse::Right) {
			//Path Tool place
			if (nodePlace && mapBoundry.contains(mapPos)) {
				this->addNode(mapPos);
			}
		}

		//## MIDDLE
		if (event.key.code == sf::Mouse::Middle) {
			//Sample
			if (mapBoundry.contains(mapPos)) {
				int idxX = (int)(mapPos.x / tileSize);
				int idxY = (int)(mapPos.y / tileSize);

				int sample = map.at(idxY).at(idxX);

				if (sample >= 0) {
					palleteSelect = sample;
					selectBorder.setPosition(pallete.at(sample).getPosition());
				}

			}
		}

	}

	if (event.type == sf::Event::MouseButtonPressed) {
		// Get click coords
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
		sf::Vector2f mapPos = win.mapPixelToCoords(pixelPos, view_map);
		sf::Vector2f guiPos = win.mapPixelToCoords(pixelPos, view_gui);

		//Left
		if (event.key.code == sf::Mouse::Left) {
			//Painting
			if (mapBoundry.contains(mapPos)) {
				isPainting = true;
				prevMap = map;
				prevMapDisplay = mapDisplay;
			}
		}


	}

	if (event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::LControl) {
			isPress_ctrl = true;
		}

		if (event.key.code == sf::Keyboard::Z) {
			isPress_z = true;
		}

		if (event.key.code == sf::Keyboard::S) {
			isPress_s = true;
		}
	}

	if (event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::LControl) {
			isPress_ctrl = false;
		}

		if (event.key.code == sf::Keyboard::Z) {
			isPress_z = false;
		}

		if (event.key.code == sf::Keyboard::S) {
			isPress_s = false;
		}
	}

	//Scroll Wheel
	if (event.type == sf::Event::MouseWheelMoved) {
		float zoom = currZoom - (zoomSpeed * event.mouseWheel.delta);

		if (zoom < zoomBounds.x && zoom >= zoomBounds.y) {

			view_map.zoom(1 / currZoom);
			currZoom = zoom;
			view_map.zoom(currZoom);
		}

	}

	gui->poll(win, event);
}


//########################################	UPDATING
void State_Editor::updatePathTool()
{
	//Get Mouse Pos
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mousePos_map = window->mapPixelToCoords(pixelPos, view_map);
	sf::Vector2f mousePos_gui = window->mapPixelToCoords(pixelPos, view_gui);

	if (nodeButton.getGlobalBounds().contains(mousePos_gui)) {
		nodeButton.setFillColor(sf::Color(0, 0, 200, 255));
	}
	else {
		nodeButton.setFillColor(sf::Color(0,0,255,255));
	}

	if (!nodePlace) {
		return;
	}
	


	nodeDisplay.setPosition(mousePos_map);
}

void State_Editor::updateCamera(float dt)
{
	if (isPress_ctrl || gui->getFocus()) {
		return;
	}

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

void State_Editor::updatePainting()
{
	if (!isPainting || palleteSelect == -1) {
		return;
	}

	//Get Mouse Pos
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mousePos_map = window->mapPixelToCoords(pixelPos, view_map);

	//Check if inside map
	if (!mapBoundry.contains(mousePos_map)) {
		return;
	}

	//Convert mouse pos to map index
	int idxX = (int)(mousePos_map.x / tileSize);
	int idxY = (int)(mousePos_map.y / tileSize);

	float scale = tileSize / spritesheet.getRect(palleteSelect).width;
	map.at(idxY).at(idxX) = palleteSelect;
	mapDisplay.at(idxY).at(idxX).setTexture(*spritesheet.getTexture());
	mapDisplay.at(idxY).at(idxX).setTextureRect(spritesheet.getRect(palleteSelect));
	mapDisplay.at(idxY).at(idxX).setScale(scale, scale);

	isChanged = true;

}

void State_Editor::update(float dt) {
	gui->update(dt);
	updateCamera(dt);
	updatePainting();
	updatePathTool();

	if (isChanged && isPress_ctrl && isPress_z) {
		//Undo
		isChanged = false;
		map = prevMap;
		mapDisplay = prevMapDisplay;
	}

	if (btn_save->getState() == ButtonState::PRESS || (isPress_ctrl && isPress_s)) {
		//Save
		std::cout << "Saving..." << std::endl;
		this->save();
	}

}



//########################################	DRAWING

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

void State_Editor::drawPathTool(sf::RenderWindow& win) {
	win.draw(nodeButton);
}

void State_Editor::drawPath(sf::RenderWindow& win)
{
	//Path lines
	for (size_t i = 0; i < pathLines.size(); i += 2) {
		sf::Vertex pathLine[2] = { pathLines.at(i), pathLines.at(i + 1) };
		win.draw(pathLine, 2, sf::Lines);
	}

	//Path nodes
	sf::CircleShape point;
	point.setRadius(5);
	point.setOrigin({ point.getRadius(), point.getRadius() });

	Node* curr = pathHead;
	Node* last = nullptr;

	sf::Color currCol = sf::Color::Blue;
	sf::Color lastCol = sf::Color::Blue;

	while (curr) {
		//Draw point
		point.setPosition(curr->pos);
		switch (curr->type) {
		case NodeType::BEGIN:
			point.setFillColor(sf::Color::Green);
			currCol = sf::Color::Green;
			break;
		case NodeType::PATH:
			point.setFillColor(sf::Color::Blue);
			currCol = sf::Color::Blue;
			break;
		case NodeType::END:
			point.setFillColor(sf::Color::Red);
			currCol = sf::Color::Red;
			break;
		}

		win.draw(point);

		//Draw lines
		sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0,0)), sf::Vertex(sf::Vector2f(0,0)) };
		line[0].position = curr->pos;
		line[0].color = currCol;
		if (last) {
			line[1].position = last->pos;
			line[1].color = lastCol;
			win.draw(line, 2, sf::Lines);
		}

		//Get next node
		last = curr;
		lastCol = currCol;
		curr = curr->next;

	}

	//Node Cursor
	if (nodePlace) {
		win.draw(nodeDisplay);
	}
}

void State_Editor::draw(sf::RenderWindow& win) {
	//Draw map
	win.setView(view_map);
	this->drawMap(win);
	this->drawPath(win);


	//Draw Gui
	win.setView(view_gui);
	gui->draw(win);
	this->drawPallete(win);
	this->drawPathTool(win);

}