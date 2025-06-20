#include "Gui.h"

//############################################################################################
//				ELEMENTS
//############################################################################################

//#################################### BASE CLASS
void Widget::setLayer(int newLayer)
{
	layer = newLayer;
}

void Widget::setID(int newID)
{
	ID = newID;
}

int Widget::getLayer()
{
	return layer;
}

int Widget::getID()
{
	return ID;
}

//#################################### PANEL
void Widget_Panel::applyPos()
{
	//Guard
	if (!spriteSheet) {
		return;
	}

	//## Vars
	float thirdX = size.x / 3.f;
	float thirdY = size.y / 3.f;

	//## Background
	background.setPosition({pos.x+borderPadding,pos.y+borderPadding});


	//## Border
	sf::Vector2f middleSize;
	middleSize.x = size.x - (2*cornerSize.x);
	middleSize.y = size.y - (2*cornerSize.y);


	//Pos
	border[0].setPosition(pos.x, pos.y);
	border[1].setPosition(pos.x + cornerSize.x, pos.y);
	border[2].setPosition(pos.x + middleSize.x + cornerSize.x, pos.y);

	border[3].setPosition(pos.x, pos.y + cornerSize.y);
	border[4].setPosition(pos.x + middleSize.x + cornerSize.x, pos.y + cornerSize.y);
	
	border[5].setPosition(pos.x, pos.y + middleSize.y + cornerSize.y);
	border[6].setPosition(pos.x + cornerSize.x, pos.y + middleSize.y + cornerSize.y);
	border[7].setPosition(pos.x + cornerSize.x + middleSize.x, pos.y + middleSize.y + cornerSize.y);
}

void Widget_Panel::applyScale()
{
	//## Background
	sf::Vector2f bgScale;
	bgScale.x = (size.x - (2 * borderPadding)) / (spriteSheet->getSize().x / 3);
	bgScale.y = (size.y - (2 * borderPadding)) / (spriteSheet->getSize().y / 3);

	background.setScale(bgScale);



	//## Border
	sf::Vector2f middleScale;
	sf::Vector2f middleSize;

	middleSize.x = size.x - (2 * cornerSize.y);
	middleSize.y = size.y - (2 * cornerSize.y);

	middleScale.x = (middleSize.x / (spriteSheet->getSize().x / 3));
	middleScale.y = (middleSize.y / (spriteSheet->getSize().y / 3));
	
	border[1].setScale(middleScale.x, 1);

	border[3].setScale(1, middleScale.y);
	border[4].setScale(1, middleScale.y);

	border[6].setScale(middleScale.x, 1);
}

Widget_Panel::Widget_Panel()
{
}

Widget_Panel::~Widget_Panel()
{
}

void Widget_Panel::poll(sf::RenderWindow& win, sf::Event& event)
{
}

void Widget_Panel::update(const float dt)
{
}

void Widget_Panel::draw(sf::RenderWindow& win)
{
	win.draw(background);
	
	for (int i = 0; i < 8; i++) {
		win.draw(border[i]);
	}

}

void Widget_Panel::setTexture(sf::Texture* sheet)
{
	spriteSheet = sheet;

	int thirdX = (spriteSheet->getSize().x / 3);
	int thirdY = (spriteSheet->getSize().y / 3);

	cornerSize = {(float)thirdX,(float)thirdY};

	background.setTexture(*spriteSheet);
	background.setTextureRect({ thirdX,thirdY,thirdX,thirdY });

	for (int i = 0; i < 8; i++) {
		border[i].setTexture(*spriteSheet);
	}

	border[0].setTextureRect({ 0,0,thirdX,thirdY });
	border[1].setTextureRect({ thirdX,0,thirdX,thirdY });
	border[2].setTextureRect({ 2 * thirdX,0,thirdX,thirdY });
	border[3].setTextureRect({ 0,thirdY,thirdX,thirdY });
	border[4].setTextureRect({ 2 * thirdX,thirdY,thirdX,thirdY });
	border[5].setTextureRect({ 0,2 * thirdY,thirdX,thirdY });
	border[6].setTextureRect({ thirdX,2 * thirdY,thirdX,thirdY });
	border[7].setTextureRect({ 2 * thirdX,2 * thirdY,thirdX,thirdY });

	this->applyPos();
	this->applyScale();
}

