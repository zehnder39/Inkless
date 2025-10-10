#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>

using namespace std;
using namespace sf;

// flags:
extern bool moving_left, moving_right, moving_up, moving_down;
extern bool facing_left;
extern bool looking_up, looking_down, looking_right, looking_left;
extern bool should_close, mouse_1, mouse_2, use_key, debug_key;

extern RenderWindow* window;
extern CircleShape entity1;
extern Sprite player_model;
extern RectangleShape ground;
extern vector<CircleShape> entities;
extern Texture ground_texture;
extern Texture rock_texture;
extern Texture player_texture;
extern Texture jump_texture;
extern Texture gutter_texture;
extern vector<CircleShape> debug_draw;
extern vector<string> debug_text;
extern Vector2i mouse_pos;
extern Vector2f player_local_poss;
extern Vector2f view_offset;
extern Vector2i tile_size;

void create_world();
void update_world();
void render();
void input();
void move_player();
void create_instance();
bool check_move(float dx, float dy);
void check_action();
bool check_breaking();
pair<Vector2i, Vector2i> pos_to_chunk_subc(Vector2f pos);

class Player
{
public:
	Vector2f position = {};
	Vector2i chunk = {};
	Vector2i subc = {};
	int jump_state = 0;
	float jump_offset = 0;
	Player(float set_x, float set_y);
	void move(float dx, float dy);
	void jump(bool moving, float dx);
};
extern Player player;

class Tile {
public:
	bool solid;
	bool breakable;
	int durability;
	float break_offset = 0;
	Vector2i subc;
	Vector2i chunk;
	Texture texture;

	virtual ~Tile() = default;

	virtual void interact() {}
	virtual void draw();
};

class Rock : public Tile
{
public:
	explicit Rock(Vector2i sub, Vector2i chun);
};

class Gutter : public Tile
{
public:
	bool up;
	bool down;
	bool left;
	bool right;
	explicit Gutter(Vector2i sub, Vector2i chun);
};

void delete_tile(Tile* tile);
extern Tile* breaking_mem;
Tile* get_tile(Vector2i chunk, Vector2i subc);
class Chunk
{
public:
	Vector2i position = {};
	unique_ptr<Tile> changeables[16][16] = {nullptr};
	RectangleShape ground;
	void rockdom();
	vector<Tile*> list_tiles();
	Chunk(Vector2i);
};
extern vector<vector<Chunk>> world_chunks;