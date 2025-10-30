#include "header.hpp"

#include <iostream>

void tile_interaction()
{
	
}

void move_player()
{
	player.speed = player.base_speed;
	float sx = 0;
	float sy = 0;
	if (moving_left) {
		sx += -player.speed;
	}
	if (moving_right) {
		sx += player.speed;
	}
	if (moving_up)
		sy += -player.speed;
	if (moving_down)
		sy += player.speed;
	player.move(sx, sy);
}

bool check_move(float dx, float dy)
{
	float nx = player.position.x + dx;
	float ny = player.position.y + dy;
	auto chunk_subc = pos_to_chunk_subc({ nx, ny });
	Vector2i chunk = chunk_subc.first.first;
	Vector2i subc = chunk_subc.first.second;
	if (chunk_subc.second)
	{
		Chunk& chunk_to_check = world_chunks[chunk.x][chunk.y];
		if (chunk_to_check.changeables[subc.x][subc.y] == nullptr)
			return true;
		else if (!chunk_to_check.changeables[subc.x][subc.y]->solid)
			return true;
	}
	return false;
}

void check_action()
{
	if (use_key)
		tile_interaction();
}