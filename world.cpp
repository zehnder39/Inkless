#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "header.hpp"


Player::Player(float set_x, float set_y)
{
	position.x = set_x;
	position.y = set_y;
}
void Player::move(float dx, float dy)
{
	float norm = 1;
	if (dx != 0 and dy != 0)
		norm = sqrtf(dx / dx + dy / dy);
	position.x += dx / norm;
	position.y += dy / norm;
}

void create_world()
{
	player_model.setFillColor(Color::Red);
	entity1.setFillColor(Color::Green);
	entities.push_back(entity1);
	if (!ground_texture.loadFromFile("ground.png", false, IntRect({ 0, 0 }, { 32, 32 }))) {
		cout << "Failed to load ground.png!" << endl;
	}
	ground_texture.setRepeated(true);
	ground.setTexture(&ground_texture);
	ground.setTextureRect(IntRect({ 0, 0 }, { 128, 128 }));
}

void update_world()
{
	
}
