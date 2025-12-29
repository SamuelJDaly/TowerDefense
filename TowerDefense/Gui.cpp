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

void Widget::setFocus(bool state)
{
	isFocus = state;
}

void Widget::setView(sf::View* newView)
{
	view = newView;
}

int Widget::getLayer()
{
	return layer;
}

int Widget::getID()
{
	return ID;
}

bool Widget::getFocus()
{
	return isFocus;
}

sf::Vector2f Widget::getPos() {
	return pos;
}

//#################################### PANEL
#pragma region Panel

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
	sf::Vector2f bgScale = {1,1};
	if (spriteSheet) {
		bgScale.x = (size.x - (2 * borderPadding)) / (spriteSheet->getSize().x / 3);
		bgScale.y = (size.y - (2 * borderPadding)) / (spriteSheet->getSize().y / 3);
	}

	background.setScale(bgScale);



	//## Border
	sf::Vector2f middleScale = {1,1};
	sf::Vector2f middleSize;

	middleSize.x = size.x - (2 * cornerSize.y);
	middleSize.y = size.y - (2 * cornerSize.y);

	if (spriteSheet) {
		middleScale.x = (middleSize.x / (spriteSheet->getSize().x / 3));
		middleScale.y = (middleSize.y / (spriteSheet->getSize().y / 3));
	}
	
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

#pragma endregion



//####################################### TABBED PANEL
void Widget_TabbedPanel::arrange() {
	float tabSizeX = tabX * size.x;
	float tabSizeY = tabY * size.y;
	float scaleX = 1;
	float scaleY = 1;

	if (tabTexture) {
		scaleX = tabSizeX / tabTexture->getSize().x;
		scaleY = tabSizeY / tabTexture->getSize().y;
	}

	//Set Tab position and scale
	for (int i = 0; i < (int)tabs.size(); i++) {
		tabs.at(i).setScale(scaleX,scaleY);
		tabs.at(i).setPosition({pos.x + (i * tabSizeX), pos.y});

		//Determine label char size
		unsigned int charSize = std::round(.8 * tabSizeY);
		tabTitles.at(i).setCharacterSize(charSize);

		//Center label
		sf::Vector2f textOrigin;
		sf::Vector2f textPos;

		textOrigin.x = (tabTitles.at(i).getGlobalBounds().getSize().x / 2) + tabTitles.at(i).getLocalBounds().getPosition().x;
		textOrigin.y = (tabTitles.at(i).getGlobalBounds().getSize().y / 2) + tabTitles.at(i).getLocalBounds().getPosition().y;

		textPos.x = tabs.at(i).getPosition().x + (tabSizeX / 2);
		textPos.y = tabs.at(i).getPosition().y + (tabSizeY / 2);

		textOrigin = {std::round(textOrigin.x), std::round(textOrigin.y)}; //Round to avoid blur
		textPos = { std::round(textPos.x), std::round(textPos.y) }; //Round to avoid blur
		tabTitles.at(i).setOrigin(textOrigin);
		tabTitles.at(i).setPosition(textPos);
	}

	//Set panel scale and pos
	panel.setSize({size.x, size.y - tabSizeY});
	panel.setPosition({pos.x, pos.y + tabSizeY});
}

Widget_TabbedPanel::Widget_TabbedPanel()
{
}

Widget_TabbedPanel::~Widget_TabbedPanel()
{
}

void Widget_TabbedPanel::setTabTexture(sf::Texture* texture)
{
	tabTexture = texture;

	//Set Tab textures and scales
	for (auto t : tabs) {
		std::cout << "Texture set" << std::endl;
		t.setTexture(*texture);
	}

	this->arrange();
}

void Widget_TabbedPanel::setPanelTexture(sf::Texture* texture)
{
	panelTexture = texture;

	//Set panel texture
	panel.setTexture(texture);

	this->arrange();
}

void Widget_TabbedPanel::setPos(sf::Vector2f newPos) {
	pos = newPos;
	this->arrange();
}

