#include "definer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "window.hpp"
#include "renderer.hpp"

#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>


float dist(Vector2f a, Vector2f b)
{
	float dx = a.x - b.x;
	float dy = a.y - b.y;
	float distance = sqrt(dx * dx + dy * dy);
	return distance;
}

Vector2f chunkSubcToPos(Vector2i chunk, Vector2i subc)
{
	return Vector2f(tile_size.x * (chunk.x * 16 + subc.x), tile_size.y * (chunk.y * 16 + subc.y));
}

pair<pair<Vector2i, Vector2i>, bool> inRange(Vector2i chunk, Vector2i subc)
{
	Vector2i nsubc = subc;
	Vector2i nchunk = chunk;
	if (subc.x > 15)
	{
		nsubc.x -= 16;
		nchunk.x += 1;
	}
	if (nsubc.x < 0)
	{
		nsubc.x += 16;
		nchunk.x -= 1;
	}
	if (nchunk.x < 0 or nchunk.x >= world_chunks.size())
		return make_pair(make_pair(Vector2i(0, 0), Vector2i(0, 0)), false);
	if (nsubc.y > 15)
	{
		nsubc.y -= 16;
		nchunk.y += 1;
	}
	if (nsubc.y < 0)
	{
		nsubc.y += 16;
		nchunk.y -= 1;
	}
	if (nchunk.y < 0 or nchunk.y >= world_chunks.size())
		return make_pair(make_pair(Vector2i(0, 0), Vector2i(0, 0)), false);
	return make_pair(make_pair(nchunk, nsubc), true);
}

pair<pair<Vector2i, Vector2i>, bool> tileLookingAt()
{
	float angle = - atan2(mouse_vector.y, mouse_vector.x);
	float vector_norm = sqrt(((mouse_vector.x * mouse_vector.x) / (tile_size.x * tile_size.x))+ ((mouse_vector.y * mouse_vector.y) / (tile_size.y * tile_size.y)));
	debug_text.push_back("vector norm = " + to_string(vector_norm));
	float norm = min( float(1.5) , vector_norm);
	float x = cos(angle) * norm * tile_size.x + player.position.x;
	float y = sin(angle) * norm * tile_size.y + player.position.y;
	return pos_to_chunk_subc(Vector2f(x, y));
}

pair<pair<Vector2i, Vector2i>, bool> pos_to_chunk_subc(Vector2f pos)
{
	Vector2i chunk = { static_cast<int>(floor(pos.x / (tile_size.x * 16))), static_cast<int>(floor(pos.y / (tile_size.y * 16))) };
	Vector2i subc = { static_cast<int>((static_cast<int>(pos.x) % (tile_size.x * 16)) / tile_size.x), static_cast<int>((static_cast<int>(pos.y) % (tile_size.y * 16)) / tile_size.y) };
	return inRange(chunk, subc);
}

Tile* getTile(Vector2i chunk, Vector2i subc)
{
	auto chunk_subc = inRange(chunk, subc);
	if (chunk_subc.second)
	{
		Tile* tile = world_chunks[chunk_subc.first.first.x][chunk_subc.first.first.y].changeables[chunk_subc.first.second.x][chunk_subc.first.second.y].get();
		return tile;
	}
	return nullptr;
}

