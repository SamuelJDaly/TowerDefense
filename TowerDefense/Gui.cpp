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
void Widget_Panel::arrange()
{
	//Guard
	if (!spriteSheet) {
		return;
	}

	//## Vars
	float scaleX, scaleY;
	float thirdX = size.x / 3;
	float thirdY = size.y / 3;
	int row = 0; int col = 0;

	//## Background
	scaleX = size.x * 3 / spriteSheet->getSize().x;
	scaleY = size.y * 3 / spriteSheet->getSize().y;

	background.setPosition(pos);
	background.setScale(scaleX, scaleY);


	//## Border
	scaleX = (size.x) / spriteSheet->getSize().x;
	scaleY = (size.y) / spriteSheet->getSize().y;

	//Pos
	border[0].setPosition(pos.x, pos.y);
	border[1].setPosition(pos.x + thirdX, pos.y);
	border[2].setPosition(pos.x + (2 * thirdX), pos.y);
	border[3].setPosition(pos.x, pos.y + thirdY);
	border[4].setPosition(pos.x + (2 * thirdX), pos.y + thirdY);
	border[5].setPosition(pos.x, pos.y + (2 * thirdY));
	border[6].setPosition(pos.x + thirdX, pos.y + (2 * thirdY));
	border[7].setPosition(pos.x + (2 * thirdX), pos.y + (2 * thirdY));

	//Scale
	border[0].setScale(scaleX, scaleY);
	border[1].setScale(scaleX, scaleY);
	border[2].setScale(scaleX, scaleY);
	border[3].setScale(scaleX, scaleY);
	border[4].setScale(scaleX, scaleY);
	border[5].setScale(scaleX, scaleY);
	border[6].setScale(scaleX, scaleY);
	border[7].setScale(scaleX, scaleY);

}

Widget_Panel::Widget_Panel()
{
}

Widget_Panel::~Widget_Panel()
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

	int thirdX = spriteSheet->getSize().x / 3;
	int thirdY = spriteSheet->getSize().y / 3;

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

	this->arrange();
}

void Widget_Panel::setSize(sf::Vector2f newSize)
{
	size = newSize;
	this->arrange();
}

void Widget_Panel::setColor(sf::Color col)
{
	background.setColor(col);
}

void Widget_Panel::setPosition(sf::Vector2f newPos)
{
	pos = newPos;
	this->arrange();
}


//####################################### LABEL
Widget_Label::Widget_Label()
{
}

Widget_Label::~Widget_Label()
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


