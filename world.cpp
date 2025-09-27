#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cmath>
#include <algorithm>
#include <iostream>

#include "header.hpp"



void create_world()
{
	player_model.setOrigin({ 16.f, 16.f });
	entity1.setFillColor(Color::Green);
	entities.push_back(entity1);
	if (!ground_texture.loadFromFile("ground2.png", false, IntRect({ 0, 0 }, { 64, 64 })))
		cout << "Failed to load ground.png!" << endl;
	ground_texture.setRepeated(true);
	if (!player_texture.loadFromFile("squid_player.png", false, IntRect({ 0, 0 }, { 32, 32 })))
		cout << "Failed to load player.png!" << endl;
	if (!jump_texture.loadFromFile("squid_jump.png", false, IntRect({ 0, 0 }, { 32, 32 })))
		cout << "Failed to load squid_jump.png!" << endl;
	player_model.setTexture(player_texture);
	player_model.setTextureRect(IntRect({ 0, 0 }, { 32, 32 }));
	player_model.setScale({ 2.f, 2.f });
	if (!rock_texture.loadFromFile("rock.png", false, IntRect({ 0, 0 }, { 16, 16 })))
		cout << "Failed to load rock.png!" << endl;

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			Chunk c({ x ,y });
			if (world_chunks.size() < x)
				world_chunks.push_back({});
			world_chunks[x].push_back(c);
			world_chunks[x][y].rockdom(rock_texture);
		}
	}
	
	
}

void update_world()
{
	
}
