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
