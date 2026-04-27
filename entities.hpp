#pragma once

#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Entity
{
public:
	Vector2f position;
	string textureName;

	virtual void interact() = 0;
	virtual void update() = 0;
	virtual void draw();

	virtual ~Entity() = default;
};

class Item : public Entity {
public:
	string name;
	int quantity = 1;

	void interact() override {}
	void update() override;

	Item(string name, int quantity, Vector2f pos);
};

extern vector<unique_ptr<Entity>> entities;
