#include "tiles.hpp"
#include "definer.hpp"
#include "items.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "entities.hpp"
#include "window.hpp"
#include "renderer.hpp"
#include "input.hpp"

Vector2i tile_size = { 64, 48 };
vector<pair<Vector2i, Vector2i>> tickingTiles;

void Tile::draw()
{
	Sprite sprite(TextureManager::get(textureName));
	sprite.setPosition(Vector2f(tile_size.x * (chunk.x * 16 + subc.x) + break_offset + baseOffset.x, tile_size.y * (chunk.y * 16 + subc.y) + baseOffset.y));
	sprite.scale(Vector2f(2.f * baseScale.x, 2.f * baseScale.y));
	window->draw(sprite);
}

Rock::Rock()
{
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	baseOffset = Vector2f(0, tile_size.y - 64);
	textureName = "assets/passive/rock";
}

Rock::Rock(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	baseOffset = Vector2f(0, tile_size.y - 64);
	textureName = "assets/passive/rock";
}

void Rock::dropLoot()
{
	int min_loot = 300;
	int max_loot = 500;
	int loot_count = rand() % (max_loot - min_loot + 1) + min_loot;
	int lost = player.inventory.addItem("stone", loot_count);
	if (!lost)
		return;
	Vector2f pos = chunkSubcToPos(chunk, subc);
	string name = "stone";
	entities.push_back(make_unique<Item>(name, lost, pos));
}

Gutter::Gutter()
{
	textureName = "assets/passive/gutter";
	solid = false;
	breakable = true;
	durability = 10;
	break_offset = 0;
}

