#include "State_Editor.h"

//####################################################################################################################
//			STATE: EDITOR
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
	pnl_left->setSize({ viewSize_gui.x * leftPanelRatio, 1 * viewSize_gui.y });

	//## Bottom Panel
	pnl_bottom = new Widget_TabbedPanel();
	pnl_bottom->setPanelTexture(textureHandler->lookup("panel_bevel"));
	pnl_bottom->setTabTexture(textureHandler->lookup("tab_simple"));
	pnl_bottom->setTabFont(font);
	pnl_bottom->setTabTitleColor(sf::Color::Black);
	pnl_bottom->addTab(2);

	pnl_bottom->setTabTitle(0, "Path Tool");
	pnl_bottom->setTabTitle(1, "Round Tool");

	sf::Vector2f pnl_bottomSize = { viewSize_gui.x - pnl_left->getSize().x, viewSize_gui.y * bottomPanelRatio };
	pnl_bottom->setSize(pnl_bottomSize);
	bottomPanelPos = { pnl_left->getSize().x,   viewSize_gui.y - pnl_bottomSize.y };
	pnl_bottom->setPos(bottomPanelPos);


	//## Save button
	btn_save = new Widget_Button();
	btn_save->setLayer(2);
	btn_save->setPosition(pnl_left->getPos());
	btn_save->move({ 20,20 });
	btn_save->setTexture(textureHandler->lookup("btn_save"));
	btn_save->setSize({ 20,20 });

	//## Size buttons
	//X increase
	btn_SizeUpX = new Widget_Button();
	btn_SizeUpX->setLayer(2);
	btn_SizeUpX->setPosition(pnl_left->getPos());
	btn_SizeUpX->move({ 70, pnl_left->getSize().y - 60 });
	btn_SizeUpX->setTexture(textureHandler->lookup("btn_plus"));
	btn_SizeUpX->setSize({ 20,20 });

	//X decrease
	btn_SizeDnX = new Widget_Button(*btn_SizeUpX);
	btn_SizeDnX->setTexture(textureHandler->lookup("btn_minus"));
	btn_SizeDnX->move({ 0,25 });

	//Y increase
	btn_SizeUpY = new Widget_Button(*btn_SizeUpX);
	btn_SizeUpY->move({ 85,0 });

	//Y decrease
	btn_SizeDnY = new Widget_Button(*btn_SizeDnX);
	btn_SizeDnY->move({ 85,0 });

	//## Load Tilset Button
	btn_loadTilset = new Widget_Button();
	btn_loadTilset->setTexture(textureHandler->lookup("btn_palette"));
	btn_loadTilset->setPosition({ 20,50 });
	btn_loadTilset->setLayer(2);
	btn_loadTilset->setSize({ 20,20 });

	//## Map size label
	label_mapSize = new Widget_Label();
	label_mapSize->setPosition(pnl_left->getPos());
	label_mapSize->move({ 100, pnl_left->getSize().y - 50 });
	label_mapSize->setFont(font);
	label_mapSize->setCharacterSize(14);
	label_mapSize->setText("10 x 10");


	//## Text box
	//Map Path 
	txtBx_mapPath = new Widget_Textbox();
	txtBx_mapPath->setPos({ 50,20 });
	txtBx_mapPath->setFont(font);
	txtBx_mapPath->setCharacterSize(12);
	txtBx_mapPath->setSize({ 200,20 });

	//Pallete Path
	txtBx_palettePath = new Widget_Textbox(*txtBx_mapPath);
	txtBx_palettePath->move({ 0.f,30.f });

	//## Add to gui
	gui->addWidget(pnl_left);
	gui->addWidget(pnl_bottom);
	gui->addWidget(btn_save);
	gui->addWidget(btn_loadTilset);
	gui->addWidget(btn_SizeUpX);
	gui->addWidget(btn_SizeDnX);
	gui->addWidget(btn_SizeUpY);
	gui->addWidget(btn_SizeDnY);
	gui->addWidget(label_mapSize);
	gui->addWidget(txtBx_palettePath);
	gui->addWidget(txtBx_mapPath);

	gui->setView(&view_gui);
}

