#include "entities.hpp"
#include "texture.hpp"
#include "window.hpp"
#include "items.hpp"
#include "definer.hpp"
#include "player.hpp"
#include "world.hpp"

vector<unique_ptr<Entity>> entities;

void Entity::draw()
{
	Sprite sprite(TextureManager::get(textureName));
	sprite.setPosition(position);
	window->draw(sprite);
}

Item::Item(string name, int quantity, Vector2f pos)
{
	this->name = name;
	this->quantity = quantity;
	this->position = pos;
	this->textureName = ItemManager::get(name).textureName;
}

void Item::update()
{
	if (dist(position, player.position) < 32.f)
	{
		quantity = player.inventory.addItem(name, quantity);
		if (quantity == 0)
		{
			deleteEntity(this);
		}
	}
}
