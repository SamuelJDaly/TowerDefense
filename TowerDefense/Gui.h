#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>



//############################################################################################
//				ELEMENTS
//############################################################################################

//#################################### BASE CLASS
class Widget {
protected:
	//Common Data
	unsigned int layer = 0;
	int ID = 0;

	sf::Vector2f size = { 1,1 };
	sf::Vector2f pos = {0,0};

public:
	//Common Functions
	void setLayer(int newLayer);
	void setID(int newID);
	

	int getLayer();
	int getID();

	//Virtual Functions
	virtual void update(const float dt) = 0;
	virtual void draw(sf::RenderWindow& win) = 0;
};




//####################################### PANEL
class Widget_Panel : public Widget {
private:
	//Data

	sf::Sprite background;
	sf::Sprite border[8];
	sf::Texture* spriteSheet; //Use a spritesheet to avoid needing to set 9 textures


	//unsigned int layer = 0; base class
	//int ID = 0; base class



	//Util
	void arrange(); //Position and scale individual sprites

public:
	//Constructor and Destructor
	Widget_Panel();
	~Widget_Panel();

	//Primary Functions
	void update(const float dt);
	void draw(sf::RenderWindow &win);
	
	void setTexture(sf::Texture* sheet);
	void setSize(sf::Vector2f newSize);
	void setColor(sf::Color col);
	void setPosition(sf::Vector2f newPos);
};


//####################################### LABEL
class Widget_Label : public Widget {
private:
	//Data
	sf::Text label;
	std::string text = "";
	sf::Font* font;

public:
	//Constructor and Destructor
	Widget_Label();
	~Widget_Label();

	//Primary Functions
	void update(float dt);
	void draw(sf::RenderWindow &win);
	
	void setText(std::string newText);
	void setFont(sf::Font* newFont);
	void setSize(sf::Vector2i size);
	void setCharacterSize(unsigned int size);
};




//############################################################################################
//				GUI
//############################################################################################
class Gui {
private:
	//Data
	std::vector<std::vector<Widget*>*> widgets; //array is 2d for layer control
	unsigned int maxLayers = 32;
	int numLayers = 0;
	int IDCounter = 0;
	std::queue<int> removedIDs;

	//Util


public:
	//Constructor and Destructor
	Gui();
	~Gui();

	//Primary Functions
	void update(const float dt);
	void draw(sf::RenderWindow &win);

	void addWidget(Widget* widget);
	void moveWidget(int ID, unsigned int newLayer);
	void remWidget(int ID);

	void setMaxLayers(unsigned int max);
};