void State_Editor::initCamera()
{
	//Map
	viewSize_map = { (float)window->getSize().x - (leftPanelRatio * (float)window->getSize().x), (float)window->getSize().y - ((float)window->getSize().y * bottomPanelRatio) };
	viewport_map = { leftPanelRatio,0,1 - leftPanelRatio,1 - bottomPanelRatio };

	view_map.setCenter(viewSize_map.x / 2, viewSize_map.y / 2);
	view_map.setSize(viewSize_map);
	view_map.setViewport(viewport_map);


	//Gui
	view_gui.setSize(viewSize_gui);
	view_gui.setCenter(viewSize_gui.x / 2, viewSize_gui.y / 2);
	view_gui.setViewport(viewport_gui);
}

void State_Editor::initMap()
{
	//## Tile Map
	tilemap = new TileMap(); //alloc
	tilemap->setTilesize(tileSize); //tilesize
	tilemap->resize(mapSize.x, mapSize.y); //dims

	//## Grid
	//Vertical lines
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

	//Horizontal lines
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

	//Boundry (used for mouse click logic)
	mapBoundry = { 0,0,mapSize.x * tileSize, mapSize.y * tileSize };

}

void State_Editor::initPalleteTool()
{
	//## Pallete setup

	//Size and positioning
	paletteSize.x = paletteRatio.x * pnl_left->getSize().x;
	paletteSize.y = paletteRatio.y * pnl_left->getSize().y;

	palettePos.x = .5f * (pnl_left->getSize().x - paletteSize.x) + pnl_left->getPos().x;
	palettePos.y = .5f * (pnl_left->getSize().y - paletteSize.y) + pnl_left->getPos().y;

	//Border Box
	paletteBorder.setFillColor(sf::Color::Transparent);
	paletteBorder.setOutlineColor(sf::Color::Black);
	paletteBorder.setOutlineThickness(1);
	paletteBorder.setPosition({ palettePos.x - 1, palettePos.y - 1 });
	paletteBorder.setSize({ paletteSize.x + 2, paletteSize.y + 2 });

	//Selection box
	selectBorder.setFillColor(sf::Color::Transparent);
	selectBorder.setOutlineColor(sf::Color::Green);
	selectBorder.setOutlineThickness(1);
}

void State_Editor::initPathTool()
{
	nodeButton.setRadius(10);
	nodeButton.setFillColor(sf::Color::Blue);
	nodeButton.setPosition(bottomPanelPos);
	nodeButton.move(20, 50);


	float radius = 10;
	nodeDisplay.setRadius(radius);
	nodeDisplay.setFillColor(sf::Color::Blue);
	nodeDisplay.setPosition(nodeButton.getPosition());
	nodeDisplay.setOrigin({ radius,radius });
}

