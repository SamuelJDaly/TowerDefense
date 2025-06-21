#include "Map.h"

//#######################################################################################
// TILE

Tile::Tile()
{
}

Tile::~Tile()
{
}

void Tile::update(float deltaTime)
{
	//Nothing for now
}

void Tile::draw(sf::RenderWindow& window)
{
	window.draw(sprite);
}

void Tile::setTexture(sf::Texture* texture)
{
	sprite.setTexture(*texture);
}

void Tile::setTextureRect(sf::IntRect rect)
{
	sprite.setTextureRect(rect);
}

void Tile::setType(sf::Vector2i newType)
{
	type = newType;
}

void Tile::setSize(float size)
{
	//Calculate scale
	float scaleX = size / sprite.getLocalBounds().width;
	float scaleY = size / sprite.getLocalBounds().height;

	//Set scale
	sprite.setScale({ scaleX, scaleY });

}

void Tile::setPosition(sf::Vector2f pos)
{
	sprite.setPosition(pos);
}

sf::Vector2i Tile::getType()
{
	return type;
}


//################################################################################################
//MAP

void Map::drawNodes(sf::RenderWindow& win)
{
	sf::CircleShape point;
	point.setRadius(2);
	point.setOrigin({point.getRadius(), point.getRadius()});

	Node* curr = pathHead;
	Node* last = nullptr;

	sf::Color currCol = sf::Color::Blue;
	sf::Color lastCol = sf::Color::Blue;

	sf::Vertex line[] = {sf::Vertex(sf::Vector2f(0,0)), sf::Vertex(sf::Vector2f(0,0)) };

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
}

Map::Map(TextureHandler* textureHandler)
{
	this->textureHandler = textureHandler;

	background.setTexture(*textureHandler->lookup("bg_test"));
	loadPath("resource/map/map_0_path.txt");
}

Map::~Map()
{
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

void Map::update(float dt)
{
}

void Map::draw(sf::RenderWindow& win)
{
	//Draw Background
	win.draw(background);

	//Draw nodes
	if (doDrawNodes) {
		this->drawNodes(win);
	}
}

void Map::loadPath(std::string filepath)
{
	Node* temp = fReadPath(filepath);
	if (!temp) {
		std::cout << "Path not loaded..." << std::endl;
		return;
	}

	pathHead = temp;
}

Node* Map::getPath()
{
	return pathHead;
}


//#######################################################################################################
// TILEMAP

void TileMap::DrawNodes(sf::RenderWindow& win)
{
	sf::CircleShape point;
	point.setRadius(2);
	point.setOrigin({ point.getRadius(), point.getRadius() });

	Node* curr = pathHead;
	Node* last = nullptr;

	sf::Color currCol = sf::Color::Blue;
	sf::Color lastCol = sf::Color::Blue;

	sf::Vertex line[] = { sf::Vertex(sf::Vector2f(0,0)), sf::Vertex(sf::Vector2f(0,0)) };

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
}

TileMap::TileMap(sf::Texture* initTileset)
{
	tileset = initTileset;
}

TileMap::~TileMap()
{
}

void TileMap::update(float deltaTime)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tilemap.at(i)->at(j)->update(deltaTime);
		}
	}
}

void TileMap::draw(sf::RenderWindow& window)
{

	if (hasBackground) {
		window.draw(background);
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tilemap.at(i)->at(j)->draw(window);
		}
	}

	if (doDrawNodes) {
		this->DrawNodes(window);
	}
}

void TileMap::loadFromFile(std::string filepath)
{
	std::ifstream inFile;

	//## Open File
	inFile.open(filepath);

	if (!inFile.is_open()) {
		std::cerr << "ERROR: Could not open world file: " << filepath << std::endl;
		exit(1);
	}

	//## Read File
	//Dimensions
	inFile >> width >> height;


	//Allocate tilemap
	for (int i = 0; i < height; i++) {
		//Allocate row
		tilemap.push_back(new std::vector<Tile*>);

		for (int j = 0; j < width; j++) {
			//Allocate column
			tilemap.at(i)->push_back(new Tile);
			tilemap.at(i)->at(j)->setTexture(tileset);
			tilemap.at(i)->at(j)->setPosition({ float(j * tileSize),float(i * tileSize) });
		}
	}

	//Tiles
	int curr = -1;
	int next = -1;
	int posX = 0;
	int posY = 0;

	while (inFile >> curr >> next) {
		if (posX >= width) {
			//Go to the next row
			posX = 0;
			posY++;
		}

		if (posY >= height) {
			//Then we are done
			break;
		}

		//Set texture rect
		int left = tileSize * curr;
		int top = tileSize * next;
		//std::cout << "Rect: " << left << ", " << top << std::endl;
		tilemap.at(posY)->at(posX)->setTextureRect({ left,top,tileSize,tileSize });
		tilemap.at(posY)->at(posX)->setType({ curr,next });

		posX++;
	}

	//## Close File
	inFile.close();
}

void TileMap::setTileset(sf::Texture* newTileset)
{
	tileset = newTileset;
	refreshTilemap();
}

void TileMap::refreshTilemap()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < height; j++) {
			tilemap.at(i)->at(j)->setTexture(tileset);
			int left = tilemap.at(i)->at(j)->getType().x * tileSize;
			int top = tilemap.at(i)->at(j)->getType().y * tileSize;
			tilemap.at(i)->at(j)->setTextureRect({ left, top, tileSize, tileSize });
			tilemap.at(i)->at(j)->setPosition({ float(j * tileSize),float(i * tileSize) });
			tilemap.at(i)->at(j)->setSize(tileSize);
		}
	}
}

void TileMap::setBackground(sf::Texture* texture, sf::Vector2f size)
{
	if (!texture) {
		std::cerr << "ERROR: Invalid background texture" << std::endl;
		return;
	}
	background.setTexture(*texture);
	float scaleX = size.x / texture->getSize().x;
	float scaleY = size.y / texture->getSize().y;

	background.setScale(scaleX, scaleY);
	hasBackground = true;
}

void TileMap::loadPath(std::string filepath)
{
	Node* temp = fReadPath(filepath);
	if (!temp) {
		std::cout << "Path not loaded..." << std::endl;
		return;
	}

	pathHead = temp;

	pathOffset(pathHead, { (float)(tileSize / 2.0), (float)(tileSize / 2.0) });
}

Node* TileMap::getPath()
{
	return pathHead;
}
