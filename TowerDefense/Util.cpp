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
	std::cout << "{" << targ.position.x << ", " << targ.position.y << ", " << targ.size.x << ", " << targ.size.y << "}" << std::endl;
}

void printRectF(sf::FloatRect targ)
{
	std::cout << "{" << targ.position.x << ", " << targ.position.y << ", " << targ.size.x << ", " << targ.size.y << "}" << std::endl;
}

sf::Vector2f round(sf::Vector2f targ)
{
	return {std::round(targ.x), std::round(targ.y)};
}

std::string strFilter(std::string target, std::string phrase)
{
	size_t pos = target.find(phrase);
	return target.erase(pos, phrase.length());
}

std::string trimFileExt(std::string target)
{
	size_t pos = target.find(".");
	size_t len = target.length() - pos;

	return target.erase(pos, len);
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

std::ostream& operator<<(std::ostream& os, Node& obj)
{
	//Output pos
	os << obj.pos.x << " " << obj.pos.y << " ";

	//Output type as int
	os << static_cast<int>(obj.type);
	
	return os;
}

Node* fReadPath(std::string filepath)
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

void fWritePath(std::string filepath, Node* pathHead)
{
	//Check for valid path
	if (!pathHead) {
		return;
	}

	//Open output file
	std::ofstream outFile;
	outFile.open(filepath);

	if (!outFile.is_open()) {
		std::cout << "Could not open file: " << filepath << std::endl;
		return;
	}

	Node* curr = pathHead;

	while (curr) {
		outFile << *curr << "\n";
		//Iterate
		curr = curr->next;
	}

	//Close output file
	outFile.close();

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

void drawPath(Node* pathHead, sf::RenderWindow& win)
{
	if (!pathHead) {
		return;
	}

	sf::CircleShape point;
	point.setRadius(2);
	point.setOrigin({ point.getRadius(), point.getRadius() });

	Node* curr = pathHead;
	Node* last = nullptr;

	sf::Color currCol = sf::Color::Blue;
	sf::Color lastCol = sf::Color::Blue;

	sf::Vertex line[2];

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
			win.draw(line, 2, sf::PrimitiveType::Lines);
		}

		//Get next node
		last = curr;
		lastCol = currCol;
		curr = curr->next;
	}
}

void Path_PushBack(Node* pathHead, sf::Vector2f pos)
{
	//Create new node
	Node* newNode = new Node;
	newNode->pos = pos;
	newNode->type = NodeType::END;

	//Check for empty list case
	if (!pathHead) {
		pathHead = newNode;
		return;
	}

	//Traverse to end of list
	Node* curr = pathHead;

	while (curr->next) {
		curr = curr->next;
	}

	curr->type = NodeType::PATH;
	curr->next = newNode;
	newNode->last = curr;
	return;
}

void Path_PopBack(Node* pathHead, int idx)
{
	//Check for empty list
	if (!pathHead) {
		std::cout << "Cannot pop empty list..." << std::endl;
		return;
	}

	//Traverse list
	Node* curr = pathHead;

	while (curr->next) {
		curr = curr->next;
	}

	//Update previous node if it exists
	if (curr->last) {
		curr->last->next = nullptr;
		curr->last->type = NodeType::END;
	}
	
	//Free data of end node
	delete curr;

	return;
}

void Path_Insert(Node* pathHead, int idx, sf::Vector2f pos)
{
	//# Create new node
	Node* newNode = new Node;
	newNode->pos = pos;
	newNode->type = NodeType::BEGIN;

	//# Empty case
	if (!pathHead) {
		pathHead = newNode;
		return;
	}

	//# Beginning case
	if (idx == 0) {
		Node* temp = pathHead;
		pathHead = newNode;
		pathHead->next = temp;
		temp->last = pathHead;
		

		if (temp->next) {
			temp->type = NodeType::PATH;
		}
		else {
			temp->type = NodeType::END;
		}
		return;
	}

	//# Traverse to idx
	Node* curr = pathHead;
	for (int i = 0; i < idx; i++) {
		if (!curr) {
			std::cout << "Cannot insert path node at index: " << idx << " out of bounds" << std::endl;
			delete newNode;
			return;
		}
		curr = curr->next;
	}

	//# Curr is now the object that needs to shift right
	curr->last->next = newNode;
	newNode->next = curr;
	curr->last = newNode;
	newNode->type = NodeType::PATH;


	return;
}

void Path_Remove(Node* pathHead, int idx)
{
	//# Empty Case
	if (!pathHead) {
		std::cout << "Cannot remove node from empty path" << std::endl;
	}

	//# Beginning Case
	if (idx == 0) {

	}

	//# Traverse to idx
	Node* curr = pathHead;
	for (int i = 0; i < idx; i++) {
		if (!curr) {
			std::cout << "Cannot remove path node at index: " << idx << " out of bounds" << std::endl;
			return;
		}
		curr = curr->next;
	}

	//# Curr is now the node that needs to be removed


}


void freePath(Node* pathHead)
{
	if (!pathHead) {
		return;
	}

	
	Node* curr = pathHead;
	Node* next;

	while (curr) {
		next = curr->next;
		delete curr;
		curr = next;
	}


	return;
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

	//std::cout << sheet->getSize().x << ", " << sheet->getSize().y << std::endl;

	int cols = (int)std::ceilf((float)sheet->getSize().x / (float)textureSize.x);
	int rows = (int)std::ceilf((float)sheet->getSize().y / (float)textureSize.y);

	numTextures = cols * rows;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			rects.push_back({ {j * textureSize.x,i * textureSize.y},{textureSize.x,textureSize.y} });
			//std::cout << j * textureSize.x << "," << i * textureSize.y << "," << textureSize.x << "," << textureSize.y << std::endl;
		}
	}
}

Spritesheet::Spritesheet()
{

}

Spritesheet::~Spritesheet()
{
	
}

int Spritesheet::fload(std::string filepath)
{
	
	sheet = new sf::Texture();

	if (!sheet->loadFromFile(filepath)) {
		std::cout << "Could not load texture: " << filepath << std::endl;
		delete sheet;
		return -1;
	}

	texturePath = filepath;

	this->slice();

	
	return 1;
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
		return { {0,0},{1,1} };
	}
	
	//std::cout << "getting rect at: " << idx << std::endl;;
	return rects.at(idx);
}

sf::IntRect Spritesheet::getRect(int x, int y)
{
	int cols = (int)(sheet->getSize().x / textureSize.x);
	int rows = (int)(sheet->getSize().y / textureSize.y);

	//Check idx validity
	if (cols <= x || rows <= y) {
		std::cout << "Invalid spritesheet index" << std::endl;
		return { {0,0},{1,1} };
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
	if (!sheet) {
		return &blankTexture;
	}

	return sheet;
}

int Spritesheet::getNumTextures()
{
	return numTextures;
}

sf::Vector2i Spritesheet::getTextureSize()
{
	return textureSize;
}

std::string Spritesheet::getTexturePath()
{
	return texturePath;
}

bool Spritesheet::containsIdx(int idx)
{
	if (idx < numTextures && idx >= 0) {
		return true;
	}

	return false;
}