void Widget_TabbedPanel::setSize(sf::Vector2f newSize) {
	size = newSize;
	this->arrange();
}

void Widget_TabbedPanel::addTab(uint8_t count)
{
	if (count + numTabs > maxTabs) {
		std::cerr << "Cannot add " << count << " tabs: over maximum..." << std::endl;
		return;
	}

	numTabs += count;
	for (int i = 0; i < count; i++) {
		sf::Sprite s;
		tabs.push_back(s);
		Widget_Label label;
		tabTitles.push_back(label);
		tabTitles.back().setText("");
		if (font) {
			tabTitles.back().setFont(font);
		}
		if (tabTexture) {
			tabs.back().setTexture(*tabTexture);
		}
	}

	this->arrange();
}

void Widget_TabbedPanel::remTab(uint8_t count)
{
	if (count > numTabs) {
		std::cerr << "Cannot remove " << count << " tabs: only " << numTabs << " tabs present..." << std::endl;
		return;
	}

	numTabs -= count;
	for (int i = 0; i < count; i++) {
		tabs.pop_back();
		tabTitles.pop_back();
	}
	
	this->arrange();
}

void Widget_TabbedPanel::setTabFont(sf::Font* newFont)
{
	font = newFont;
	for (auto l : tabTitles) {
		l.setFont(font);
	}

	this->arrange();
}

void Widget_TabbedPanel::setTabTitle(int idx, std::string title)
{
	if (idx >= numTabs) {
		std::cerr << "Cannot set tab title at index " << idx << ": out of bounds..." << std::endl;
		return;
	}

	tabTitles.at(idx).setText(title);

	this->arrange();
}

void Widget_TabbedPanel::setTabTitleColor(sf::Color color)
{
	for (auto l : tabTitles) {
		l.setTextColor(color);
	}
}

void Widget_TabbedPanel::poll(sf::RenderWindow& win, sf::Event& event) {
	//Mouse click
	if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left) {
		sf::Vector2f mousePos;
		if (view) {
			//Then use mouse coords transformed to view
			mousePos = win.mapPixelToCoords(sf::Mouse::getPosition(win), *view);
		}
		else {
			//Use raw mouse coords in window
			mousePos = { (float)sf::Mouse::getPosition(win).x, (float)sf::Mouse::getPosition(win).y };
		}

		for (int i = 0; i < (int)tabs.size(); i++) {
			if (tabs.at(i).getGlobalBounds().contains(mousePos)) {
				//TODO: Set previous selected tab appearance to inactive
				tabSel = i; //Set tab select
				//TODO: Set new selected tab apperance to active
				break;
			}
		}
	}
}


void Widget_TabbedPanel::update(const float dt) {
	//
}

void Widget_TabbedPanel::draw(sf::RenderWindow& win) {
	//Draw Panel
	panel.draw(win);

	//Draw Tabs
	for (int i = 0; i < numTabs; i++) {
		win.draw(tabs.at(i));
		tabTitles.at(i).draw(win);
	}
}

uint8_t Widget_TabbedPanel::getTabSel()
{
	return tabSel;
}


//#################################### SCROLLABLE PANEL
Widget_ScrollPanel::Widget_ScrollPanel() {

}

Widget_ScrollPanel::~Widget_ScrollPanel() {

}

void Widget_ScrollPanel::poll(sf::RenderWindow& win, sf::Event& event) {

}

void Widget_ScrollPanel::update(float dt) {
	
}

void Widget_ScrollPanel::draw(sf::RenderWindow& win) {
	
}


//####################################### LABEL
#pragma region Label

void Widget_Label::centerOrigin()
{
	float cX = std::round(label.getLocalBounds().width / 2.f);
	float cY = std::round(label.getLocalBounds().height / 2.f);

	label.setOrigin(cX, cY);
}

