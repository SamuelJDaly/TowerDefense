#include "Util.h"


float dist(sf::Vector2f posA, sf::Vector2f posB)
{
	//sqrt(~x^2 + ~y^2)
	return std::sqrtf(powf(posA.x - posB.x, 2) + powf(posA.y - posB.y, 2));
}

sf::Vector2f diff(sf::Vector2f A, sf::Vector2f B)
{
	return {B.x - A.x, B.y - A.y};
}

float radToDeg(float radians)
{
	
	return (radians * 180 / PI);
}

float degToRad(float degrees)
{
	return (degrees * PI) / 180;
}

std::istream& operator>>(std::istream& is, Node& obj)
{
	//Get pos
	is >> obj.pos.x >> obj.pos.y;

	//Get type
	int rawType;
	is >> rawType;
	auto type = static_cast<NodeType>(rawType);

	obj.type = type;
	return is;
}

Node* fLoadPath(std::string filepath)
{
	//This function loads a set of nodes from a given filepath
	Node* pathHead = nullptr;

	//Open file
	std::ifstream inFile;

	inFile.open(filepath);

	if (!inFile.is_open()) {
		std::cout << "Cannot open file: " << filepath << std::endl;
		return nullptr;
	}

	//Read file
	Node curr;
	Node* last = nullptr;

	while (inFile >> curr) {
		//Call implicit copy constructor
		Node* newNode = new Node(curr);
		if (last) {
			//If there is a last then link with current node and vice versa
			newNode->last = last;
			last->next = newNode;
		}
		else {
			//If there is no last, then this is the first node
			pathHead = newNode;
		}


		//Store current node as last
		last = newNode;
	}

	//Close file
	inFile.close();

	return pathHead;
}

void pathOffset(Node* head, sf::Vector2f offSet)
{
	Node* curr = head;
	while (curr) {
		//Apply offset
		curr->pos.x += offSet.x;
		curr->pos.y += offSet.y;

		//Iterate
		curr = curr->next;
	}
}


//#####################################################################################################
//			SPRITESHEET
//#####################################################################################################

void Spritesheet::slice()
{
	if (!texture) {
		std::cout << "Could not slice spritesheet: Invalid Texture..." << std::endl;
		return;
	}

	rects.clear();

	int cols = (int)(texture->getSize().x / textureSize.x);
	int rows = (int)(texture->getSize().y / textureSize.y);

	numTextures = cols * rows;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			rects.push_back({ j * textureSize.x,i * textureSize.y,textureSize.x,textureSize.y });
		}
	}
}

Spritesheet::Spritesheet()
{
}

Spritesheet::~Spritesheet()
{
	if (isLocalTexture) {
		delete texture;
	}
}

int Spritesheet::fload(std::string filepath)
{
	
	texture = new sf::Texture();

	if (!texture->loadFromFile(filepath)) {
		std::cout << "Could not load texture: " << filepath << std::endl;
		delete texture;
		return -1;
	}

	isLocalTexture = true;

	this->slice();

	
	return 0;
}

void Spritesheet::setTextureSize(sf::Vector2f newSize)
{
	textureSize = newSize;
	this->slice();
}

void Spritesheet::setTextureSize(float width, float height)
{
	textureSize = {width, height};
	this->slice();
}

void Spritesheet::setTexture(sf::Texture* newTexture)
{
	//Check for valid texture
	if (!newTexture) {
		std::cout << "Cannot set spritesheet texture: Invalid Texture..." << std::endl;
		return;
	}
	
	//Check for local texture
	if (isLocalTexture) {
		delete texture;
		isLocalTexture = false;
	}

	//Assign new texture
	texture = newTexture;
	this->slice();
}

void Spritesheet::setTexture(sf::Texture* newTexture, sf::Vector2f newSize)
{
	//Check for valid texture
	if (!newTexture) {
		std::cout << "Cannot set spritesheet texture: Invalid Texture..." << std::endl;
		return;
	}

	//Check for local texture
	if (isLocalTexture) {
		delete texture;
		isLocalTexture = false;
	}

	//Assign new texture
	texture = newTexture;
	textureSize = newSize;
	this->slice();
}

sf::IntRect Spritesheet::getRect(int idx)
{
	//Check idx validity
	if (numTextures <= idx) {
		std::cout << "Invalid spritesheet index" << std::endl;
		return {0,0,1,1};
	}
	
	
	return rects.at(idx);
}

sf::IntRect Spritesheet::getRect(int x, int y)
{
	int cols = (int)(texture->getSize().x / textureSize.x);
	int rows = (int)(texture->getSize().y / textureSize.y);

	//Check idx validity
	if (cols <= x || rows <= y) {
		std::cout << "Invalid spritesheet index" << std::endl;
		return {0,0,1,1};
	}
	
	//Flatten (x,y) and return
	return rects.at(x+(y*cols));
}