void Widget_Panel::setSize(sf::Vector2f newSize)
{
	size = newSize;
	this->applyPos();
	this->applyScale();
}

void Widget_Panel::setColor(sf::Color col)
{
	background.setColor(col);
}

void Widget_Panel::setPosition(sf::Vector2f newPos)
{
	pos = newPos;
	this->applyPos();
}

void Widget_Panel::setBorderPadding(float padding)
{
	borderPadding = padding;
	this->applyPos();
	this->applyScale();
}

sf::Vector2f Widget_Panel::getSize() {
	return size;
}

sf::Vector2f Widget_Panel::getPos()
{
	return pos;
}


//####################################### LABEL
Widget_Label::Widget_Label()
{
}

Widget_Label::~Widget_Label()
{
}

void Widget_Label::poll(sf::RenderWindow& win, sf::Event& event)
{
	
}

void Widget_Label::update(float dt)
{
}

void Widget_Label::draw(sf::RenderWindow& win)
{
	win.draw(label);
}

void Widget_Label::setText(std::string newText)
{
	text = newText;
	label.setString(text);
}

void Widget_Label::setFont(sf::Font* newFont)
{
	label.setFont(*newFont);
}

void Widget_Label::setSize(sf::Vector2i size)
{
	//NOT IMPLEMENTED
}

void Widget_Label::setCharacterSize(unsigned int size)
{
	label.setCharacterSize(size);
}


//####################################### TEXT BOX

void Widget_Textbox::init()
{
	//Rectangle
	rectangle.setFillColor(sf::Color::White);
	rectangle.setOutlineColor(sf::Color::Black);
	rectangle.setOutlineThickness(1);
	rectangle.setSize({ charSize + (2 * margins.x), charSize + (2 * margins.y) });

	//Text
	textObject.setCharacterSize(charSize);
	textObject.setFillColor(sf::Color::Black);

	//Cursor
	cursor[0].color = sf::Color::Black;
	cursor[1].color = sf::Color::Black;
}

void Widget_Textbox::arrangeCursor()
{
	sf::Vector2f cursorPos = textObject.getPosition();
	cursorPos.x += textObject.getGlobalBounds().width + 2;
	cursorPos.y += (.25 * textObject.getGlobalBounds().height);

	cursor[0].position = cursorPos;
	cursorPos.y += textObject.getGlobalBounds().height + (.5 * textObject.getGlobalBounds().height);
	cursor[1].position = cursorPos;

}

void Widget_Textbox::arrange()
{

	//## Positioning
	//Rectangle
	rectangle.setPosition(pos);

	//Text
	sf::Vector2f textPos = pos;
	textPos.x += margins.x;
	//textPos.y += margins.y;
	textObject.setPosition(textPos);

	//Cursor
	this->arrangeCursor();
	
}

Widget_Textbox::Widget_Textbox()
{
	this->init();
}

Widget_Textbox::~Widget_Textbox()
{
}


void Widget_Textbox::poll(sf::RenderWindow& win, sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed) {
		sf::Vector2i pixelPos = sf::Mouse::getPosition(win);
		sf::Vector2f fPos = {(float)pixelPos.x, (float)pixelPos.y};

		if (editable && rectangle.getGlobalBounds().contains(fPos)) {
			active = true;
			
		}
		else {
			active = false;
		}
	}
	
	//Typing
	if (active && event.type == sf::Event::TextEntered) {
		if (event.text.unicode < 0x80) // it's printable
		{
			char key = (char)event.text.unicode;
			textObject.setString(textObject.getString() + key);
			this->arrangeCursor();
		}
	}
}

