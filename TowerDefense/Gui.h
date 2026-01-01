#pragma once
#include <iostream>
#include <vector>
#include <queue>
#include <SFML/Graphics.hpp>

/*
Project: GUI System
Created: 28 MAY 2025
Updated: 28 NOV 2025

Description:
	This file contains the GUI System. The system is made up of a collection of widets, and a Gui class to manage them.

	Widgets:
		-Panel: A 9 slice border and a background. Helps divide the window up and make widgets more visible.
		-Tabbed Panel: A panel with tabs at the top. For now it just tracks which tab is selected and the resultant behavior is external.
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
	sf::View* view = nullptr;

	sf::Vector2f size = { 32,32};
	sf::Vector2f pos = {0,0};

public:
	//Common Functions
	void setLayer(int newLayer);
	void setID(int newID);
	void setFocus(bool state = true);
	void setView(sf::View* newView);

	int getLayer();
	int getID();
	bool getFocus();
	sf::Vector2f getPos();

	//Virtual Functions
	virtual void poll(sf::RenderWindow &win, sf::Event &event) = 0;
	virtual void update(const float dt) = 0;
	virtual void draw(sf::RenderWindow& win) = 0;
};


//Predeclarations
class Widget_Label;

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
};

//####################################### TABBED PANEL
class Widget_TabbedPanel : public Widget {
private:
	//Data
	uint8_t tabSel = 0;
	uint8_t numTabs = 0;
	uint8_t maxTabs = 10; //Arbitrary, might want to find a better system

	float tabX = .2;
	float tabY = .1;

	Widget_Panel panel;
	std::vector<sf::Sprite> tabs;
	std::vector<Widget_Label> tabTitles;

	sf::Font* font;
	sf::Texture* panelTexture = nullptr;
	sf::Texture* tabTexture = nullptr;

	//Util
	void arrange();

public:
	//Constructor and Destructor
	Widget_TabbedPanel();
	~Widget_TabbedPanel();

	//Primary Functions
	void setTabTexture(sf::Texture* texture);
	void setPanelTexture(sf::Texture* texture);
	void setPos(sf::Vector2f newPos);
	void setSize(sf::Vector2f newSize);

	void addTab(uint8_t count = 1);
	void remTab(uint8_t count = 1);
	void setTabFont(sf::Font* newFont);
	void setTabTitle(int idx, std::string title);
	void setTabTitleColor(sf::Color color);

	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(const float dt);
	void draw(sf::RenderWindow& win);

	uint8_t getTabSel();
};

//####################################### SCROLLABLE PANEL
class Widget_ScrollPanel : public Widget {
private:
	//Data
	sf::View scrollview;
	float scrollOffset = 0;
	float maxOffset = 1;
	float minOffset = 0;
	Widget_Panel panel;
	sf::Sprite scrollBar;



	//Util
public:
	//Constructor and Destructor
	Widget_ScrollPanel();
	~Widget_ScrollPanel();

	//Primary Functions
	void poll(sf::RenderWindow& win, sf::Event& event);
	void update(const float dt);
	void draw(sf::RenderWindow& win);
};

//####################################### LABEL
class Widget_Label : public Widget {
private:
	//Data
	sf::Text label;
	std::string text = "";
	sf::Font* font;

	sf::Color col_base = sf::Color(255, 255, 255, 255); //Base color
	float highlightModifier = 0.2; // default 30% highlight
	sf::Color col_actual = sf::Color(204, 204, 204, 255); //UnHighlit color
	bool isHighlight = false;

	//Util
	void centerOrigin();

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
	void setOrigin(sf::Vector2f newOrigin);

	sf::FloatRect getGlobalBounds();
	sf::FloatRect getLocalBounds();
	
	void setText(std::string newText);
	void setFont(sf::Font* newFont);
	void setSize(sf::Vector2i size);
	void setCharacterSize(unsigned int size);
	void setTextColor(sf::Color color);

	void setHighlightModifier(float value);
	void toggleHighlight();
	void setHighlightState(bool state);
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
	sf::Vector2i selection = {0,0};

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
	void move(sf::Vector2f offset);

	void setText(std::string newText);
	void setFont(sf::Font* newFont);
	void setSize(sf::Vector2f newSize);
	void setCharacterSize(unsigned int size);

	std::string getText();
};


//###########################################	BUTTON
enum class ButtonState {
	UNPRESS, HOVER, PRESS
};

enum class en_SliceType {
	NONE,THREE,FOUR,NINE
};

class Widget_Button : public Widget {
private:
	//Data
	ButtonState state = ButtonState::UNPRESS;
	ButtonState lastState = ButtonState::UNPRESS;
	sf::Vector2i textureSize = { 1,1 };
	sf::Texture* texture;

	sf::FloatRect bounds = {0,0,1,1};

	en_SliceType sliceType = en_SliceType::THREE;

	std::vector<sf::Sprite*> slices;

	//Util
	void align();
	void updateTexture();

public:
	//Constructor and Destructor
	Widget_Button();
	~Widget_Button();


	//Primary Functions
	void setState(ButtonState newState);
	void setPosition(sf::Vector2f newPos);
	void move(sf::Vector2f offset);
	void setSize(sf::Vector2f newSize);
	void setTexture(sf::Texture* newTexture);

	void setType(en_SliceType newType);

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
	sf::View* view = nullptr;

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

	void setView(sf::View* newView);
	void setMaxLayers(unsigned int max);
	
	bool getFocus();
};




