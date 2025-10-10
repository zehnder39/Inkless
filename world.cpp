#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "header.hpp"

Vector2i tile_size = {64, 48};
vector<vector<Chunk>> world_chunks;

void delete_tile(Tile* tile)
{
	world_chunks[tile->chunk.x][tile->chunk.y].changeables[tile->subc.x][tile->subc.y] = nullptr;
	delete tile;
}

void create_world()
{
	player_model.setOrigin({ 24.f, 16.f });
	entity1.setFillColor(Color::Green);
	entities.push_back(entity1);
	if (!ground_texture.loadFromFile("ground3.png", false, IntRect({ 0, 0 }, { 128, 128 })))
		cout << "Failed to load ground.png!" << endl;
	ground_texture.setRepeated(true);
	if (!player_texture.loadFromFile("squid_player.png", false, IntRect({ 0, 0 }, { 32, 32 })))
		cout << "Failed to load player.png!" << endl;
	if (!jump_texture.loadFromFile("squid_jump.png", false, IntRect({ 0, 0 }, { 32, 32 })))
		cout << "Failed to load squid_jump.png!" << endl;
	player_model.setTexture(player_texture);
	player_model.setTextureRect(IntRect({ 0, 0 }, { 32, 32 }));
	player_model.setScale({ 2.f, 2.f });
	if (!rock_texture.loadFromFile("rock.png", false, IntRect({ 0, 0 }, { 16, 16 })))
		cout << "Failed to load rock.png!" << endl;

	for (int x = 0; x < 3; x++)
	{
		for (int y = 0; y < 3; y++)
		{
			Chunk c({ x ,y });
			if (world_chunks.size() < x)
				world_chunks.push_back({});
			world_chunks[x].push_back(c);
			world_chunks[x][y].rockdom();
		}
	}
	
	
}

int break_time = 0;
Tile* breaking_mem;

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
		Vector2i chunk_poss = pos_to_chunk_subc({ player.position.x + dx, player.position.y + dy }).first;
		Vector2i subc = pos_to_chunk_subc({ player.position.x + dx, player.position.y + dy }).second;
		if (0 <= chunk_poss.x and chunk_poss.x < world_chunks.size() and 0 <= chunk_poss.y and chunk_poss.y < world_chunks[chunk_poss.x].size())
		{
			Chunk& chunk = world_chunks[chunk_poss.x][chunk_poss.y];
			Tile* tile = chunk.changeables[subc.x][subc.y].get();
			CircleShape cir(30.f);
			cir.setFillColor(Color::Red);
			cir.setPosition(Vector2f(chunk_poss.x * (tile_size.x * 16) + subc.x * tile_size.x, chunk_poss.y * (tile_size.y * 16) + subc.y * tile_size.y));
			debug_draw.push_back(cir);
			if (tile != nullptr)
			{
				if (tile != breaking_mem)
				{
					break_time = 0;
					breaking_mem = tile;
				}
				else if (break_time >= tile->durability)
				{
					delete_tile(tile);
					break_time = 0;
				}
				else
					tile->break_offset = (break_time % 5 - 2) * 5 * break_time / tile->durability;
				return true;
			}
		}
	}
	return false;
}

void place_gutter()
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
	Vector2i chunk = pos_to_chunk_subc({ player.position.x + dx, player.position.y + dy }).first;
	Vector2i subc = pos_to_chunk_subc({ player.position.x + dx, player.position.y + dy }).second;
	if (0 <= chunk.x and chunk.x < world_chunks.size() and 0 <= chunk.y and chunk.y < world_chunks[chunk.x].size())
	{
		CircleShape cir(30.f);
		cir.setFillColor(Color::Blue);
		cir.setPosition(Vector2f(chunk.x * (tile_size.x * 16) + subc.x * tile_size.x, chunk.y * (tile_size.y * 16) + subc.y * tile_size.y));
		debug_draw.push_back(cir);
		if (get_tile(chunk, subc) == nullptr)
		{
			debug_text.push_back("Placing ...");
			world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y] = make_unique<Rock>(Vector2i(subc.x, subc.y), Vector2i(chunk.x, chunk.y));
		}
	}
}

void update_world()
{
	if (!check_breaking())
		break_time = 0;
	else
		break_time++;
	if (use_key)
		place_gutter();
}
