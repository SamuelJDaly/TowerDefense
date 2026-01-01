#include "Gui.h"

//############################################################################################
//				ELEMENTS
//############################################################################################

//#################################### BASE CLASS
#pragma region base
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
#pragma endregion


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
#pragma region tabbedpanel
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

#pragma endregion

//#################################### SCROLLABLE PANEL
#pragma region scrollpanel

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

#pragma endregion

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

void Widget_Label::setHighlightColor(sf::Color col)
{
	highlightColor = col;

	if (isHighlight) {
		label.setFillColor(highlightColor);
	}
}

void Widget_Label::toggleHighlight()
{
	isHighlight = !isHighlight;

	if (isHighlight) {
		label.setFillColor(highlightColor);
	}
	else {
		label.setFillColor(baseColor);
	}
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
	baseColor = color;

	if (!isHighlight) {
		label.setFillColor(baseColor);
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

void Widget_Button::align()
{
	float scaleX = 1;
	float scaleY = 1;

	//Calculate floor and ceiling fractions to make non equally
	//divisible textures work
	int thirdF = std::floor(textureSize.x / 3.f);
	int thirdC = std::ceil(textureSize.x / 3.f);
	int thirdFY = std::floor(textureSize.y / 3.f);
	int thirdCY = std::ceil(textureSize.y / 3.f);
	int halfF = std::floor(textureSize.x / 2.f);
	int halfC = std::ceil(textureSize.x / 2.f);
	int halfFY = std::floor(textureSize.y / 2.f);
	int halfCY = std::ceil(textureSize.y / 2.f);

	float thirdX = size.x / 3;
	float thirdY = size.y / 3;

	switch (sliceType) {
	case en_SliceType::NONE:
		//Pos
		slices.at(0)->setPosition(pos);

		//Scale
		scaleX = size.x / textureSize.x;
		scaleY = size.y / textureSize.y;
		slices.at(0)->setScale(scaleX, scaleY);

		break;
	case en_SliceType::THREE:
		//Pos
		slices.at(0)->setPosition(pos);
		slices.at(1)->setPosition({pos.x + thirdF, pos.y});
		slices.at(2)->setPosition({ pos.x + (size.x - thirdF), pos.y});

		//Scale
		slices.at(0)->setScale({ 1, size.y / textureSize.y });
		scaleX = (size.x - (2 * thirdF)) / (thirdC);
		slices.at(1)->setScale({scaleX , size.y / textureSize.y });
		slices.at(2)->setScale({ 1, size.y / textureSize.y });

		break;
	case en_SliceType::FOUR:
		//Pos
		slices.at(0)->setPosition({pos.x, pos.y});
		slices.at(1)->setPosition({ pos.x + (size.x / 2.f), pos.y});
		slices.at(2)->setPosition({ pos.x, pos.y + (size.y / 2.f)});
		slices.at(3)->setPosition({ pos.x + (size.x / 2.f), pos.y + (size.y  / 2.f)});

		//Scale
		slices.at(0)->setScale({ (size.x / 2.f) / (halfF),(size.y / 2.f) / (halfFY)});
		slices.at(1)->setScale({ (size.x / 2.f) / (halfC), (size.y / 2.f) / (halfFY)});
		slices.at(2)->setScale({ (size.x / 2.f) / (halfF), (size.y / 2.f) / (halfCY)});
		slices.at(3)->setScale({ (size.x / 2.f) / (halfC), (size.y / 2.f) / (halfCY)});
		

		break;
	case en_SliceType::NINE:
		//Pos
		slices.at(0)->setPosition({pos.x, pos.y});
		slices.at(1)->setPosition({ pos.x + thirdF, pos.y});
		slices.at(2)->setPosition({ pos.x + size.x - thirdF, pos.y});

		slices.at(3)->setPosition({ pos.x, pos.y + thirdFY });
		slices.at(4)->setPosition({ pos.x + thirdF, pos.y + thirdFY });
		slices.at(5)->setPosition({ pos.x + size.x - thirdF, pos.y + thirdFY });

		slices.at(6)->setPosition({ pos.x, pos.y + size.y - thirdFY});
		slices.at(7)->setPosition({ pos.x + thirdF, pos.y + size.y - thirdFY });
		slices.at(8)->setPosition({ pos.x + size.x - thirdF, pos.y + size.y - thirdFY });

		//Scale
		scaleX = (size.x - (2 * thirdF))  / (thirdC);
		scaleY = (size.y - (2 * thirdFY)) / (thirdCY);
		
		slices.at(1)->setScale(scaleX, 1);
		slices.at(3)->setScale(1, scaleY);
		slices.at(4)->setScale(scaleX,scaleY);
		slices.at(5)->setScale(1, scaleY);
		slices.at(7)->setScale(scaleX, 1);

		break;
	}
}

void Widget_Button::updateTexture()
{
	int offset = 0;

	//Calculate floor and ceiling fractions to make non equally
	//divisible textures work
	int thirdF = std::floor(textureSize.x / 3.f);
	int thirdC = std::ceil(textureSize.x / 3.f);
	int thirdFY = std::floor(textureSize.y / 3.f);
	int thirdCY = std::ceil(textureSize.y / 3.f);
	int halfF = std::floor(textureSize.x / 2.f);
	int halfC = std::ceil(textureSize.x / 2.f);
	int halfFY = std::floor(textureSize.y / 2.f);
	int halfCY = std::ceil(textureSize.y / 2.f);

	for (sf::Sprite* s : slices) {
		s->setTexture(*texture);
	}

	switch (state) {
	case ButtonState::UNPRESS:
		offset = 0;
		break;
	case ButtonState::HOVER:
		offset = textureSize.x;
		break;
	case ButtonState::PRESS:
		offset = 2 * textureSize.x;
		break;
	}

	switch (sliceType) {
	case en_SliceType::NONE:
		slices.front()->setTextureRect({ offset,0,(int)textureSize.x,(int)textureSize.y });
		break;
	case en_SliceType::THREE:

		slices.at(0)->setTextureRect({ offset,0,thirdF,(int)textureSize.y});
		slices.at(1)->setTextureRect({ offset + thirdF,0,thirdC,(int)textureSize.y });
		slices.at(2)->setTextureRect({ offset + thirdF + thirdC,0,thirdF,(int)textureSize.y });

		break;
	case en_SliceType::FOUR:

		slices.at(0)->setTextureRect({ offset,0,halfF,halfFY });
		slices.at(1)->setTextureRect({ offset + halfF,0,halfC,halfFY });
		slices.at(2)->setTextureRect({ offset, halfFY,halfF,halfCY});
		slices.at(3)->setTextureRect({ offset + halfF, halfFY,halfC,halfCY});

		break;
	case en_SliceType::NINE:

		slices.at(0)->setTextureRect({ offset,0,thirdF,thirdFY });
		slices.at(1)->setTextureRect({ offset + thirdF,0,thirdC,thirdFY });
		slices.at(2)->setTextureRect({ offset + thirdF + thirdC,0,thirdF,thirdFY });

		slices.at(3)->setTextureRect({ offset,thirdFY,thirdF,thirdCY });
		slices.at(4)->setTextureRect({ offset + thirdF,thirdFY,thirdC,thirdCY });
		slices.at(5)->setTextureRect({ offset + thirdF + thirdC, thirdFY,thirdF,thirdCY });

		slices.at(6)->setTextureRect({ offset,thirdFY + thirdCY,thirdF,thirdFY });
		slices.at(7)->setTextureRect({ offset + thirdF,thirdFY + thirdCY,thirdC,thirdFY });
		slices.at(8)->setTextureRect({ offset + thirdF + thirdC,thirdFY + thirdCY,thirdF,thirdFY });
		break;
	}

}

Widget_Button::Widget_Button()
{
	for (int i = 0; i < 3; i++) {
		slices.push_back(new sf::Sprite());
	}
}

Widget_Button::~Widget_Button()
{
	for (sf::Sprite* s : slices) {
		delete s;
	}
}

void Widget_Button::setState(ButtonState newState)
{
	state = newState;
}

void Widget_Button::setPosition(sf::Vector2f newPos)
{
	sf::Vector2f offset = { 0,0 };
	offset.x = newPos.x - pos.x;
	offset.y = newPos.y - pos.y;
	pos = newPos;

	bounds.left = pos.x;
	bounds.top = pos.y;

	for (sf::Sprite* s : slices) {
		s->move(offset);
	}
}

void Widget_Button::move(sf::Vector2f offset)
{
	bounds.top += offset.y;
	bounds.left += offset.x;

	for (sf::Sprite* s : slices) {
		s->move(offset);
	}
}

void Widget_Button::setSize(sf::Vector2f newSize)
{
	size = newSize;

	bounds.width = size.x;
	bounds.height = size.y;

	this->align();
}

void Widget_Button::setTexture(sf::Texture* newTexture)
{
	//Check for valid ptr
	if (!newTexture) {
		std::cout << "Invalid Button Texture..." << std::endl;
		return;
	}

	texture = newTexture;

	textureSize.x = texture->getSize().x / 3.f;
	textureSize.y = texture->getSize().y;


	this->updateTexture();
	this->align();
}

void Widget_Button::setType(en_SliceType newType)
{
	sliceType = newType;

	for (sf::Sprite* s : slices) {
		delete s;
	}

	slices.clear();

	switch (sliceType) {
	case  en_SliceType::NONE:
		slices.push_back(new sf::Sprite());
		break;
	case  en_SliceType::THREE:
		for (int i = 0; i < 3; i++) {
			slices.push_back(new sf::Sprite());
		}
		break;
	case  en_SliceType::FOUR:
		for (int i = 0; i < 4; i++) {
			slices.push_back(new sf::Sprite());
		}
		break;
	case  en_SliceType::NINE:
		for (int i = 0; i < 9; i++) {
			slices.push_back(new sf::Sprite());
		}
		break;
	}

	this->updateTexture();
	this->align();
}

void Widget_Button::poll(sf::RenderWindow& win, sf::Event& event)
{
	if (event.type == sf::Event::MouseMoved) {
		//Get Mouse Pos
		sf::Vector2i mousePos = sf::Mouse::getPosition(win);
		if (bounds.contains({(float)mousePos.x, (float)mousePos.y})) {
			lastState = state;
			state = ButtonState::HOVER;
			this->updateTexture();
		}
		else {
			lastState = state;
			state = ButtonState::UNPRESS;
			this->updateTexture();
		}

	}

	if (event.type == sf::Event::MouseButtonPressed) {
		if (state == ButtonState::HOVER) {
			lastState = state;
			state = ButtonState::PRESS;
			this->updateTexture();
		}
	}

	if (event.type == sf::Event::MouseButtonReleased) {
		sf::Vector2i mousePos = sf::Mouse::getPosition(win);
		if (state == ButtonState::PRESS) {
			lastState = state;
			if (bounds.contains({ (float)mousePos.x, (float)mousePos.y })) {
				lastState = state;
				state = ButtonState::HOVER;
				this->updateTexture();
			}
			else {
				lastState = state;
				state = ButtonState::UNPRESS;
				this->updateTexture();
			}
		}
	}

	

}

void Widget_Button::update(const float dt)
{
	
}

void Widget_Button::draw(sf::RenderWindow& win)
{
	for (sf::Sprite* s : slices) {
		win.draw(*s);
	}
}

ButtonState Widget_Button::getState()
{
	return state;
}

bool Widget_Button::contains(sf::Vector2f pos)
{
	if (bounds.contains(pos)) {
		return true;
	}

	return false;
}

#pragma endregion


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


