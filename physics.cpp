#include "header.hpp"

#include <iostream>

void move_player()
{
	float sx = 0;
	float sy = 0;
	if (moving_left) {
		sx += -3.5;
	}
	if (moving_right) {
		sx += 3.5;
	}
	if (moving_up)
		sy += -3.5;
	if (moving_down)
		sy += 3.5;
	player.move(sx, sy);
}

bool check_move(float dx, float dy)
{
	float nx = player.position.x + dx;
	float ny = player.position.y + dy;
	Vector2i chunk = pos_to_chunk_subc({ nx, ny }).first;
	Vector2i subc = pos_to_chunk_subc({ nx, ny }).second;
	if (0 <= chunk.x and chunk.x <= int(world_chunks.size()))
	{
		if (0 <= chunk.y and chunk.y <= int(world_chunks[chunk.x].size()))
		{
			Chunk& chunk_to_check = world_chunks[chunk.x][chunk.y];
			if (chunk_to_check.changeables[subc.x][subc.y] == nullptr)
				return true;
			else if (chunk_to_check.changeables[subc.x][subc.y]->solid == false)
				return true;
		}
	}
	return false;
}

void check_action()
{

}