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

void Tile::setType(int newType)
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

int Tile::getType()
{
	return type;
}


//#######################################################################################################
// TILEMAP

TileMap::TileMap()
{
	
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
		drawPath(pathHead, window);
	}
}

void TileMap::loadFromFile(std::string filepath, TextureHandler& textureHandler)
{
	std::ifstream inFile;

	//## Open File
	inFile.open(filepath);

	if (!inFile.is_open()) {
		std::cerr << "ERROR: Could not open map file: " << filepath << std::endl;
		exit(1);
	}

	//## Read File
	//Dimensions
	inFile >> width >> height;
	inFile >> tileSize >> textureSize;
	inFile >> tileSetName >> tileSetPath;

	//Try to find tileset texture
	sf::Texture* tilesetTexture = textureHandler.lookup(tileSetName);
	if (!tilesetTexture) {
		//Then load into texture handler
		textureHandler.addTexture(tileSetName, tileSetPath);
		tilesetTexture = textureHandler.lookup(tileSetName);
	}

	tileset.setTextureSize({textureSize,textureSize});
	tileset.setTexture(tilesetTexture);

	//Allocate tilemap
	for (int i = 0; i < height; i++) {
		//Allocate row
		tilemap.push_back(new std::vector<Tile*>);

		for (int j = 0; j < width; j++) {
			//Allocate column
			tilemap.at(i)->push_back(new Tile);
			tilemap.at(i)->at(j)->setTexture(tileset.getTexture());
			tilemap.at(i)->at(j)->setPosition({ float(j * tileSize),float(i * tileSize) });
		}
	}

	//Tiles
	int curr = -1;
	int next = -1;
	int posX = 0;
	int posY = 0;

	while (inFile >> curr) {
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
		
		//std::cout << "Rect: " << left << ", " << top << std::endl;
		tilemap.at(posY)->at(posX)->setTextureRect(tileset.getRect(curr));
		tilemap.at(posY)->at(posX)->setType(curr);
		if (curr >= 0) {
			tilemap.at(posY)->at(posX)->setTexture(tileset.getTexture());
		}

		posX++;
	}

	//## Close File
	inFile.close();
}

void TileMap::writeToFile(std::string filepath)
{
	std::ofstream outFile;

	//Open File
	outFile.open(filepath);

	if (!outFile.is_open()) {
		std::cout << "Could not open file: " << filepath << std::endl;
		return;
	}

	//Header
	outFile << width << " " << height << "\n";
	outFile << tileSize << " " << textureSize << "\n";
	outFile << tileSetName << " " << tileSetPath << "\n";


	//Tiles
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			outFile << tilemap.at(i)->at(j)->getType() << " ";
		}
		outFile << "\n";
	}

	//Close File
	outFile.close();
}

void TileMap::setTileset(Spritesheet newSet)
{
	tileset = newSet;
	tileSetPath = newSet.getTexturePath();
	refreshTilemap();
}

void TileMap::refreshTilemap()
{
	//Basically we need to either assign a given rect from the spritesheet if the tile type is valid
	// (ie. corresponds to a spritesheet rect), or the the blank texture if not


	//For each tile in the tilemap, row major
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			Tile* curr = tilemap.at(i)->at(j);
			//Set texture and rect
			if (!tileset.containsIdx(curr->getType())) {
				//Then assign blank texture
				curr->setTexture(tileset.getTexture());
				curr->setTextureRect({0,0,1,1});
			}
			else {
				curr->setTexture(tileset.getTexture());
				curr->setTextureRect(tileset.getRect(curr->getType()));
			}

			//Set size
			curr->setSize(tileSize);
			
			//Set Position
			curr->setPosition({(float)(tileSize * j), (float)(tileSize * i) });

		}
	}
}

void TileMap::modTile(int x, int y, int type)
{
	//Out of y bounds
	if (height <= x) {
		return;
	}

	//Out of x bounds
	if (width <= x) {
		return;
	}

	//Blank Texture
	if (!tileset.containsIdx(type)) {
		tilemap.at(y)->at(x)->setTexture(tileset.getTexture());
		tilemap.at(y)->at(x)->setTextureRect({0,0,1,1});
		tilemap.at(y)->at(x)->setType(type);
		tilemap.at(y)->at(x)->setSize(tileSize);
		std::cout << "Set blank texture at: " << x << ", " << y << std::endl;
		return;
	}

	//Normal Texture
	tilemap.at(y)->at(x)->setTexture(tileset.getTexture());
	tilemap.at(y)->at(x)->setTextureRect(tileset.getRect(type));
	tilemap.at(y)->at(x)->setType(type);
	tilemap.at(y)->at(x)->setSize(tileSize);
	std::cout << "Set spritesheet texture at: " << x << ", " << y << std::endl;
}

void TileMap::resize(int newWidth, int newHeight)
{
	//## Height
	//Increase
	if (newHeight > height) {
		for (int i = height; i < newHeight; i++) {
			std::vector<Tile*>* dataRow = new std::vector<Tile*>;

			for (int j = 0; j < width; j++) {
				Tile* curr = new Tile();
				curr->setTexture(tileset.getTexture());
				curr->setTextureRect({0,0,1,1});
				curr->setSize(tileSize);
				dataRow->push_back(curr);
			}

			tilemap.push_back(dataRow);
		}
	}

	//Decrease
	if (newHeight < height) {
		//Check for min size
		if (newHeight <= 0) {
			return;
		}

		//Shrink
		for (int i = newHeight; i < height; i++) {
			for (int j = 0; j < width; j++) {
				delete tilemap.at(i)->at(j);
			}
			tilemap.at(i)->clear();

			delete tilemap.at(i);
			tilemap.pop_back();
		}
	}

	height = newHeight;

	//## Width
	//Increase
	if (newWidth > width) {
		for (int i = 0; i < height; i++) {
			for (int j = width; j < newWidth; j++) {
				Tile* curr = new Tile();
				curr->setTexture(tileset.getTexture());
				curr->setTextureRect({0,0,1,1});
				curr->setSize(tileSize);
				tilemap.at(i)->push_back(curr);
			}
		}
	}

	//Decrease
	if (newWidth < width) {
		//Check for min size
		if (newWidth <= 0) {
			return;
		}

		//Shrink
		for (int i = 0; i < height; i++) {
			for (int j = newWidth; j < width; j++) {
				delete tilemap.at(i)->at(j);
				tilemap.at(i)->pop_back();
			}
		}
	}


	//## Set new size values
	width = newWidth;

	refreshTilemap();
}

int TileMap::getType(int x, int y)
{
	//Check Bounds
	if (y >= height || x >= width) {
		return -1;
	}

	//Get type at index
	return tilemap.at(y)->at(x)->getType();
}

void TileMap::setTilesetName(std::string name)
{
	tileSetName = name;
}

void TileMap::setTilesize(float newSize)
{
	tileSize = newSize;
	this->refreshTilemap();
}

void TileMap::setTexturesize(float newSize)
{
	textureSize = newSize;
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
}

Node* TileMap::getPath()
{
	return pathHead;
}
