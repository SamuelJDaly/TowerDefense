#pragma once
#include "Gamestate.h"


class State_Editor : public Gamestate {
private:
	//## Data
	Gui* gui;
	sf::Texture* activeTileset = nullptr;
	sf::Font* font;

	//Camera
	sf::View view_map;
	sf::View view_gui;
	sf::Vector2f viewSize_map = { 1280.f,720.f };
	sf::Vector2f viewSize_gui = { 1280.f,720.f };
	sf::FloatRect viewport_map = { .25f,0.f,1.f,.2f };
	sf::FloatRect viewport_gui = { 0.f,0.f,1.f,1.f };

	sf::FloatRect cameraBounds = { -100.f,-100.f,600.f,600.f }; //How much can the camera offset
	float panSpeed = 300.f;
	float zoomSpeed = .03f;
	float currZoom = 1.f;
	sf::Vector2f zoomBounds = { 1.7f,.50f };

	//Gui
	float leftPanelRatio = .25f; //Fraction of view left panel covers (x axis)
	float bottomPanelRatio = .3f; //Fraction of view bottom panel covers (y axis)
	Widget_Button* btn_save;
	Widget_Button* btn_SizeUpX;
	Widget_Button* btn_SizeDnX;
	Widget_Button* btn_SizeUpY;
	Widget_Button* btn_SizeDnY;
	Widget_Button* btn_loadTilset;
	Widget_Panel* pnl_left;
	Widget_TabbedPanel* pnl_bottom;
	Widget_Label* label_mapSize;
	Widget_Textbox* txtBx_palettePath;
	Widget_Textbox* txtBx_mapPath;
	sf::Vector2f bottomPanelPos = { 0.f,0.f };

	//Texture Select
	Spritesheet spritesheet;
	std::vector<sf::Sprite> palette;
	int paletteColumns = 3;
	int numTextures = 0;
	sf::Vector2f palettePos = { 0.f,0.f };
	sf::Vector2f paletteSize = { 1.f,1.f };
	sf::Vector2f paletteRatio = { .75,.75 };
	sf::RectangleShape paletteBorder;
	int paletteSelect = -1;
	sf::RectangleShape selectBorder;



	//Map
	sf::FloatRect mapBoundry;
	sf::Vector2i mapSize = { 10,10 };
	float tileSize = 50.f;
	sf::Image blankImage;
	sf::Texture* blankTexture;
	TileMap* tilemap;
	sf::Color gridColor = sf::Color::Red;
	std::vector<sf::Vertex> grid_horizontal;
	std::vector<sf::Vertex> grid_vertical;


	//Path
	sf::CircleShape nodeDisplay;
	sf::CircleShape nodeButton;
	Node* pathHead = nullptr;
	Node* pathEnd = nullptr;
	std::vector<sf::Vertex> pathLines;

	//General
	std::string filename = "resource/map/editor";

	bool isPainting = false;
	bool nodePlace = false;
	bool isChanged = false;

	bool isPress_ctrl = false;
	bool isPress_z = false;
	bool isPress_s = false;

	//## Util
	void initGui();
	void initCamera();
	void initTest();
	void initMap();
	void initPalleteTool();
	void initPathTool();

	void addNode(sf::Vector2f pos);
	void remNode();
	void refreshGrid();

public:
	//Constructor and Destructor
	State_Editor(TextureHandler* textureHandler, sf::RenderWindow* window);
	~State_Editor();

	//Primary Functions
	void loadPallete(int txSize, std::string filepath);

	void saveMap(std::string filepath);
	void savePath(std::string filepath);
	void save();

	void loadMap(std::string filepath);
	void loadPath(std::string filepath);
	void load(std::string mapname);

	void poll(sf::RenderWindow& win, sf::Event& event);

	void updatePathTool();
	void updateCamera(float dt);
	void updatePainting();
	void update(float dt);


	void drawMap(sf::RenderWindow& win);
	void drawPallete(sf::RenderWindow& win);
	void drawRoundTool(sf::RenderWindow& win);
	void drawPathTool(sf::RenderWindow& win);
	void drawPath(sf::RenderWindow& win);
	void draw(sf::RenderWindow& win);
};


