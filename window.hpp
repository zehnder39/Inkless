#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Window.hpp>
#include <SFML/Window/Event.hpp>
#include <functional>
#include "renderer.hpp"

using namespace std;
using namespace sf;

//gui colors
extern Color baseButtonColor;
extern Color baseHoveredColor;
extern Color basePlaceholderTextColor;


extern Sprite player_model;
extern Texture ground_texture, debugGroundTexture;
extern Texture rock_texture;
extern Texture player_texture;
extern Texture jump_texture;
extern Texture gutter_point_texture, gutter_horizontal_texture, gutter_vertical_texture, gutter_up_left_texture, gutter_down_left_texture, gutter_branch_horizontal_texture, gutter_branch_horizontal_down_texture, gutter_branch_vertical_texture, gutter_up_texture, gutter_down_texture, gutter_left_texture;

extern RenderWindow* window;

void create_instance();
void load_textures();
void createTypeMenuScreens();

enum class GameState
{
	TitleScreen,
	WorldCreation,
	WorldLoading,
	InGame
};

extern GameState currentState;


struct Button
{
	Text text = Text(font, "Button", 1);
	RectangleShape box;
	Color buttonColor = baseButtonColor;
	Color hoveredColor = baseHoveredColor;

	bool checkOnBox();
	void draw();
	function<void()> action = []{};
	Button();
	Button(Text txt, float x, float y);
	Button(Text txt, Vector2f xy);
};

struct TextBox
{
	Text placeholderText = Text(font, "Type here dumbass..", 1);
	RectangleShape box;
	Color boxColor;
	string text;
	bool onlyInt = false;
	bool selected = false;

	bool checkOnBox();
	void draw();

	TextBox();
	TextBox(float x, float y, string placeholderTxt, int charSize);
	TextBox(Vector2f xy, string placeholderTxt, int charSize);
};

struct TypeMenuScreen
{
	Color backgroundColor = Color(100, 100, 100);
	string title;
	vector<Button> buttons;
	vector<TextBox> textBoxes;
	void draw();
	void update();
	TypeMenuScreen() = default;
	TypeMenuScreen(string tit) : title(tit) {}
};

extern TypeMenuScreen titleScreen;
extern TypeMenuScreen worldCreationScreen;