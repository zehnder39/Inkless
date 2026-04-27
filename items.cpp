#include "items.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"
#include "player.hpp"
#include "input.hpp"

unordered_map<string, ItemDef> ItemManager::itemMap;

void ItemManager::registerItem(string name, ItemDef def) {
	itemMap[name] = def;
}

const ItemDef& ItemManager::get(string name) {
	return itemMap.at(name);
}

void InventorySlot::draw(float x, float y)
{
	position = Vector2f(x, y);
	Color color1 = Color(100, 100, 100);
	Color color2 = Color(50, 50, 50);
	RectangleShape bg({ 32.f * guiScale, 32.f * guiScale });
	bg.setPosition({ x, y });
	bg.setFillColor(hovered ? color1 : color2);
	window->draw(bg);
	if (quantity == 0)
		return;
	Sprite sprite(TextureManager::get(ItemManager::get(item).textureName));
	sprite.scale({ (float)guiScale, (float)guiScale });
	sprite.setPosition({ x, y });
	window->draw(sprite);

	Text quantityText(font, to_string(quantity), 12 * guiScale);
	quantityText.setPosition({ x + 4, y + 4 });
	window->draw(quantityText);
}

void InventorySlot::draw(float x, float y, bool transparent)
{
	position = Vector2f(x, y);
	Color color1 = Color(100, 100, 100);
	Color color2 = Color(50, 50, 50);
	RectangleShape bg({ 32.f * guiScale, 32.f * guiScale });
	bg.setPosition({ x, y });
	bg.setFillColor(hovered ? color1 : color2);
	window->draw(bg);
	if (item == "")
		return;
	Sprite sprite(TextureManager::get(ItemManager::get(item).textureName));
	if (quantity == 0 && transparent)
		sprite.setColor(Color(255, 255, 255, 100));
	sprite.scale({ (float)guiScale, (float)guiScale });
	sprite.setPosition({ x, y });
	window->draw(sprite);

	Text quantityText(font, to_string(quantity), 12 * guiScale);
	quantityText.setPosition({ x + 4, y + 4 });
	window->draw(quantityText);
}

void InventorySlot::update()
{
	hovered = false;
	float width = 32.f * guiScale;
	float height = 32.f * guiScale;
	if (MouseInputManager::globalPosition.x < position.x || MouseInputManager::globalPosition.x > position.x + width || MouseInputManager::globalPosition.y < position.y || MouseInputManager::globalPosition.y > position.y + height)
		return;
	hovered = true;
	if (MouseInputManager::isActionTapped("break"))
	{
		auto temp = make_pair(item, quantity);
		item = player.itemOnCursor.first;
		quantity = player.itemOnCursor.second;
		player.itemOnCursor = temp;
	}
}

int Inventory::getWidth() const {
	int totalWidth = (width * 32 + (width + 1) * 4) * guiScale;
	return totalWidth;
}

int Inventory::getHeight() const {
	int rows = (maxSlots + width - 1) / width; // Calculate the number of rows needed
	int totalHeight = ((rows + 2) * 32 + (rows + 1) * 4) * guiScale;
	return totalHeight;
}

void Inventory::draw(float x)
{
	int totalWidth = getWidth();
	int totalHeight = getHeight();
	RectangleShape bg({ (float)totalWidth, (float)totalHeight });
	float y = view_offset.y - totalHeight / 2;
	bg.setPosition({ x, y });
	bg.setFillColor(Color(200, 200, 200));
	window->draw(bg);
	for (int i = 0; i < maxSlots; i++)
	{
		int row = i / width;
		int col = i % width;
		slots[i].draw(x + (col * 36 + 4) * guiScale, y + ((row + 2) * 36 + 4) * guiScale);
		slots[i].update();
	}
	Text nameText(font, name, 32 * guiScale);
	nameText.setPosition({x + 4, y + 4});
	window->draw(nameText);
	
	if (player.itemOnCursor.first == "" || player.itemOnCursor.second == 0)
		return;
	Sprite itemOnCursor(TextureManager::get(ItemManager::get(player.itemOnCursor.first).textureName));
	itemOnCursor.scale({ (float)guiScale, (float)guiScale });
	itemOnCursor.setPosition({ float(MouseInputManager::globalPosition.x), float(MouseInputManager::globalPosition.y) });
	window->draw(itemOnCursor);

	Text quantityText(font, to_string(player.itemOnCursor.second), 12 * guiScale);
	quantityText.setPosition({ float(MouseInputManager::globalPosition.x) + 4, float(MouseInputManager::globalPosition.y) + 4 });
	window->draw(quantityText);
}

int Inventory::addItem(string item, int quantity)
{
	for (auto& slot : slots)
	{
		if (slot.item == item && slot.quantity < ItemManager::get(item).maxStack)
		{
			int space = ItemManager::get(item).maxStack - slot.quantity;
			int toAdd = min(space, quantity);
			slot.quantity += toAdd;
			quantity -= toAdd;
			if (quantity <= 0)
				return 0;
		}
	}
	for (auto& slot : slots)
	{
		if (slot.quantity == 0)
		{
			int toAdd = min(ItemManager::get(item).maxStack, quantity);
			slot.item = item;
			slot.quantity = toAdd;
			quantity -= toAdd;
			if (quantity <= 0)
				return 0;
		}
	}
	return quantity; // Return remaining quantity that couldn't be added

}

int Inventory::removeItem(string item, int quantity)
{
	int maxQuantity = quantity;
	for (auto& slot : slots)
	{
		if (slot.item == item)
		{
			int toRemove = min(slot.quantity, quantity);
			slot.quantity -= toRemove;
			quantity -= toRemove;
			if (quantity <= 0)
				return maxQuantity;
		}
	}
	return maxQuantity - quantity; // Return remaining quantity that couldn't be removed
}

int Inventory::getItemQuantity(string item)
{
	int total = 0;
	for (const auto& slot : slots)
	{
		if (slot.item == item)
			total += slot.quantity;
	}
	return total;
}

void createItems()
{
	ItemManager::registerItem("stone", {
		"Stone",
		"A simple stone. Will be used to craft shit.",
		"items/resources/stone",
		64,
		nullptr
		});
}