void Widget_Textbox::update(float dt)
{
	//Cursor Blink
	blinkTimer += dt;
	if (blinkTimer >= blinkThreshold) {
		isCursorVisible = !isCursorVisible;
		blinkTimer = 0;
	}
}

void Widget_Textbox::draw(sf::RenderWindow& win)
{
	//Box
	win.draw(rectangle);

	//Text
	win.draw(textObject);

	//Cursor
	if (active && isCursorVisible) {
		win.draw(cursor, 2, sf::Lines);
	}
}

void Widget_Textbox::setPos(sf::Vector2f newPos)
{
	pos = newPos;
	this->arrange();
}

void Widget_Textbox::setText(std::string newText)
{
	textObject.setString(newText);
	this->arrange();
}

void Widget_Textbox::setFont(sf::Font* newFont)
{
	if (!newFont) {
		std::cout << "Cannot set font: Invalid Font" << std::endl;
		return;
	}

	font = newFont;

	textObject.setFont(*font);
	this->arrange();
}

void Widget_Textbox::setSize(sf::Vector2f newSize)
{
	size = newSize;

	rectangle.setSize(newSize);

	this->arrange();
}

void Widget_Textbox::setCharacterSize(unsigned int size)
{
	charSize = size;
	textObject.setCharacterSize(charSize);
	this->arrange();
}


//############################################################################################
//				GUI
//############################################################################################

Gui::Gui()
{
}

Gui::~Gui()
{
	//De allocate widget list
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			delete widgets.at(i)->at(j);
		}

		delete widgets.at(i);
	}
}

void Gui::poll(sf::RenderWindow& win, sf::Event& event)
{
	//Poll widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			widgets.at(i)->at(j)->poll(win, event);
		}
	}
}

void Gui::update(const float dt)
{
	//Update widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			widgets.at(i)->at(j)->update(dt);
		}
	}
}

void Gui::draw(sf::RenderWindow& win)
{
	//Draw widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			widgets.at(i)->at(j)->draw(win);
		}
	}
}

void Gui::addWidget(Widget* widget)
{
	if (!widget) {
		return;
	}

	while (widget->getLayer() >= numLayers) {
		std::vector<Widget*>* row = new std::vector<Widget*>;

		widgets.push_back(row);

		numLayers++;
	}

	//Add to widget vector
	widgets.at(widget->getLayer())->push_back(widget);

	//Assign ID
	if (!removedIDs.empty()) {
		widget->setID(removedIDs.front());
		removedIDs.pop();
		return;
	}

	//If no unused ids then assign a new one
	widget->setID(IDCounter++);
}

void Gui::moveWidget(int ID, unsigned int newLayer)
{
	//Validate layer
	if (newLayer >= maxLayers) {
		std::cout << "Cannot move widget: " << ID << ", invalid layer" << std::endl;
		return;
	}

	//Update widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			if (widgets.at(i)->at(j)->getID() == ID) {

				//Copy widget to new layer
				widgets.at(i)->at(j)->setLayer(newLayer);
				addWidget(widgets.at(i)->at(j));

				//Remove widget from old layer
				auto it = std::next(widgets.at(i)->begin(), j);
				widgets.at(i)->erase(it); 

				return;
			}
		}
	}

	//Not found
	std::cout << "Could not move widget: " << ID << ", ID not found" << std::endl;
	return;
}

void Gui::remWidget(int ID)
{
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			if (widgets.at(i)->at(j)->getID() == ID) {
				//Remove widget from  layer
				removedIDs.push(widgets.at(i)->at(j)->getID());
				auto it = std::next(widgets.at(i)->begin(), j);
				delete widgets.at(i)->at(j);
				widgets.at(i)->erase(it);
				return;
			}
		}
	}

	//Not found
	std::cout << "Could not remove widget: " << ID << ", ID not found" << std::endl;
	return;

}

void Gui::setMaxLayers(unsigned int max)
{
	maxLayers = max;
}