Widget_Label::Widget_Label()
{
	float temp = 255 * (1 - highlightModifier);
	col_actual = col_base * sf::Color(temp, temp, temp, 255);
	label.setFillColor(col_actual);
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

void Widget_Label::setPosition(sf::Vector2f newPos)
{
	label.setPosition(newPos);
}

void Widget_Label::move(sf::Vector2f offset)
{
	label.move(offset);
}

void Widget_Label::setOrigin(sf::Vector2f newOrigin)
{
	label.setOrigin(newOrigin);
}

sf::FloatRect Widget_Label::getGlobalBounds() {
	return label.getGlobalBounds();
}

sf::FloatRect Widget_Label::getLocalBounds()
{
	return label.getLocalBounds();
}

void Widget_Label::setText(std::string newText)
{
	text = newText;
	label.setString(text);
	this->centerOrigin();
}

void Widget_Label::setFont(sf::Font* newFont)
{
	label.setFont(*newFont);
	this->centerOrigin();
}

void Widget_Label::setSize(sf::Vector2i size)
{
	//NOT IMPLEMENTED
}

void Widget_Label::setCharacterSize(unsigned int size)
{
	label.setCharacterSize(size);
	this->centerOrigin();
}

void Widget_Label::setTextColor(sf::Color color)
{
	col_base = color;

	float temp = 1 - highlightModifier;
	col_actual = col_base * sf::Color(temp, temp, temp, 1);

	label.setFillColor(col_actual);
}

void Widget_Label::setHighlightModifier(float value)
{
	highlightModifier = value;
	float temp = 1 - value;
	col_actual = col_base * sf::Color(temp, temp, temp, 1);
	label.setFillColor(col_actual);
}

void Widget_Label::toggleHighlight()
{
	isHighlight = !isHighlight;

	if (isHighlight) {
		//Add highlight modifier to color (ie set to base color)
		label.setFillColor(col_base);
	}
	else {
		label.setFillColor(col_actual);
	}
}

void Widget_Label::setHighlightState(bool state)
{
	isHighlight = state;

	if (isHighlight) {
		//Add highlight modifier to color (ie set to base color)
		label.setFillColor(col_base);
	}
	else {
		label.setFillColor(col_actual);
	}
}

#pragma endregion

//####################################### TEXT BOX
#pragma region Text Box
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
	textPos.y = rectangle.getPosition().y + (.5 * textObject.getGlobalBounds().height);
	//textPos.y += margins.y;
	textObject.setPosition(textPos);

	//Cursor
	this->arrangeCursor();
	
}

