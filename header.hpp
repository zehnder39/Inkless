#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include <array>
#include <string>

using namespace std;
using namespace sf;

// flags:
extern bool moving_left, moving_right, moving_up, moving_down, break_key;
extern bool facing_left, facing_up;
extern bool should_close;

extern RenderWindow* window;
extern CircleShape entity1;
extern Sprite player_model;
extern RectangleShape ground;
extern vector<CircleShape> entities;
extern Texture ground_texture;
extern Texture rock_texture;
extern Texture player_texture;
extern Texture jump_texture;
extern vector<CircleShape> debug_draw;
extern vector<string> debug_text;


void create_world();
void update_world();
void render();
void input();
void move_player();
void create_instance();
bool check_move(float dx, float dy);
void check_action();
bool check_breaking();

class Player
{
public:
	Vector2f position = {};
	Vector2i chunk = {};
	Vector2i sub_c = {};
	int jump_state = 0;
	float jump_offset = 0;
	Player(float set_x, float set_y);
	void move(float dx, float dy);
	void jump(bool moving, float dx);
};
extern Player player;

class Tile
{
public:
	bool solid;
	bool breakable;
	int durability;
	Vector2f position;
	Vector2i sub_c;
	Vector2i chunk;
	Texture texture;
	Tile(Vector2f poss, Vector2i sub, Vector2i chun, bool sol, bool breaka, int dura, Texture tex);
	void destroy();
	void draw();
};

extern Tile* breaking_mem;

class Chunk
{
public:
	Vector2i position = {};
	Tile* changeables[16][16] = {nullptr};
	RectangleShape ground;
	void rockdom(Texture tex);
	vector<Tile*> list_tiles();
	Chunk(Vector2i);
};
extern vector<vector<Chunk>> world_chunks;