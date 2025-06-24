#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>

/*
Project: GUI System
Created: 28 MAY 2025
Updated: 19 JUN 2025

Description:
	This file contains the GUI System. The system is made up of a collection of widets, and a Gui class to manage them.

	Widgets:
		-Panel: A 9 slice border and a background. Helps divide the window up and make widgets more visible.
		-Label: SFML Text object with a wrapper to make it a widget. Displays text.
		-Button: Clickable control to fire of certain events. The exact mechanism is not yet implemented.
		-Textbox: Text object with a background rectangle, optionally editable. For now it handles a single line of text.

	GUI:
		-List of Widgets
		-Layer System
		-Widget ID System
*/

//############################################################################################
//				WIDGETS
//############################################################################################

//#################################### BASE CLASS
class Widget {
protected:
	//Common Data
	unsigned int layer = 0;
	int ID = 0;
	bool isFocus = false;

	sf::Vector2f size = { 1,1 };
	sf::Vector2f pos = {0,0};

public:
	//Common Functions
	void setLayer(int newLayer);
	void setID(int newID);
	void setFocus(bool state = true);

	int getLayer();
	int getID();
	bool getFocus();

	//Virtual Functions
	virtual void poll(sf::RenderWindow &win, sf::Event &event) = 0;
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
	sf::Vector2f cornerSize = {1,1};
	sf::Vector2f size = {1,1};
	float borderPadding = 0;

	//unsigned int layer = 0; base class
	//int ID = 0; base class



	//Util
	void applyPos(); //Position panel components
	void applyScale(); //Scale panel components

public:
	//Constructor and Destructor
	Widget_Panel();
	~Widget_Panel();

	//Primary Functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(const float dt);
	void draw(sf::RenderWindow &win);
	
	void setTexture(sf::Texture* sheet);
	void setSize(sf::Vector2f newSize);
	void setColor(sf::Color col);
	void setPosition(sf::Vector2f newPos);
	void setBorderPadding(float padding);

	sf::Vector2f getSize();
	sf::Vector2f getPos();
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
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(float dt);
	void draw(sf::RenderWindow &win);
	void setPosition(sf::Vector2f newPos);
	void move(sf::Vector2f offset);
	
	void setText(std::string newText);
	void setFont(sf::Font* newFont);
	void setSize(sf::Vector2i size);
	void setCharacterSize(unsigned int size);
};


//###########################################	TEXT BOX
class Widget_Textbox : public Widget {
private:
	//Data
	sf::Vector2f pos = {0.f,0.f};
	sf::Vector2f size = {1.f,1.f};
	sf::Text textObject;
	std::string text = "";
	sf::Font* font;
	unsigned int charSize = 12;
	sf::Vector2f margins = { 2,2 };

	float blinkTimer = 0;
	float blinkThreshold = .75f; //seconds between blink
	bool isCursorVisible = false;
	sf::Vertex cursor[2];

	float backspaceThresholdLow = .05f;
	float backspaceAcceleration = .008f;
	float backspaceThresholdCurr = .1f;
	float backspaceThresholdHigh = .1f;
	float backspaceTimer = 0.f;

	sf::RectangleShape rectangle;

	bool editable = true;
	bool active = false;
	bool isBackspace = false;

	//Util
	void init();
	void arrangeCursor();
	void arrange();
	void backspace();

public:
	//Constructor and Destructor
	Widget_Textbox();
	~Widget_Textbox();

	//Primary functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(float dt);
	void draw(sf::RenderWindow& win);

	void setPos(sf::Vector2f newPos);

	void setText(std::string newText);
	void setFont(sf::Font* newFont);
	void setSize(sf::Vector2f newSize);
	void setCharacterSize(unsigned int size);

	std::string getText();
};


//###########################################	BUTTON
enum class ButtonState {
	PRESS, UNPRESS, HOVER
};

class Widget_Button : public Widget {
private:
	//Data
	sf::Sprite graph;
	ButtonState state = ButtonState::UNPRESS;
	ButtonState lastState = ButtonState::UNPRESS;

	//Util
	

public:
	//Constructor and Destructor
	Widget_Button();
	~Widget_Button();


	//Primary Functions
	void setState(ButtonState newState);
	void setPosition(sf::Vector2f pos);
	void move(sf::Vector2f offset);
	void setSize(sf::Vector2f size);
	void setTexture(sf::Texture* texture);
	void setTextureRect(sf::IntRect rect);


	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(const float dt);
	void draw(sf::RenderWindow& win);
	
	ButtonState getState();
	bool contains(sf::Vector2f pos);

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
	bool hasFocus = false;

	//Util


public:
	//Constructor and Destructor
	Gui();
	~Gui();

	//Primary Functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(const float dt);
	void draw(sf::RenderWindow &win);

	void addWidget(Widget* widget);
	void moveWidget(int ID, unsigned int newLayer);
	void remWidget(int ID);

	void setMaxLayers(unsigned int max);
	
	bool getFocus();
};




