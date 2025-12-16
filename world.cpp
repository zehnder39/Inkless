#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "definer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "saves.hpp"

Vector2i tile_size = {64, 48};
vector<vector<Chunk>> world_chunks;

void create_world(string world_nam, int seed)
{
    world_name = world_nam;
	srand(seed);
	player_model.setOrigin({ 16.f, 16.f });
	entity1.setFillColor(Color::Green);
	entities.push_back(entity1);
	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Chunk c({ x ,y });
			if (int(world_chunks.size()) <= x)
				world_chunks.push_back({});
			world_chunks[x].push_back(std::move(c));
			world_chunks[x][y].rockdom();
		}
	}	
}

int break_time = 0;
Tile* breaking_mem = nullptr;

void delete_tile(Tile* tile)
{
	Vector2i chunk = tile->chunk;
	Vector2i subc = tile->subc;
	world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y].reset();
	world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y] = nullptr;
	update_surroundings(chunk, subc);
}

bool check_breaking()
{
	if (mouse_1)
	{
		int dx = tile_size.x;
		if (looking_left)
			dx = -tile_size.x;
		else if (looking_right)
			dx = tile_size.x;
		else
			dx = 0;
		int dy = tile_size.y;
		if (looking_up)
			dy = -tile_size.y;
		else if (looking_down)
			dy = tile_size.y;
		else
			dy = 0;
		Vector2i chunk_poss = tile_looking_at().first.first;
		Vector2i subc = tile_looking_at().first.second;
		if (tile_looking_at().second)
		{
			Chunk& chunk = world_chunks[chunk_poss.x][chunk_poss.y];
			Tile* tile = chunk.changeables[subc.x][subc.y].get();
			CircleShape cir(30.f);
			cir.setFillColor(Color::Red);
			cir.setPosition(Vector2f(chunk_poss.x * (tile_size.x * 16) + subc.x * tile_size.x, chunk_poss.y * (tile_size.y * 16) + subc.y * tile_size.y));
			debug_draw.push_back(cir);
			if (tile)
			{
				if (tile != breaking_mem)
				{
					break_time = 0;
					breaking_mem = tile;
					tile->break_offset = 0;
				}
				else if (break_time >= tile->durability)
				{
					delete_tile(tile);
					break_time = 0;
				}
				else
					tile->break_offset = float((break_time % 5 - 2) * 5 * break_time / tile->durability);
				return true;
			}
		}
	}
	if (breaking_mem)
	{
		breaking_mem->break_offset = 0;
	}
	break_time = 0;
	return false;
}

void place_gutter()
{
	Vector2i chunk = tile_looking_at().first.first;
	Vector2i subc = tile_looking_at().first.second;
	if (tile_looking_at().second)
	{
		CircleShape cir(30.f);
		cir.setFillColor(Color::Blue);
		cir.setPosition(Vector2f(chunk.x * (tile_size.x * 16) + subc.x * tile_size.x, chunk.y * (tile_size.y * 16) + subc.y * tile_size.y));
		debug_draw.push_back(cir);
		if (!get_tile(chunk, subc))
		{
			world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y] = make_unique<Gutter>(Vector2i(subc.x, subc.y), Vector2i(chunk.x, chunk.y));
			auto& slot = world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y];
			update_surroundings(chunk, subc);
			auto& slot2 = world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y];
		}
	}
}

void update_surroundings(Vector2i chunk, Vector2i subc)
{
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			Vector2i nsubc = subc;
			Vector2i nchunk = chunk;
			nsubc += Vector2i(x, y);
			Tile* tile = get_tile(nchunk, nsubc);
			if (!tile)
				continue;
			tile->update();
		}
	}
}

void update_world()
{
	if (!check_breaking())
		break_time = 0;
	else
		break_time++;
	if (mouse_2)
		place_gutter();
}


Player::Player(float set_x, float set_y)
{
	position.x = set_x;
	position.y = set_y;
}


void Player::updateChunkSubc()
{
	auto chunkSubc = pos_to_chunk_subc(player.position);
	chunk = chunkSubc.first.first;
	subc = chunkSubc.first.second;
}

Rock::Rock()
{
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	texture = rock_texture;
	baseOffset = Vector2f(0, tile_size.y - 64);
}

Rock::Rock(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	texture = rock_texture;
	baseOffset = Vector2f(0, tile_size.y - 64);
}

Gutter::Gutter(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	solid = false;
	breakable = true;
	durability = 10;
	break_offset = 0;
}

Gutter::Gutter()
{
	solid = false;
	breakable = true;
	durability = 10;
	break_offset = 0;
}

void Gutter::update()
{
	bool callBreak = false;
	up = false;
	down = false;
	left = false;
	right = false;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x, subc.y - 1))))
		up = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x, subc.y + 1))))
		down = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x - 1, subc.y))))
		left = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x + 1, subc.y))))
		right = true;
	if (!right && !left && !up && !down)
	{
		texture = gutter_point_texture;
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (!right && !left && !up && down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_down_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && !left && up && !down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_up_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && !left && up && down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_vertical_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && left && !up && !down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_left_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (!right && left && !up && down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_down_left_texture;
		center = Vector2f(tile_size.x / 4, tile_size.y * 3 / 4);
	}

	if (!right && left && up && !down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_up_left_texture;
		center = Vector2f(tile_size.x / 4, tile_size.y / 4);
	}

	if (!right && left && up && down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_vertical_texture;
		center = Vector2f(tile_size.x / 6, tile_size.y / 2);
	}
	if (right && !left && !up && !down)
	{
		texture = gutter_left_texture;
		baseScale = Vector2f(-float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (right && !left && !up && down)
	{
		texture = gutter_down_left_texture;
		baseScale = Vector2f(-float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 3 / 4, tile_size.y * 3 / 4);
	}
	if (right && !left && up && !down)
	{
		texture = gutter_up_left_texture;
		baseScale = Vector2f(-float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 3 / 4, tile_size.y / 4);
	}
	if (right && !left && up && down)
	{
		texture = gutter_branch_vertical_texture;
		baseScale = Vector2f(-float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 5 / 6, tile_size.y / 2);
	}
	if (right && left && !up && !down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_horizontal_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (right && left && !up && down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_horizontal_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y * 5 / 6);
	}
	if (right && left && up && !down)
	{
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_horizontal_down_texture;
		center = Vector2f(tile_size.x / 2, tile_size.y / 6);
	}
	if (right && left && up && down)
		delete_tile(get_tile(chunk, subc));

}

Chunk::Chunk()
{
	ground.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&ground_texture);
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
	debugGround.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	debugGround.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	debugGround.setTexture(&debugGroundTexture);
	debugGround.setTextureRect(IntRect({ 0, 0 }, { 1024, 1024 }));
}

Chunk::Chunk(const Vector2i poss)
{
	position = poss;
	ground.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&ground_texture);
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
	debugGround.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	debugGround.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	debugGround.setTexture(&debugGroundTexture);
	debugGround.setTextureRect(IntRect({ 0, 0 }, { 1024, 1024 }));
}
vector<Tile*> Chunk::list_tiles()
{
	vector<Tile*> in_chunk;
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (changeables[x][y] != nullptr)
				in_chunk.push_back(changeables[x][y].get());
		}
	}
	return in_chunk;
}

void Chunk::rockdom()
{
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			int num = rand() % 8;
			if (num == 0)
				changeables[x][y] = make_unique<Rock>(Vector2i(x, y), Vector2i(position.x, position.y));
		}
	}
}