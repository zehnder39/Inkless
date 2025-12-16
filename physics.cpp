#include "definer.hpp"
#include "physics.hpp"
#include "world.hpp"
#include "input.hpp"
#include "renderer.hpp"

#include <iostream>

bool swimmingAdvUp, swimmingAdvDown, swimmingAdvLeft, swimmingAdvRight;

pair<Vector2f, Vector2f> swimmingPath ;






bool playerOutOffSwimmingPath()
{
	if ((player.position.x >= swimmingPath.first.x && player.position.x >= swimmingPath.second.x) || (player.position.x <= swimmingPath.first.x && player.position.x <= swimmingPath.second.x))
		return true;
	if ((player.position.y >= swimmingPath.first.y && player.position.y >= swimmingPath.second.y) || (player.position.y <= swimmingPath.first.y && player.position.y <= swimmingPath.second.y))
		return true;
}

pair<Tile*, bool> nextSwimmingTile()
{
	Vector2f delta = (swimmingPath.first - swimmingPath.second);
	float norm = sqrt(delta.x * delta.x + delta.y * delta.y);
	if (norm != 0)
		delta *= 10 / norm;
	else
		delta = { 0, 0 };
	auto chunk_subc = pos_to_chunk_subc(player.position + delta);
	auto tile = get_tile(chunk_subc.first.first, chunk_subc.first.second);
	bool gut = dynamic_cast<Gutter*>(tile);
	return make_pair(tile, gut);

}

void updateSwimmingPath(Gutter *gutter)
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
	Vector2f gutterPos = chunk_subc_to_pos(gutter->chunk, gutter->subc);

	auto next = nextSwimmingTile();
	if (!next.second)
	{
		player.swimming = false;
		Vector2f delta = (swimmingPath.first - swimmingPath.second);
		float norm = sqrt(delta.x * delta.x + delta.y * delta.y);
		delta *= player.base_speed / norm;
		player.walk(delta);
		cout << "next tile is not gutter" << endl;
		return;
	}

	Gutter* nextGutter = dynamic_cast<Gutter*>(next.first);
	gutterPos = chunk_subc_to_pos(nextGutter->chunk, nextGutter->subc);
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
			player.swimming = false;
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
	player.animation_offset = 0;
	player.animation_state = 0;
	if (moving_right && !moving_left)
	{
		swimmingAdvRight = true;
		swimmingAdvLeft = false;
	}
	if (moving_left && !moving_right)
	{
		swimmingAdvRight = false;
		swimmingAdvLeft = true;
	}
	if (moving_up && !moving_down)
	{
		swimmingAdvUp = true;
		swimmingAdvDown = false;
	}
	if (moving_down && !moving_up)
	{
		swimmingAdvUp = false;
		swimmingAdvDown = true;
	}
	debug_text.push_back("Swimming right advantage: " + to_string(swimmingAdvRight));
	debug_text.push_back("Swimming left advantage: " + to_string(swimmingAdvLeft));
	debug_text.push_back("Swimming up advantage: " + to_string(swimmingAdvUp));
	debug_text.push_back("Swimming down advantage: " + to_string(swimmingAdvDown));
	Tile* tile = get_tile(player.chunk, player.subc);
	if (auto* gutter = dynamic_cast<Gutter*>(tile))
	{
		Vector2f speed;
		updateSwimmingPath(gutter);
		Vector2f pathDir = swimmingPath.first - player.position;
		pathDir *= 1 / sqrt(pathDir.x * pathDir.x + pathDir.y * pathDir.y);
		// Set swimming advantage
		if (pathDir.x < 0)
		{
			swimmingAdvLeft = true;
			swimmingAdvRight = false;
		}
		else if (pathDir.x > 0)
		{
			swimmingAdvRight = true;
			swimmingAdvLeft = false;
		}
		if (pathDir.y < 0)
		{
			swimmingAdvUp = true;
			swimmingAdvDown = false;
		}
		else if (pathDir.y > 0)
		{
			swimmingAdvDown = true;
			swimmingAdvUp = false;
		}
		// Reverse swimming direction;
		if ((swimmingAdvLeft && pathDir.x > 0) || (swimmingAdvRight && pathDir.x < 0) || (swimmingAdvUp && pathDir.y > 0) || (swimmingAdvDown && pathDir.y < 0))
		{
			auto temp = swimmingPath.first;
			swimmingPath.first = swimmingPath.second;
			swimmingPath.second = temp;
			cout << "reversing swimming direction" << endl;
		}
		// Move towards on swimmingPath objective
		if (pathDir.x != 0)
			speed.x = pathDir.x * player.baseSwimSpeed;
		if (pathDir.y != 0)
			speed.y = pathDir.y * player.baseSwimSpeed;
		player.swim(speed);
		debug_text.push_back("swimming speed: " + std::to_string(speed.x) + ", " + std::to_string(speed.y));
	}
	else
		player.swimming = false;
	CircleShape cir(3.f);
	cir.setFillColor(Color::Yellow);
	cir.setPosition(swimmingPath.second);
	debug_draw.push_back(cir);
	cir.setFillColor(Color::Magenta);
	cir.setPosition(swimmingPath.first);
	debug_draw.push_back(cir);
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

void Player::walk(Vector2f movement)
{
	move(movement);
	walkAnimation(movement.x != 0 or movement.y != 0, movement.x);
}

void Player::swim(Vector2f movement)
{
	move(movement);
}

void Player::move(Vector2f movement)
{
	if (movement.x > 0)
		facing_left = false;
	else if (movement.x < 0)
		facing_left = true;
	float norm = 1;
	if (movement.x != 0 and movement.y != 0)
		norm = sqrtf(movement.x / movement.x + movement.y / movement.y);
	if (check_move(movement.x, 0))
		position.x += movement.x * tile_size.x / (norm * 64);
	if (check_move(0, movement.y))
		position.y += movement.y * tile_size.y / (norm * 64);
	updateChunkSubc();
}
