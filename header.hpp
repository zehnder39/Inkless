#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>

using namespace std;
using namespace sf;

extern bool moving_left, moving_right, moving_up, moving_down;
extern bool should_close;
extern RenderWindow* window;
extern CircleShape entity1;
extern CircleShape player_model;
extern RectangleShape ground;
extern vector<CircleShape> entities;
extern Texture ground_texture;

void create_world();
void update_world();
void render();
void input();
void move_player();
void create_instance();

class Player
{
public:
	Vector2f position = {};
	Player(float set_x, float set_y);
	void move(float dx, float dy);
};
extern Player player;