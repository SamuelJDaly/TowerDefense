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
	float scaleX = size / sprite.getGlobalBounds().width;
	float scaleY = size / sprite.getGlobalBounds().height;

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

		posX++;
	}

	//## Close File
	inFile.close();
}

void TileMap::setTileset(sf::Texture* newTileset)
{
	tileset.setTexture(newTileset);
	refreshTilemap();
}

void TileMap::refreshTilemap()
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			tilemap.at(i)->at(j)->setTexture(tileset.getTexture());
			//Calculate texture rect
			int idx = tilemap.at(i)->at(j)->getType();
			tilemap.at(i)->at(j)->setTextureRect(tileset.getRect(idx));
			//Set Position and scale
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
