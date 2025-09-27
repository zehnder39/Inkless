#include "header.hpp"

#include <iostream>

int break_time = 0;
Tile* breaking_mem;

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
	Vector2i subc = { static_cast<int>((static_cast<int>(nx) % 1024) / 64), static_cast<int>((static_cast<int>(ny) % 1024) / 64) };
	Vector2i chunk = { static_cast<int>(floor(nx / 1024)), static_cast<int>(floor(ny / 1024)) };
	if (0 <= chunk.x and chunk.x < world_chunks.size())
	{
		if (0 <= chunk.y and chunk.y < world_chunks[chunk.x].size())
		{
			auto chunk_to_check = world_chunks[chunk.x][chunk.y];
			if (chunk_to_check.changeables[subc.x][subc.y] == NULL)
				return true;
		}
	}
	return false;
}

bool check_breaking()
{
	if (break_key)
	{
		int dx = 64;
		if (facing_left)
			dx = -64;
		int dy = 64;
		if (facing_up)
			dy = -64;
		Vector2i chunk_poss = { static_cast<int>(floor((player.position.x + dx) / 1024)), static_cast<int>(floor((player.position.y + dy )/ 1024)) };
		Vector2i sub_c = { static_cast<int>((static_cast<int>(player.position.x + dx) % 1024) / 64), static_cast<int>((static_cast<int>(player.position.y + dy) % 1024) / 64) };
		if (0 <= chunk_poss.x < world_chunks.size() and 0 <= chunk_poss.y < world_chunks[chunk_poss.x].size())
		{
			Chunk chunk = world_chunks[chunk_poss.x][chunk_poss.y];
			Tile* tile = chunk.changeables[sub_c.x][sub_c.y];
			CircleShape cir(30.f);
			cir.setFillColor(Color::Red);
			cir.setPosition(Vector2f(chunk_poss.x * 1024 + sub_c.x * 64, chunk_poss.y * 1024 + sub_c.y * 64));
			debug_draw.push_back(cir);
			if (tile != nullptr)
			{
				if (tile != breaking_mem)
				{
					break_time = 0;
					breaking_mem = tile;
					cout << "changed tile" << endl;
				}
				else if (break_time >= tile->durability)
				{
					tile->destroy();
					cout << "destroyed tile" << endl;
				}
				else
					return true;
			}
			else
				cout << "tile not there" << endl;
		}
	}
	return false;
}

void check_action()
{
	if (!check_breaking())
		break_time = 0;
	else
		break_time++;
}