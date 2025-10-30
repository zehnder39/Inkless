#include "header.hpp"
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <iostream>


void update_surroundings(Vector2i chunk, Vector2i subc)
{
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			Vector2i nsubc = subc;
			Vector2i nchunk = chunk;
			nsubc += Vector2i(x, y);
			Tile* tile = get_tile(nchunk, nsubc);
			if (tile == nullptr)
				continue;
			tile->update();
		}
	}
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

pair<pair<Vector2i, Vector2i>, bool> tile_looking_at()
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

Tile* get_tile(Vector2i chunk, Vector2i subc)
{
	auto chunk_subc = inRange(chunk, subc);
	if (chunk_subc.second)
	{
		Tile* tile = world_chunks[chunk_subc.first.first.x][chunk_subc.first.first.y].changeables[chunk_subc.first.second.x][chunk_subc.first.second.y].get();
		return tile;
	}
	return nullptr;
}

Player::Player(float set_x, float set_y)
{
	position.x = set_x;
	position.y = set_y;
}
void Player::move(float dx, float dy)
{
	if (dx > 0)
		facing_left = false;
	else if (dx < 0)
		facing_left = true;
	float norm = 1;
	if (dx != 0 and dy != 0)
		norm = sqrtf(dx / dx + dy / dy);
	if (check_move(dx, 0))
		position.x += dx / norm;
	if (check_move(0, dy))
		position.y += dy / norm;
	chunk = pos_to_chunk_subc(position).first.first;
	subc = pos_to_chunk_subc(position).first.second;
	jump(dx != 0 or dy != 0, dx);
}

void Player::jump(bool moving, float dx)
{
	int animation_time = 25;
	int jump_angle = 30;
	if (jump_state != 0)
	{	
		player_model.setTexture(jump_texture);
		int mod = 1;
		if (facing_left)
			mod = -1;
		player_model.setRotation(degrees(jump_angle * mod * (animation_time - jump_state) * 2 / animation_time));
		player_model.setScale({ mod * 2.f, 2.f });
		jump_offset = 3 * (fabsf(jump_state - animation_time / 2) - animation_time / 2);
		jump_state--;
	}
	if (jump_state == 0)
	{
		player_model.setRotation(degrees(0));
		if (moving)
			jump_state = animation_time;
		jump_offset = 0;
		player_model.setTexture(player_texture);
	}
}

Rock::Rock(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	solid = true;
	breakable = true;
	durability = 120;
	break_offset = 0;
	texture = rock_texture;
}

Gutter::Gutter(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	solid = false;
	breakable = true;
	durability = 10;
	break_offset = 0;
}

void Gutter::update()
{
	bool callBreak = false;
	up = false;
	down = false;
	left = false;
	right = false;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x, subc.y - 1))))
		up = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x, subc.y + 1))))
		down = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x - 1, subc.y))))
		left = true;
	if (auto* a = dynamic_cast<Gutter*>(get_tile(chunk, Vector2i(subc.x + 1, subc.y))))
		right = true;
	if (!right && !left && !up && !down)
	{
		texture = gutter_point_texture;
		baseScale = Vector2f(float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
	}
	if (!right && !left && !up && down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_down_texture;
	}
		
	if (!right && !left && up && !down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_up_texture;
	}
		
	if (!right && !left && up && down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_vertical_texture;
	}
		
	if (!right && left && !up && !down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_left_texture;
	}
	if (!right && left && !up && down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_down_left_texture;
	}
		
	if (!right && left && up && !down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_up_left_texture;
	}
		
	if (!right && left && up && down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_vertical_texture;
	}
	if (right && !left && !up && !down)
	{
		texture = gutter_left_texture;
		baseScale = Vector2f( - float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f );
	}
	if (right && !left && !up && down)
	{
		texture = gutter_down_left_texture;
		baseScale = Vector2f( - float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f(tile_size.x, 0.f );
	}
	if (right && !left && up && !down)
	{
		texture = gutter_up_left_texture;
		baseScale = Vector2f( - float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = Vector2f( tile_size.x, 0.f );
	}
	if (right && !left && up && down)
	{
		texture = gutter_branch_vertical_texture;
		baseScale = Vector2f( - float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset =  Vector2f(tile_size.x, 0.f);
	}
	if (right && left && !up && !down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_horizontal_texture;
	}
	if (right && left && !up && down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_horizontal_texture;
	}
	if (right && left && up && !down)
	{
		baseScale = Vector2f( float(tile_size.x) / 64, float(tile_size.y) / 64);
		baseOffset = { 0.f, 0.f };
		texture = gutter_branch_horizontal_down_texture;
	}
	if (right && left && up && down)
		delete_tile(get_tile(chunk, subc));

}

void Gutter::interact()
{

}

void Tile::draw()
{
	Sprite sprite(texture);
	sprite.setPosition(Vector2f( tile_size.x * (chunk.x * 16 + subc.x) + break_offset + baseOffset.x, tile_size.y * (chunk.y * 16 + subc.y) + baseOffset.y ));
	sprite.scale(Vector2f( 2.f * baseScale.x, 2.f * baseScale.y ));
	window->draw(sprite);
}

Chunk::Chunk(Vector2i poss)
{
	position = poss;
	ground.setSize(Vector2f( 16 * tile_size.x, 16 * tile_size.y));
	ground.setPosition(Vector2f((tile_size.x * 16) * position.x, (tile_size.y * 16) * position.y));
	ground.setTexture(&ground_texture);
	ground.setTextureRect(IntRect({ 0, 0 }, { 384, 512 }));
}
vector<Tile*> Chunk::list_tiles()
{
	vector<Tile*> in_chunk;
	for (int x = 0; x < 16; x ++)
	{
		for (int y = 0; y < 16; y++)
		{
			if (changeables[x][y] != nullptr)
				in_chunk.push_back(changeables[x][y].get());
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
			int num = rand() % 8;
			if (num == 0)
				changeables[x][y] = make_unique<Rock>(Vector2i(x,y), Vector2i(position.x, position.y));
		}
	}
}
