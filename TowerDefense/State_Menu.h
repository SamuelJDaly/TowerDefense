#pragma once
#include "Gamestate.h"

class State_Menu : public Gamestate {
private:
	//## Data
	Gui* gui;
	sf::Font font_generic;

	sf::Sprite background;

	//Text
	Widget_Label* label_title;

	//Buttons
	Widget_Button* btn_game;
	Widget_Button* btn_editor;
	Widget_Button* btn_quit;

	//## Util
	void initGui();

public:
	//Constructor and Destructor
	State_Menu(TextureHandler* textureHandler, sf::RenderWindow* window);
	~State_Menu();

	//Primary Functions
	void update(float dt);
	void poll(sf::RenderWindow& win, sf::Event& event);
	void draw(sf::RenderWindow& win);

};

