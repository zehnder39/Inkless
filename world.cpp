#include <cmath>
#include <algorithm>
#include <iostream>

#include "definer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "saves.hpp"
#include "texture.hpp"
#include "player.hpp"
#include "tiles.hpp"
#include "entities.hpp"

int chunkSize = 16;
vector<vector<Chunk>> world_chunks;
Vector2i worldSize = { 3, 3 };

void create_world(string world_nam, int seed)
{
    world_name = world_nam;
	srand(seed);
	player.model.setOrigin({ 16.f, 16.f });
	for (int x = 0; x < worldSize.x; x++)
	{
		for (int y = 0; y < worldSize.y; y++)
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
	world_chunks[chunk.x][chunk.y].tiles[subc.x][subc.y].reset();
	removeFromVector(tickingTiles, make_pair(chunk, subc));
	update_surroundings(chunk, subc);
}

void deleteEntity(Entity* entity)
{
	removeFromVector(entities, entity);
}

bool breakingTile()
{
	if (!MouseInputManager::isActionActive("break"))
	{
		if (breaking_mem)
		{
			breaking_mem->break_offset = 0;
		}
		break_time = 0;
		return false;
	}

	Vector2i chunk_poss = tileLookingAt().first.first;
	Vector2i subc = tileLookingAt().first.second;
	if (tileLookingAt().second)
	{
		Chunk& chunk = world_chunks[chunk_poss.x][chunk_poss.y];
		Tile* tile = chunk.tiles[subc.x][subc.y].get();
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

				tile->dropLoot();
				delete_tile(tile);
				break_time = 0;
			}
			else
				tile->break_offset = float((break_time % 5 - 2) * 5 * break_time / tile->durability);
			return true;
		}
	}
}

void place_gutter()
{
	Vector2i chunk = tileLookingAt().first.first;
	Vector2i subc = tileLookingAt().first.second;
	if (tileLookingAt().second)
	{
		if (!getTile(chunk, subc))
		{
			world_chunks[chunk.x][chunk.y].tiles[subc.x][subc.y] = make_unique<Gutter>(Vector2i(subc.x, subc.y), Vector2i(chunk.x, chunk.y));
			auto& slot = world_chunks[chunk.x][chunk.y].tiles[subc.x][subc.y];
			update_surroundings(chunk, subc);
			auto& slot2 = world_chunks[chunk.x][chunk.y].tiles[subc.x][subc.y];
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
			Tile* tile = getTile(nchunk, nsubc);
			if (!tile)
				continue;
			tile->update();
		}
	}
}

void update_world()
{
	if (!breakingTile())
		break_time = 0;
	else
		break_time++;
	if (MouseInputManager::isActionTapped("place"))
		place_gutter();
	for (auto& entity : entities)
	{
		entity->update();
	}
	for (auto chunkSubc : tickingTiles)
	{
		Tile* tile = getTile(chunkSubc.first, chunkSubc.second);
		if (tile)
			tile->update();
	}
}

Chunk::Chunk()
{
	ground.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&TextureManager::get("ground/ground3"));
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
	debugGround.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	debugGround.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	debugGround.setTexture(&TextureManager::get("ground/debug"));
	debugGround.setTextureRect(IntRect({ 0, 0 }, { 1024, 1024 }));
}

Chunk::Chunk(const Vector2i poss)
{
	position = poss;
	ground.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&TextureManager::get("ground/ground3"));
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
	debugGround.setSize(Vector2f(16 * tile_size.x, 16 * tile_size.y));
	debugGround.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	debugGround.setTexture(&TextureManager::get("ground/debug"));
	debugGround.setTextureRect(IntRect({ 0, 0 }, { 1024, 1024 }));
}

vector<Tile*> Chunk::list_tiles()
{
	vector<Tile*> in_chunk;
	for (int x = 0; x < 16; x++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (tiles[x][y] != nullptr)
				in_chunk.push_back(tiles[x][y].get());
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
			if (x == 8 && y == 8)
			{
				tiles[x][y] = make_unique<Chest>(position, Vector2i(x, y));
				continue;
			}
			if (x == 7 && y == 8)
			{
				tiles[x][y] = make_unique<Furnace>(position, Vector2i(x, y));
				continue;
			}
			int num = rand() % 8;
			if (num == 0)
				tiles[x][y] = make_unique<Rock>(Vector2i(x, y), Vector2i(position.x, position.y));
		}
	}
}
