#include "items.hpp"
#include "texture.hpp"
#include "renderer.hpp"
#include "window.hpp"

static unordered_map<string, ItemDef> itemMap;
int guiScale = 2;

void ItemManager::registerItem(string name, ItemDef def) {
	itemMap[name] = def;
}

const ItemDef& ItemManager::get(string name) {
	return itemMap.at(name);
}

void InventorySlot::draw(float x, float y)
 {
	 RectangleShape bg({ 32.f * guiScale, 32.f * guiScale });
	 bg.setPosition({ x, y });
	 bg.setFillColor(Color(100, 100, 100));
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

int Inventory::getWidth() {
	int totalWidth = (width * 32 + (width + 1) * 4) * guiScale;
	return totalWidth;
}

int Inventory::getHeight() {
	int rows = (maxSlots + width - 1) / width; // Calculate the number of rows needed
	int totalHeight = (rows * 32 + (rows + 1) * 4) * guiScale;
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
		slots[i].draw(x + (col * 36 + 4) * guiScale, y + (row * 36 + 4) * guiScale);
	}
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