void Widget_Textbox::backspace()
{
	if (text == "") {
		return;
	}
	//For now have it delete the last character of the text
	text.pop_back();
	textObject.setString(text);
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
			isFocus = true;
			
		}
		else {
			active = false;
			isFocus = false;
		}
	}
	
	//Typing
	if (active && event.type == sf::Event::TextEntered) {
		if (event.text.unicode < 0x80 && event.text.unicode > 0x19) // it's printable
		{
			char key = (char)event.text.unicode;
			text += key;
			textObject.setString(text);
			this->arrangeCursor();
		}
	}


	//Key Press
	if (active && event.type == sf::Event::KeyPressed) {
		if (event.key.code == sf::Keyboard::BackSpace) {
			isBackspace = true;
			backspaceTimer = backspaceThresholdHigh;
		}
	}

	//Key Release
	if (active && event.type == sf::Event::KeyReleased) {
		if (event.key.code == sf::Keyboard::BackSpace) {
			isBackspace = false;
			backspaceThresholdCurr = backspaceThresholdHigh;
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

	//Backspace
	backspaceTimer += dt;
	if (backspaceTimer >= backspaceThresholdCurr) {
		if (isBackspace) {
			backspace();
			if (backspaceThresholdCurr >= backspaceThresholdLow) {
				backspaceThresholdCurr -= backspaceAcceleration;
			}
		}
		backspaceTimer = 0;
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

void Widget_Textbox::move(sf::Vector2f offset)
{
	pos.x += offset.x;
	pos.y += offset.y;
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

std::string Widget_Textbox::getText()
{
	return text;
}

#pragma endregion

//####################################	BUTTON
#pragma region Button

Widget_Button::Widget_Button()
{
}

Widget_Button::~Widget_Button()
{
}

void Widget_Button::setState(ButtonState newState)
{
	state = newState;
}

void Widget_Button::setPosition(sf::Vector2f pos)
{
	graph.setPosition(pos);
}

void Widget_Button::move(sf::Vector2f offset)
{
	graph.move(offset);
}

void Widget_Button::setSize(sf::Vector2f size)
{
	float scaleX = size.x / graph.getGlobalBounds().width;
	float scaleY = size.y / graph.getGlobalBounds().height;

	graph.setScale(scaleX, scaleY);
}

void Widget_Button::setTexture(sf::Texture* texture)
{
	//Check for valid ptr
	if (!texture) {
		std::cout << "Invalid Button Texture..." << std::endl;
		return;
	}

	textureSize.x = texture->getSize().x / 3;
	textureSize.y = texture->getSize().y;

	graph.setTexture(*texture);
	graph.setTextureRect({0,0,textureSize.x,textureSize.y});
}

void Widget_Button::setTextureRect(sf::IntRect rect)
{
	graph.setTextureRect(rect);
}

void Widget_Button::poll(sf::RenderWindow& win, sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		//Get Mouse Pos
		sf::Vector2i mousePos = sf::Mouse::getPosition(win);
		if (graph.getGlobalBounds().contains({(float)mousePos.x, (float)mousePos.y})) {
			lastState = state;
			state = ButtonState::HOVER;
			graph.setTextureRect({ textureSize.x,0,textureSize.x,textureSize.y });
		}
		else {
			lastState = state;
			state = ButtonState::UNPRESS;
			graph.setTextureRect({ 0,0,textureSize.x,textureSize.y });
		}

	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (state == ButtonState::HOVER) {
			lastState = state;
			state = ButtonState::PRESS;
			graph.setTextureRect({ textureSize.x*2,0,textureSize.x,textureSize.y });
		}
	}

	if (event.type == sf::Event::MouseButtonReleased) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(win);
		if (state == ButtonState::PRESS) {
			lastState = state;
			if (graph.getGlobalBounds().contains({ (float)mousePos.x, (float)mousePos.y })) {
				lastState = state;
				state = ButtonState::HOVER;
				graph.setTextureRect({ textureSize.x,0,textureSize.x,textureSize.y });
			}
			else {
				lastState = state;
				state = ButtonState::UNPRESS;
				graph.setTextureRect({ 0,0,textureSize.x,textureSize.y });
			}
		}
	}

	

}

void Widget_Button::update(const float dt)
{
	
}

void Widget_Button::draw(sf::RenderWindow& win)
{
	win.draw(graph);
}

ButtonState Widget_Button::getState()
{
	return state;
}

bool Widget_Button::contains(sf::Vector2f pos)
{
	if (graph.getGlobalBounds().contains(pos)) {
		return true;
	}

	return false;
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
	hasFocus = false;
	//Update widgets
	for (size_t i = 0; i < widgets.size(); i++) {
		for (size_t j = 0; j < widgets.at(i)->size(); j++) {
			widgets.at(i)->at(j)->update(dt);
			if (widgets.at(i)->at(j)->getFocus()) {
				hasFocus = true;
			}
		}
	}
}

void Gui::draw(sf::RenderWindow& win)
{
	if (view) {
		win.setView(*view);
	}

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

	//Set widget view if applicable
	if (view) {
		widget->setView(view);
	}
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

void Gui::setView(sf::View* newView) {
	view = newView;
	
	for (int i = 0; i < numLayers; i++) {
		for (int j = 0; j < widgets.at(i)->size(); j++) {
			widgets.at(i)->at(j)->setView(newView);
		}
	}
}

void Gui::setMaxLayers(unsigned int max)
{
	maxLayers = max;
}

bool Gui::getFocus()
{
	return hasFocus;
}