void State_Editor::initTest()
{
	this->loadPallete(16, "resource/tex/tileset_0.png");
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

void State_Editor::remNode()
{
	//Delete the last node in the path
	if (!pathHead) {
		return;
	}

	if (!pathEnd) {
		//One node case
		delete pathHead;
		pathHead = nullptr;
	}
	else if (pathEnd->last->type == NodeType::BEGIN) {
		//Two node case
		pathHead->next = nullptr;
		delete pathEnd;
		pathEnd = nullptr;
	}
	else {
		//Three or more node case
		Node* temp = pathEnd->last;
		temp->next = nullptr;
		temp->type = NodeType::END;
		delete pathEnd;
		pathEnd = temp;
	}


	return;
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
	blankImage.create(1, 1, sf::Color::White);

	blankTexture = new sf::Texture();
	blankTexture->loadFromImage(blankImage);


	//Init State
	this->window = window;
	this->textureHandler = textureHandler;
	this->initCamera();
	this->initGui();
	this->initPalleteTool();
	this->initMap();
	this->initPathTool();
	this->initTest();
}

State_Editor::~State_Editor()
{
	delete gui;
	delete font;
	delete blankTexture;

	//Delete path
	freePath(pathHead);

	delete tilemap;

}

//###############################################################################################	GENERAL

void State_Editor::loadPallete(int txSize, std::string filepath)
{
	//This function sets up the palette vector by texturing, scaling, and positioning the sprites
	//## Check for valid filepath
	std::ifstream inFile;
	inFile.open(filepath);
	if (!inFile) {
		std::cout << "Cannot open file: " << filepath << std::endl;
		return;
	}


	inFile.close();


	spritesheet.setTextureSize(txSize, txSize);
	if (!spritesheet.fload(filepath)) {
		std::cout << "Spritesheet load failed" << std::endl;
		return;
	}

	//Clear palette
	palette.clear();

	//Assign each spritesheet texture to a representative sprite in the palette (ie what user clicks on to select given texture)
	int row = 0;
	int col = 0;

	for (int i = 0; i < spritesheet.getNumTextures(); i++) {
		sf::Sprite curr;

		//Texturing
		curr.setTexture(*spritesheet.getTexture());
		curr.setTextureRect(spritesheet.getRect(i));

		//Scaling
		float colX = paletteSize.x / paletteColumns; //col width = width of palette / num columns

		float scale = colX / curr.getTextureRect().width; //scale = width of column / width of texture

		curr.setScale(scale, scale);

		//Positioning
		float posX = palettePos.x + (colX * col);
		float posY = palettePos.y + (colX * row); //Bc we are encforcing square tiles we just reuse colX
		curr.setPosition(posX, posY);

		//Iterate row and column
		col++;
		if (col >= paletteColumns) {
			col = 0;
			row++;
		}

		//Add to palette
		palette.push_back(curr);
	}

	//Set up selection border
	selectBorder.setSize({ (float)palette.at(0).getGlobalBounds().width, (float)palette.at(0).getGlobalBounds().height });

	//Update the tilemap spritesheet data
	tilemap->setTileset(spritesheet);
	tilemap->setTexturesize(txSize);
	tilemap->setTilesetName(strFilter(trimFileExt(filepath), "resource/tex/"));
}

void State_Editor::saveMap(std::string filepath)
{
	tilemap->writeToFile(filepath);
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

void State_Editor::loadMap(std::string filepath) {
	tilemap->loadFromFile(filepath, *textureHandler);
}

void State_Editor::loadPath(std::string filepath) {
	//NOT IMPLEMENTED
}

void State_Editor::load(std::string mapname) {
	loadMap(mapname);
	loadPath(mapname);
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
				isPainting = false; //???? debounce ?
			}


			//Pallete select
			if (paletteBorder.getGlobalBounds().contains(guiPos)) {
				nodePlace = false;
				paletteSelect = -1;
				for (size_t i = 0; i < palette.size(); i++) {
					if (palette.at(i).getGlobalBounds().contains(guiPos)) {
						paletteSelect = (int)i;
						selectBorder.setPosition(palette.at(i).getPosition());
						break;
					}
				}
			}

			//Path Tool select
			if (pnl_bottom->getTabSel() == 0) {
				if (nodePlace && !nodeButton.getGlobalBounds().contains(guiPos)) {
					nodePlace = false;
				}

				if (!nodePlace && nodeButton.getGlobalBounds().contains(guiPos)) {
					nodePlace = true;
					isPainting = false;
					paletteSelect = -1;
				}
			}


			//Resize buttons
			if (mapSize.x > 1 && btn_SizeDnX->getState() == ButtonState::PRESS) {
				//Then reduce X size
				mapSize.x--;
				tilemap->resize(mapSize.x, mapSize.y);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
				mapBoundry.width -= tileSize;
				cameraBounds.width -= tileSize;
				refreshGrid();
			}

			if (btn_SizeUpX->getState() == ButtonState::PRESS) {
				//Then increase X size
				mapSize.x++;
				tilemap->resize(mapSize.x, mapSize.y);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
				mapBoundry.width += tileSize;
				cameraBounds.width += tileSize;
				refreshGrid();
			}

			if (mapSize.y > 1 && btn_SizeDnY->getState() == ButtonState::PRESS) {
				//Then reduce Y size
				mapSize.y--;
				tilemap->resize(mapSize.x, mapSize.y);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
				mapBoundry.height -= tileSize;
				cameraBounds.height -= tileSize;
				refreshGrid();
			}

			if (btn_SizeUpY->getState() == ButtonState::PRESS) {
				//Then increase Y size
				mapSize.y++;
				tilemap->resize(mapSize.x, mapSize.y);
				label_mapSize->setText(std::to_string(mapSize.x) + " x " + std::to_string(mapSize.y));
				mapBoundry.height += tileSize;
				cameraBounds.height += tileSize;
				refreshGrid();
			}

			if (btn_loadTilset->getState() == ButtonState::PRESS) {
				//Then load the palette
				this->loadPallete(16, txtBx_palettePath->getText());
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

				int sample = tilemap->getType(idxX, idxY);

				if (sample >= 0) {
					paletteSelect = sample;
					selectBorder.setPosition(palette.at(sample).getPosition());
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

		if (event.key.code == sf::Keyboard::Backspace) {
			if (nodePlace) {
				remNode();
			}
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
	if (pnl_bottom->getTabSel() != 0) {
		return;
	}

	//Get Mouse Pos
	sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
	sf::Vector2f mousePos_map = window->mapPixelToCoords(pixelPos, view_map);
	sf::Vector2f mousePos_gui = window->mapPixelToCoords(pixelPos, view_gui);

	if (nodeButton.getGlobalBounds().contains(mousePos_gui)) {
		nodeButton.setFillColor(sf::Color(0, 0, 200, 255));
	}
	else {
		nodeButton.setFillColor(sf::Color(0, 0, 255, 255));
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
	if (!isPainting || paletteSelect == -1) {
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

	tilemap->modTile(idxX, idxY, paletteSelect);

	isChanged = true;

}

void State_Editor::update(float dt) {
	gui->update(dt);
	updateCamera(dt);
	updatePainting();
	updatePathTool();

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
	tilemap->draw(win);

	//Grid
	for (size_t i = 0; i < grid_horizontal.size(); i += 2) {
		sf::Vertex gridLine[2] = { grid_horizontal.at(i), grid_horizontal.at(i + 1) };
		win.draw(gridLine, 2, sf::Lines);
	}

	for (size_t i = 0; i < grid_vertical.size(); i += 2) {
		sf::Vertex gridLine[2] = { grid_vertical.at(i), grid_vertical.at(i + 1) };
		win.draw(gridLine, 2, sf::Lines);
	}
}

void State_Editor::drawPallete(sf::RenderWindow& win) {
	for (size_t i = 0; i < palette.size(); i++) {
		win.draw(palette.at(i));
	}

	if (paletteSelect >= 0) {
		win.draw(selectBorder);
	}

	win.draw(paletteBorder);
}

void State_Editor::drawRoundTool(sf::RenderWindow& win)
{
	sf::RectangleShape test;
	test.setSize({ 100, 100 });
	test.setFillColor(sf::Color::Red);
	test.setPosition(pnl_bottom->getPos());
	test.move({ 20,20 });

	win.draw(test);
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
	switch (pnl_bottom->getTabSel()) {
	case 0:
		//Draw Path Tool
		this->drawPathTool(win);
		break;
	case 1:
		//Draw Round Tool
		this->drawRoundTool(win);
		break;
	default:
		//Do nothing
		break;
	}

}

