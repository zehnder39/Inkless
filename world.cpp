#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "header.hpp"

Vector2i tile_size = {64, 48};
vector<vector<Chunk>> world_chunks;


void create_world()
{
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
			std::cout << "Before updating surroundings: \n PLACED: unique_ptr.get() = " << slot.get()
				<< " &typeid(Tile)=" << &typeid(Tile)
				<< " &typeid(Gutter)=" << &typeid(Gutter)
				<< " sizeof(Tile)=" << sizeof(Tile)
				<< " sizeof(Gutter)=" << sizeof(Gutter) << '\n';
			update_surroundings(chunk, subc);
			auto& slot2 = world_chunks[chunk.x][chunk.y].changeables[subc.x][subc.y];
			std::cout << "After updating surroundings: \n PLACED: unique_ptr.get() = " << slot2.get()
				<< " &typeid(Tile)=" << &typeid(Tile)
				<< " &typeid(Gutter)=" << &typeid(Gutter)
				<< " sizeof(Tile)=" << sizeof(Tile)
				<< " sizeof(Gutter)=" << sizeof(Gutter) << '\n';
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