Gutter::Gutter(Vector2i sub, Vector2i chun)
{
	subc = sub;
	chunk = chun;
	textureName = "assets/passive/gutter";
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
	if (auto* a = dynamic_cast<Gutter*>(getTile(chunk, Vector2i(subc.x, subc.y - 1))))
		up = true;
	if (auto* a = dynamic_cast<Gutter*>(getTile(chunk, Vector2i(subc.x, subc.y + 1))))
		down = true;
	if (auto* a = dynamic_cast<Gutter*>(getTile(chunk, Vector2i(subc.x - 1, subc.y))))
		left = true;
	if (auto* a = dynamic_cast<Gutter*>(getTile(chunk, Vector2i(subc.x + 1, subc.y))))
		right = true;
	if (!right && !left && !up && !down)
	{
		textureName = "assets/passive/gutter/point";
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (!right && !left && !up && down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/down";
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && !left && up && !down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/up";
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && !left && up && down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/vertical";
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}

	if (!right && left && !up && !down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/left";
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (!right && left && !up && down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/down_left";
		center = Vector2f(tile_size.x / 4, tile_size.y * 3 / 4);
	}

	if (!right && left && up && !down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/up_left";
		center = Vector2f(tile_size.x / 4, tile_size.y / 4);
	}

	if (!right && left && up && down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/branch_vertical_right";
		center = Vector2f(tile_size.x / 6, tile_size.y / 2);
	}
	if (right && !left && !up && !down)
	{
		textureName = "gutter_left_textureassets/passive/gutter/left";
		baseScale = Vector2f(-1.f, 1.f);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (right && !left && !up && down)
	{
		textureName = "assets/passive/gutter/down_left";
		baseScale = Vector2f(-1.f, 1.f);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 3 / 4, tile_size.y * 3 / 4);
	}
	if (right && !left && up && !down)
	{
		textureName = "assets/passive/gutter/up_left";
		baseScale = Vector2f(-1.f, 1.f);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 3 / 4, tile_size.y / 4);
	}
	if (right && !left && up && down)
	{
		textureName = "assets/passive/gutter/branch_vertical_right";
		baseScale = Vector2f(-1.f, 1.f);
		baseOffset = Vector2f(tile_size.x, 0.f);
		center = Vector2f(tile_size.x * 5 / 6, tile_size.y / 2);
	}
	if (right && left && !up && !down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/horizontal";
		center = Vector2f(tile_size.x / 2, tile_size.y / 2);
	}
	if (right && left && !up && down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/branch_horizontal_up";
		center = Vector2f(tile_size.x / 2, tile_size.y * 5 / 6);
	}
	if (right && left && up && !down)
	{
		baseScale = Vector2f(1.f, 1.f);
		baseOffset = { 0.f, 0.f };
		textureName = "assets/passive/gutter/branch_horizontal_down";
		center = Vector2f(tile_size.x / 2, tile_size.y / 6);
	}
	if (right && left && up && down)
		delete_tile(getTile(chunk, subc));

}

void Container::interact()
{
	player.state = playerState::inventory;
	player.container = this;
}

Chest::Chest()
{
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	baseOffset = Vector2f(0, tile_size.y - 64);
	textureName = "assets/passive/chest";
}

Chest::Chest(Vector2i chunk, Vector2i subc)
{
	this->subc = subc;
	this->chunk = chunk;
	solid = true;
	breakable = true;
	durability = 45;
	break_offset = 0;
	baseOffset = Vector2f(0, tile_size.y - 64);
	textureName = "assets/passive/chest";
}

pair<string, int> Chest::removeItem(int quantity)
{
	int maxQuantity = quantity;
	string itemName = "";
	for (int i = 0; i < inventory.slots.size(); i++)
	{
		if (inventory.slots[i].quantity > 0)
		{
			if (inventory.slots[i].item != itemName && itemName != "")
				continue;
			itemName = inventory.slots[i].item;
			int to_remove = min(quantity, inventory.slots[i].quantity);
			inventory.slots[i].quantity -= to_remove;
			quantity -= to_remove;
			if (quantity == 0)
				return make_pair(itemName, maxQuantity);
		}
	}
	return make_pair(itemName, maxQuantity - quantity);
}

void Machine::update()
{	
	if (!running)
		return;
	if (craftName.empty()) return;
	const CraftDef& craft = CraftManager::get(craftName);
	if (progress == 0)
	{
		for (int i = 0; i < inputSlots.size(); i++)
		{
			auto req = craft.ingredients[i];
			if (inputSlots[i].item != req.first || inputSlots[i].quantity < req.second)
			{
				return;
			}	
		}
		for (int i = 0; i < inputSlots.size(); i++)
		{
			auto req = craft.ingredients[i];
			inputSlots[i].quantity -= req.second;
		}

	}
	progress += progressSpeed;
	if (progress < craft.duration)
		return;
	progress = 0;
	for (int i = 0; i < craft.results.size(); i++)
	{
		outputSlots[i].item = craft.results[i].first;
		outputSlots[i].quantity += craft.results[i].second;
	}
}

int Machine::addItem(string item, int quantity)
{
	for (int i = 0; i < allowedItems.size(); i++)
	{
		if (allowedItems[i] == item)
		{
			int maxQuantity;
			inputSlots[i].item = item;
			CraftDef craft = CraftManager::get(craftName);
			for (const auto& req : craft.ingredients)
			{
				if (req.first == item)
				{
					maxQuantity = 5 * req.second;
				}
			}
			int to_add = min(quantity, maxQuantity - inputSlots[i].quantity);
			inputSlots[i].quantity += to_add;
			quantity -= to_add;
			if (quantity == 0)
				return 0;
		}
	}
	return quantity;
}

int Machine::removeItem(string item, int quantity)
{
	int Maxquantity = quantity;
	for (int i = 0; i < outputSlots.size(); i++)
	{
		if (outputSlots[i].item == item)
		{
			int to_remove = min(quantity, outputSlots[i].quantity);
			outputSlots[i].quantity -= to_remove;
			quantity -= to_remove;
			if (quantity == 0)
				return Maxquantity;
		}
	}
	return Maxquantity - quantity;
}

pair<string, int> Machine::removeItem(int quantity)
{
	int maxQuantity = quantity;
	string itemName = "";
	for (int i = 0; i < outputSlots.size(); i++)
	{
		if (outputSlots[i].quantity > 0)
		{
			if (outputSlots[i].item != itemName && itemName != "")
				continue;
			itemName = outputSlots[i].item;
			int to_remove = min(quantity, outputSlots[i].quantity);
			outputSlots[i].quantity -= to_remove;
			quantity -= to_remove;
			if (quantity == 0)
				return make_pair(itemName, maxQuantity);
		}
	}
	return make_pair(itemName, maxQuantity - quantity);
}

void Machine::drawInterface(float x)
{
	RectangleShape background(Vector2f(window->getSize().x / 2, 36 * (((inputSlots.size() + maxWidth - 1) / maxWidth) + 5) * guiScale));
	background.setFillColor(Color(200, 200, 200));
	background.setPosition({ x, view_offset.y - 36 * 3 * guiScale });
	window->draw(background);
	Text titleText(font, craftName.empty() ? "Select Craft" : craftName, 16 * guiScale);
	titleText.setPosition(Vector2f(x, view_offset.y - 40 * guiScale));
	window->draw(titleText);
	for (int i = 0; i < inputSlots.size(); i++)
	{
		inputSlots[i].draw(x + 36 * (i % maxWidth) * guiScale, view_offset.y + int(i / maxWidth) * 36 , true);
	}
	for (int i = 0; i < outputSlots.size(); i++)
	{
		outputSlots[i].draw(x + (window->getSize().x / 2) - 36 * (i % maxWidth) * guiScale, view_offset.y + int(i / maxWidth) * 36, true);
	}
	Sprite selectCraftSprite(TextureManager::get("placeHolder"));
	selectCraftSprite.setPosition(Vector2f(x, view_offset.y - 40 * guiScale));
	window->draw(selectCraftSprite);
	if (MouseInputManager::isActionTapped("break") && selectCraftSprite.getGlobalBounds().contains(Vector2f(MouseInputManager::globalPosition)))
	{
		CraftSelector::open = true;
		CraftSelector::machine = this;
	}
	if (player.itemOnCursor.second != 0)
	{
		auto item = player.itemOnCursor;
		player.itemOnCursor.second = addItem(item.first, item.second);
	}

	if (craftName.empty())
		return;
	Text progressText(font, craftName + " " + to_string(int(progress / CraftManager::get(craftName).duration * 100)) + "%", 12 * guiScale);
	progressText.setPosition(Vector2f(x, view_offset.y - 20 * guiScale));
	window->draw(progressText);
	Sprite selectedCraftSprite(TextureManager::get(CraftManager::get(craftName).textureName));
	selectedCraftSprite.setPosition(Vector2f(x, view_offset.y - 40 * guiScale));
	window->draw(selectedCraftSprite);
	
}

void Machine::setCraft(string craftName)
{
	this->craftName = craftName;
	auto craft = CraftManager::get(craftName);
	inputSlots.clear();
	outputSlots.clear();
	allowedItems.clear();
	for (const auto& req : craft.ingredients)
	{
		InventorySlot slot;
		slot.item = req.first;
		slot.quantity = 0;
		inputSlots.push_back(slot);
		allowedItems.push_back(req.first);
	}
	for (const auto& res : craft.results)
	{
		InventorySlot slot;
		slot.item = res.first;
		slot.quantity = 0;
		outputSlots.push_back(slot);
	}
}