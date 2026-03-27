#include "definer.hpp"
#include "physics.hpp"
#include "world.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "player.hpp"

#include <iostream>

bool swimmingAdvUp, swimmingAdvDown, swimmingAdvLeft, swimmingAdvRight;

//flags
bool gamePaused = false;

pair<Vector2f, Vector2f> swimmingPath;



bool GutterPathing::playerOutOffSwimmingPath()
{
	if ((player.position.x >= swimmingPath.first.x && player.position.x >= swimmingPath.second.x) || (player.position.x <= swimmingPath.first.x && player.position.x <= swimmingPath.second.x))
		return true;
	if ((player.position.y >= swimmingPath.first.y && player.position.y >= swimmingPath.second.y) || (player.position.y <= swimmingPath.first.y && player.position.y <= swimmingPath.second.y))
		return true;
}

pair<Tile*, bool> GutterPathing::nextSwimmingTile()
{
	Vector2f delta = (swimmingPath.first - swimmingPath.second);
	float norm = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (norm != 0)
		delta *= 10 / norm;
	else
		delta = { 0, 0 };
	auto chunk_subc = pos_to_chunk_subc(player.position + delta);
	auto tile = getTile(chunk_subc.first.first, chunk_subc.first.second);
	bool gut = dynamic_cast<Gutter*>(tile);
	return make_pair(tile, gut);

}

void GutterPathing::updateSwimmingPath(Gutter *gutter)
{
	if (!playerOutOffSwimmingPath())
	{
		debug_text.push_back("player in swimming path");
		return;
	}
	if (dist(player.position, swimmingPath.first) > 3.f)
	{
		return;
	}
	auto mem = swimmingPath.first;
	Vector2f gutterPos = chunkSubcToPos(gutter->chunk, gutter->subc);

	auto next = nextSwimmingTile();
	if (!next.second)
	{
		player.state = playerState::normal;
		Vector2f delta = (swimmingPath.first - swimmingPath.second);
		float norm = sqrt(delta.x * delta.x + delta.y * delta.y);
		delta *= player.base_speed / norm;
		player.walk(delta);
		cout << "next tile is not gutter" << endl;
		return;
	}

	Gutter* nextGutter = dynamic_cast<Gutter*>(next.first);
	gutterPos = chunkSubcToPos(nextGutter->chunk, nextGutter->subc);
	if (swimmingPath.first == nextGutter->center + gutterPos) //player at centre of tile
	{
		int numberDir = 0;
		if (gutter->up)
			numberDir += 1;
		if (gutter->down)
			numberDir += 1;
		if (gutter->left)
			numberDir += 1;
		if (gutter->right)
			numberDir += 1;
		if (numberDir == 1 && ((gutter->up && swimmingAdvDown) || (gutter->down && swimmingAdvUp) || (gutter->left && swimmingAdvRight) || (gutter->right && swimmingAdvLeft)))
		{
			player.state = playerState::normal;
			player.walk({ 0,0 });
			cout << "cul de sac + at center of tile + going to cds" << endl;
		}
		debug_text.push_back("at centre of tile");
		if (gutter->up && swimmingAdvUp)
		{
			swimmingPath.first = Vector2f(gutterPos.x + tile_size.x / 2, gutterPos.y);
		}
		if (gutter->down && swimmingAdvDown)
		{
			swimmingPath.first = Vector2f(gutterPos.x + tile_size.x / 2, gutterPos.y + tile_size.y);
		}
		if (gutter->right && swimmingAdvRight)
		{
			swimmingPath.first = Vector2f(gutterPos.x + tile_size.x, gutterPos.y + tile_size.y / 2);
		}
		if (gutter->left && swimmingAdvLeft)
		{
			swimmingPath.first = Vector2f(gutterPos.x, gutterPos.y + tile_size.y / 2);
		}
	}
	else
	{
		
		swimmingPath.first = nextGutter->center + gutterPos;
	}

	if (mem != swimmingPath.first)
	{
		swimmingPath.second = mem;
	}
}

void tile_interaction()
{
	auto mouse_tile = pos_to_chunk_subc(Vector2f(mouse_pos));
	if (!mouse_tile.second)
		return;
	if (mouse_tile.first == tileLookingAt().first)
	{
		Tile* tile = getTile(mouse_tile.first.first, mouse_tile.first.second);
		if (tile)
			tile->interact();
	}
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
	if (KeyInputManager::isActionTapped("use"))
	{
		if (player.state == playerState::swimming)
		{
			player.state = playerState::normal;
			return;
		}

		tile_interaction();
	}
	if (KeyInputManager::isActionTapped("debug"))
	{
		debug_visual = !debug_visual;
	}
	if (KeyInputManager::isActionTapped("inventory"))
	{
		if (player.state == playerState::inventory)
			player.state = playerState::normal;
		else
			player.state = playerState::inventory;
	}
}

void playerMovement()
{
	if (player.state == playerState::inventory)
		return;
	if (player.state == playerState::normal)
		player.walk();
	else
		player.swim();
}