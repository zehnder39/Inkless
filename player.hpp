#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "items.hpp"
#include "physics.hpp"

using namespace std;
using namespace sf;

enum playerState
{
	normal,
	swimming,
	inventory
};

class Player
{
public:
	Vector2f position = {};
	Vector2i chunk = {};
	Vector2i subc = {};
	float base_speed = 3.5f;
	float speed;
	float baseSwimSpeed = 8.0f;
	int animation_state = 0;
	float animation_offset = 0;
	Sprite model;
	playerState state = normal;
	Container* container = nullptr;
	pair<string, int> itemOnCursor = { "", 0 };

	GutterPathing gutterPathing;

	Inventory inventory{56, 8, string("Inventory")};

	void updateChunkSubc();
	void swim();
	void walk();
	void walk(Vector2f movement);
	void move(Vector2f movement);
	void walkAnimation(bool moving, float dx);

	Player(float x, float y, Texture& texture)
		: position(x, y), model(texture)
	{
		model.setPosition(position);
	}
};
extern Player player;