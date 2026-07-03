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
	//label_title->setTextColor(sf::Color(255, 100, 100, 255));
	label_title->setHighlightColor(sf::Color::Yellow);
	label_title->setText("Generic Tower Defense");
	label_title->setCharacterSize(54);
	label_title->setPosition({ std::floor(centerX), 100 });
	label_title->setLayer(3);

	//Game button
	btn_game = new Widget_Button();
	btn_game->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_game->setType(en_SliceType::NINE);
	btn_game->setSize({ 300,50 });
	btn_game->setPosition({ centerX - 150,centerY - 100 });

	btn_game->setLabelFont(&font_second);
	btn_game->setLabelText("Start Game");
	btn_game->setLabelCharSize(20);
	btn_game->setLabelTextColor(sf::Color::Black);

	//Editor button
	btn_editor = new Widget_Button();
	btn_editor->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_editor->setType(en_SliceType::NINE);
	btn_editor->setSize({ 300,50 });
	btn_editor->setPosition({ centerX - 150,centerY });

	btn_editor->setLabelFont(&font_second);
	btn_editor->setLabelText("Level Editor");
	btn_editor->setLabelCharSize(20);
	btn_editor->setLabelTextColor(sf::Color::Black);

	//Quit button
	btn_quit = new Widget_Button();
	btn_quit->setTexture(textureHandler->lookup("btn_blank_sq_ol"));
	btn_quit->setType(en_SliceType::NINE);
	btn_quit->setSize({ 300,50 });
	btn_quit->setPosition({ centerX - 150,centerY + 100 });

	btn_quit->setLabelFont(&font_second);
	btn_quit->setLabelText("Quit");
	btn_quit->setLabelCharSize(20);
	btn_quit->setLabelTextColor(sf::Color::Black);

	//Version Label
	Widget_Label* label_version = new Widget_Label();
	label_version->setFont(&font_second);
	label_version->setCharacterSize(10);
	label_version->setTextColor(sf::Color::Black);
	label_version->setText("Version: " + VERSION_GAME);

	int x = window->getSize().x - (int)label_version->getGlobalBounds().size.x - 10;
	int y = window->getSize().y - (int)label_version->getGlobalBounds().size.y - 10;
	label_version->setPosition({(float)x,(float)y});

	//Add components to gui
	gui->addWidget(label_title);
	gui->addWidget(btn_game);
	gui->addWidget(btn_editor);
	gui->addWidget(btn_quit);
	gui->addWidget(label_version);
}

State_Menu::State_Menu(TextureHandler* textureHandler, sf::RenderWindow* window)
{
	this->textureHandler = textureHandler;
	this->window = window;
	if (!font_generic.openFromFile("resource/font/jmhtype.ttf")) {
		std::cerr << "Could not load menu font" << std::endl;
	}
	if (!font_second.openFromFile("resource/font/roboto_regular.ttf")) {
		std::cerr << "Could not load menu font" << std::endl;
	}
	this->initGui();

	background.setTexture(*textureHandler->lookup("bg_menu"));
	background = sf::Sprite(*textureHandler->lookup("bg_menu"));
	float scaleX = (float)window->getSize().x / (float)background.getTexture().getSize().x;
	float scaleY = (float)window->getSize().y / (float)background.getTexture().getSize().y;
	background.setScale({ scaleX, scaleY });
}

State_Menu::~State_Menu()
{
	delete gui;
}

void State_Menu::poll(sf::RenderWindow& win, std::optional<sf::Event> event)
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
	
	sf::Vertex line1[2];
	sf::Vertex line2[2];
	
	float centerX = win.getSize().x / 2;

	line1[0].position = { centerX - 150, 0};
	line1[1].position = { centerX - 150, (float)win.getSize().y};
	line1[0].color = {255,0,0,255};
	line1[1].color = { 255,0,0,255 };

	line2[0].position = { centerX + 150, 0 };
	line2[1].position = { centerX + 150, (float)win.getSize().y };
	line2[0].color = { 255,0,0,255 };
	line2[1].color = { 255,0,0,255 };
	

	win.draw(background);
	gui->draw(win);
	//win.draw(line1, 2, sf::Lines);
	//win.draw(line2, 2, sf::Lines);
}