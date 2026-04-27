#include "player.hpp"
#include "definer.hpp"
#include "texture.hpp"
#include "input.hpp"
#include "world.hpp"
#include "renderer.hpp"

Player player(960.f, 540.f, placeHolderTexture);

void Player::updateChunkSubc()
{
	auto chunkSubc = pos_to_chunk_subc(player.position);
	chunk = chunkSubc.first.first;
	subc = chunkSubc.first.second;
}

void Player::walk()
{
	player.speed = player.base_speed;
	Vector2f speed = {};
	if (KeyInputManager::isActionActive("move_left")) {
		speed.x += -player.speed;
	}
	if (KeyInputManager::isActionActive("move_right")) {
		speed.x += player.speed;
	}
	if (KeyInputManager::isActionActive("move_up"))
		speed.y += -player.speed;
	if (KeyInputManager::isActionActive("move_down"))
		speed.y += player.speed;
	move(speed);
	walkAnimation(speed.x != 0 or speed.y != 0, speed.x);
}

void Player::walk(Vector2f movement)
{
	this->speed = this->base_speed;
	move(movement);
	walkAnimation(movement.x != 0 or movement.y != 0, movement.x);
}

void Player::swim()
{
	animation_offset = 0;
	animation_state = 0;
	if (KeyInputManager::isActionActive("move_right") && !KeyInputManager::isActionActive("move_left"))
	{
		gutterPathing.swimmingAdvRight = true;
		gutterPathing.swimmingAdvLeft = false;
	}
	if (KeyInputManager::isActionActive("move_left") && !KeyInputManager::isActionActive("move_right"))
	{
		gutterPathing.swimmingAdvRight = false;
		gutterPathing.swimmingAdvLeft = true;
	}
	if (KeyInputManager::isActionActive("move_up") && !KeyInputManager::isActionActive("move_down"))
	{
		gutterPathing.swimmingAdvUp = true;
		gutterPathing.swimmingAdvDown = false;
	}
	if (KeyInputManager::isActionActive("move_down") && !KeyInputManager::isActionActive("move_up"))
	{
		gutterPathing.swimmingAdvUp = false;
		gutterPathing.swimmingAdvDown = true;
	}
	debug_text.push_back("Swimming right advantage: " + to_string(gutterPathing.swimmingAdvRight));
	debug_text.push_back("Swimming left advantage: " + to_string(gutterPathing.swimmingAdvLeft));
	debug_text.push_back("Swimming up advantage: " + to_string(gutterPathing.swimmingAdvUp));
	debug_text.push_back("Swimming down advantage: " + to_string(gutterPathing.swimmingAdvDown));
	Tile* tile = getTile(player.chunk, player.subc);
	if (auto* gutter = dynamic_cast<Gutter*>(tile))
	{
		Vector2f speed;
		gutterPathing.updateSwimmingPath(gutter);
		Vector2f pathDir = gutterPathing.swimmingPath.first - position;
		pathDir *= 1 / sqrt(pathDir.x * pathDir.x + pathDir.y * pathDir.y);
		// Set swimming advantage
		if (pathDir.x < 0)
		{
			gutterPathing.swimmingAdvLeft = true;
			gutterPathing.swimmingAdvRight = false;
		}
		else if (pathDir.x > 0)
		{
			gutterPathing.swimmingAdvRight = true;
			gutterPathing.swimmingAdvLeft = false;
		}
		if (pathDir.y < 0)
		{
			gutterPathing.swimmingAdvUp = true;
			gutterPathing.swimmingAdvDown = false;
		}
		else if (pathDir.y > 0)
		{
			gutterPathing.swimmingAdvDown = true;
			gutterPathing.swimmingAdvUp = false;
		}
		// Reverse swimming direction;
		if ((gutterPathing.swimmingAdvLeft && pathDir.x > 0) || (gutterPathing.swimmingAdvRight && pathDir.x < 0) || (gutterPathing.swimmingAdvUp && pathDir.y > 0) || (gutterPathing.swimmingAdvDown && pathDir.y < 0))
		{
			auto temp = gutterPathing.swimmingPath.first;
			gutterPathing.swimmingPath.first = gutterPathing.swimmingPath.second;
			gutterPathing.swimmingPath.second = temp;
		}
		// Move towards on swimmingPath objective
		if (pathDir.x != 0)
			speed.x = pathDir.x * baseSwimSpeed;
		if (pathDir.y != 0)
			speed.y = pathDir.y * baseSwimSpeed;
		move(speed);
		debug_text.push_back("swimming speed: " + std::to_string(speed.x) + ", " + std::to_string(speed.y));
	}
	else
		player.state = playerState::normal;
	CircleShape cir(3.f);
	cir.setFillColor(Color::Yellow);
	cir.setPosition(gutterPathing.swimmingPath.second);
	debug_draw.push_back(cir);
	cir.setFillColor(Color::Magenta);
	cir.setPosition(gutterPathing.swimmingPath.first);
	debug_draw.push_back(cir);
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

void Player::walkAnimation(bool moving, float dx)
{
	int animation_time = 25;
	int animation_angle = 30;
	if (animation_state != 0)
	{
		model.setTexture(TextureManager::get("player/jumping"));
		int mod = 1;
		if (facing_left)
			mod = -1;
		model.setRotation(degrees(animation_angle * mod * (animation_time - animation_state) * 2 / animation_time));
		model.setScale({ mod * 2.f, 2.f });
		animation_offset = 3 * (fabsf(animation_state - animation_time / 2) - animation_time / 2);
		animation_state--;
	}
	if (animation_state == 0)
	{
		model.setRotation(degrees(0));
		if (moving)
			animation_state = animation_time;
		animation_offset = 0;
		model.setTexture(TextureManager::get("player/standing"));
	}
}