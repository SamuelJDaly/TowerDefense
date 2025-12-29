#include "State_Menu.h"

//##############################################################################################################
//				STATE: MENU
////##############################################################################################################

void State_Menu::initGui()
{
	gui = new Gui();

	float centerX = window->getSize().x / 2;
	float centerY = window->getSize().y / 2;

	//Title
	label_title = new Widget_Label();
	label_title->setFont(&font_generic);
	label_title->setText("Generic Tower Defense");
	label_title->setCharacterSize(54);
	label_title->setPosition({ std::floor(centerX), 100 });
	label_title->setLayer(3);

	//Game button
	btn_game = new Widget_Button();
	btn_game->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_game->setSize({ 300,50 });
	btn_game->setPosition({ centerX - 150,centerY - 100 });

	//Editor button
	btn_editor = new Widget_Button();
	btn_editor->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_editor->setSize({ 300,50 });
	btn_editor->setPosition({ centerX - 150,centerY });

	//Quit button
	btn_quit = new Widget_Button();
	btn_quit->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_quit->setSize({ 300,50 });
	btn_quit->setPosition({ centerX - 150,centerY + 100 });



	//Add components to gui
	gui->addWidget(label_title);
	gui->addWidget(btn_game);
	gui->addWidget(btn_editor);
	gui->addWidget(btn_quit);
}

State_Menu::State_Menu(TextureHandler* textureHandler, sf::RenderWindow* window)
{
	this->textureHandler = textureHandler;
	this->window = window;
	if (!font_generic.loadFromFile("resource/font/jmhtype.ttf")) {
		std::cerr << "Could not load menu font" << std::endl;
	}
	this->initGui();

	background.setTexture(*textureHandler->lookup("bg_menu"));
	float scaleX = (float)window->getSize().x / (float)background.getTexture()->getSize().x;
	float scaleY = (float)window->getSize().y / (float)background.getTexture()->getSize().y;
	background.setScale(scaleX, scaleY);
}

State_Menu::~State_Menu()
{
	delete gui;
}

void State_Menu::poll(sf::RenderWindow& win, sf::Event& event)
{
	gui->poll(win, event);

	if (btn_game->getState() == ButtonState::PRESS) {
		nextState = En_Gamestate::GAME;
		isFinished = true;
	}

	if (btn_editor->getState() == ButtonState::PRESS) {
		nextState = En_Gamestate::EDITOR;
		isFinished = true;
	}

	if (btn_quit->getState() == ButtonState::PRESS) {
		nextState = En_Gamestate::END;
		isFinished = true;
	}
}

void State_Menu::update(float dt)
{
	gui->update(dt);

}

void State_Menu::draw(sf::RenderWindow& win)
{
	win.draw(background);
	gui->draw(win);
}