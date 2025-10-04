#include "header.hpp"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>


vector<CircleShape> entities;

Player player(960, 540);
CircleShape entity1(100.f);
RectangleShape ground({1024.f, 768.f});
RenderWindow* window = nullptr;
Texture ground_texture;
Texture rock_texture;
Texture player_texture;
Texture jump_texture;
Sprite player_model(player_texture);
vector<vector<Chunk>> world_chunks = { {}, {}, {} };
Tile* breaking[2];

pair<Vector2i, Vector2i> pos_to_chunk_subc(Vector2f pos)
{
	Vector2i chunk = { static_cast<int>(floor(pos.x / (tile_size.x * 16))), static_cast<int>(floor(pos.y / (tile_size.y * 16))) };
	Vector2i sub_c = { static_cast<int>((static_cast<int>(pos.x) % (tile_size.x * 16)) / tile_size.x), static_cast<int>((static_cast<int>(pos.y) % (tile_size.y * 16)) / tile_size.y) };
	return make_pair(chunk, sub_c);
}

Tile* get_tile(Vector2i chunk, Vector2i subc)
{
	Tile* tile = world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y];
	return tile;
}

Player::Player(float set_x, float set_y)
{
	position.x = set_x;
	position.y = set_y;
}
void Player::move(float dx, float dy)
{
	if (dx > 0)
		facing_left = false;
	else if (dx < 0)
		facing_left = true;
	float norm = 1;
	if (dx != 0 and dy != 0)
		norm = sqrtf(dx / dx + dy / dy);
	if (check_move(dx, 0))
		position.x += dx / norm;
	if (check_move(0, dy))
		position.y += dy / norm;
	chunk = pos_to_chunk_subc(player.position).first;
	jump(dx != 0 or dy != 0, dx);
}

void Player::jump(bool moving, float dx)
{
	int animation_time = 25;
	int jump_angle = 30;
	if (jump_state != 0)
	{	
		player_model.setTexture(jump_texture);
		int mod = 1;
		if (facing_left)
			mod = -1;
		player_model.setRotation(degrees(jump_angle * mod * (animation_time - jump_state) * 2 / animation_time));
		player_model.setScale({ mod * 2.f, 2.f });
		jump_offset = 3 * (fabsf(jump_state - animation_time / 2) - animation_time / 2);
		jump_state--;
	}
	if (jump_state == 0)
	{
		player_model.setRotation(degrees(0));
		if (moving)
			jump_state = animation_time;
		jump_offset = 0;
		player_model.setTexture(player_texture);
	}
}

Tile::Tile(Vector2f poss,Vector2i sub, Vector2i chun, bool sol, bool breaka, int dura , Texture tex)
{
	solid = sol;
	breakable = breaka;
	texture = tex;
	position = poss;
	durability = dura;
	sub_c = sub;
	chunk = chun;
}

void Tile::draw()
{
	Sprite sprite(texture);
	sprite.setPosition({ position.x + break_offset, position.y });
	sprite.scale({ 4.f, 4.f });
	window->draw(sprite);
}

Chunk::Chunk(Vector2i poss)
{
	position = poss;
	ground.setSize({ 1024.f, 768.f });
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&ground_texture);
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
}
vector<Tile*> Chunk::list_tiles()
{
	vector<Tile*> in_chunk;
	for (int x = 0; x < 16; x ++)
	{
		for (int y = 0; y < 16; y++)
		{
			in_chunk.push_back(changeables[x][y]);
		}
	}
	return in_chunk;
}

void Chunk::rockdom(Texture tex)
{
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			int num = rand() % 8;
			if (num == 0)
				changeables[x][y] = new Tile(Vector2f( (x * tile_size.x) + (position.x * (tile_size.x * 16)), (y * tile_size.y) + (position.y * (tile_size.y * 16)) ), Vector2i(x,y), Vector2i(position.x, position.y), true, true, 45, tex);
		}
	}
}
