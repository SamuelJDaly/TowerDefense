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

void printRectI(sf::IntRect targ)
{
	std::cout << "{" << targ.left << ", " << targ.top << ", " << targ.width << ", " << targ.height << "}" << std::endl;
}

void printRectF(sf::FloatRect targ)
{
	std::cout << "{" << targ.left << ", " << targ.top << ", " << targ.width << ", " << targ.height << "}" << std::endl;
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
	if (!sheet) {
		std::cout << "Could not slice spritesheet: Invalid Texture..." << std::endl;
		return;
	}

	rects.clear();

	std::cout << sheet->getSize().x << ", " << sheet->getSize().y << std::endl;

	int cols = (int)std::ceilf((float)sheet->getSize().x / (float)textureSize.x);
	int rows = (int)std::ceilf((float)sheet->getSize().y / (float)textureSize.y);

	numTextures = cols * rows;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			rects.push_back({ j * textureSize.x,i * textureSize.y,textureSize.x,textureSize.y });
			std::cout << j * textureSize.x << "," << i * textureSize.y << "," << textureSize.x << "," << textureSize.y << std::endl;
		}
	}
}

Spritesheet::Spritesheet()
{
}

Spritesheet::~Spritesheet()
{
	if (isLocalTexture) {
		delete sheet;
	}
}

int Spritesheet::fload(std::string filepath)
{
	
	sheet = new sf::Texture();

	if (!sheet->loadFromFile(filepath)) {
		std::cout << "Could not load texture: " << filepath << std::endl;
		delete sheet;
		return -1;
	}

	isLocalTexture = true;

	this->slice();

	
	return 0;
}

void Spritesheet::setTextureSize(sf::Vector2i newSize)
{
	textureSize = newSize;
	if (sheet) {
		this->slice();
	}
}

void Spritesheet::setTextureSize(int width, int height)
{
	textureSize = {width, height};
	if (sheet) {
		this->slice();
	}
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
		delete sheet;
		isLocalTexture = false;
	}

	//Assign new texture
	sheet = newTexture;
	this->slice();
}

void Spritesheet::setTexture(sf::Texture* newTexture, sf::Vector2i newSize)
{
	//Check for valid texture
	if (!newTexture) {
		std::cout << "Cannot set spritesheet texture: Invalid Texture..." << std::endl;
		return;
	}

	//Check for local texture
	if (isLocalTexture) {
		delete sheet;
		isLocalTexture = false;
	}

	//Assign new texture
	sheet = newTexture;
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
	int cols = (int)(sheet->getSize().x / textureSize.x);
	int rows = (int)(sheet->getSize().y / textureSize.y);

	//Check idx validity
	if (cols <= x || rows <= y) {
		std::cout << "Invalid spritesheet index" << std::endl;
		return {0,0,1,1};
	}
	
	//Flatten (x,y) and return
	return rects.at(x+(y*cols));
}

std::vector<sf::IntRect> Spritesheet::getRects()
{
	return rects;
}

sf::Texture* Spritesheet::getTexture()
{
	return sheet;
}

int Spritesheet::getNumTextures()
{
	return numTextures;
}
