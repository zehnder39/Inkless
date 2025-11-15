#include "header.hpp"

#include <iostream>

bool swimmingUp, swimmingDown, swimmingLeft, swimmingRight;


void tile_interaction()
{
	auto mouse_tile = pos_to_chunk_subc(Vector2f(mouse_pos));
	if (!mouse_tile.second)
		return;
	if (mouse_tile.first == tile_looking_at().first)
	{
		Tile* tile = get_tile(mouse_tile.first.first, mouse_tile.first.second);
		if (tile)
			tile->interact();
	}
}

void playerWalk()
{
	player.speed = player.base_speed;
	Vector2f speed = {};
	if (moving_left) {
		speed.x += -player.speed;
	}
	if (moving_right) {
		speed.x += player.speed;
	}
	if (moving_up)
		speed.y += -player.speed;
	if (moving_down)
		speed.y += player.speed;
	player.walk(speed);
}

void playerSwim()
{
	if (moving_right)
	{
		swimmingRight = true;
		swimmingLeft = false;
	}
	if (moving_left)
	{
		swimmingRight = false;
		swimmingLeft = true;
	}
	if (moving_up)
	{
		swimmingUp = true;
		swimmingDown = false;
	}
	if (moving_down)
	{
		swimmingUp = false;
		swimmingDown = true;
	}
	Tile* tile = get_tile(player.chunk, player.subc);
	if (auto* gutter = dynamic_cast<Gutter*>(tile))
	{
		Vector2f speed;
		// Set swimming speed based on gutter directions
		if (gutter->up && swimmingUp || (!gutter->left && !gutter->up && gutter->down && !gutter->right))
			speed.y = -player.swim_speed;
		if (gutter->down && swimmingDown || (!gutter->left && gutter->up && !gutter->down && !gutter->right))
			speed.y = player.swim_speed;
		if (gutter->right && swimmingRight || (!gutter->up && !gutter->down && !gutter->right && gutter->left))
			speed.x = player.swim_speed;
		if (gutter->left && swimmingLeft || (!gutter->up && !gutter->down && gutter->right && !gutter->left))
			speed.x = -player.swim_speed;
		// Set swimming direction based on gutter direction
		if (gutter->up && !gutter->down && !swimmingDown)
		{
			swimmingUp = true;
			swimmingDown = false;
		}
		if (gutter->down && !gutter->up && !swimmingUp)
		{
			swimmingUp = false;
			swimmingDown = true;
		}
		if (gutter->right && !gutter->left && !swimmingLeft)
		{
			swimmingRight = true;
			swimmingLeft = false;
		}
		if (gutter->left && !gutter->right && !swimmingRight)
		{
			swimmingRight = false;
			swimmingLeft = true;
		}
		player.swim(speed);
		debug_text.push_back("swim speed x: " + to_string(speed.x) + " y: " + to_string(speed.y));
	}
	else
		player.swimming = false;
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

void playerMovement()
{
	if (!player.swimming)
		playerWalk();
	else
		playerSwim();
}