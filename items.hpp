#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

#include "constants.hpp"

class Player;

using namespace std;
using namespace sf;

void createItems();

struct InventorySlot
{
	string item = "";
	int quantity = 0;
	bool hovered = false;
    Vector2f position;

    void draw(float x, float y);
	void draw(float x, float y, bool transparent);
    void update();

	InventorySlot() {}
};

struct Inventory {
    vector<InventorySlot> slots;
	int maxSlots;
    int width;
    string name;

    void draw(float x);
	int getItemQuantity(string item);
	int addItem(string item, int quantity);
	int removeItem(string item, int quantity);
    int getWidth() const;
    int getHeight() const;

	Inventory(int maxSlots, int width, string name) : maxSlots(maxSlots), width(width), name(name) {
		slots.resize(maxSlots);
	}
};

struct ItemDef {
    string name;
    string description;
    string textureName;
	int maxStack;

    function<void(Player&)> onUse;

    bool operator==(const ItemDef& other) const {
        return this->name == other.name;// Items are "equal" if their IDs match
    }
};

class ItemManager {
private:
	static unordered_map<string, ItemDef> itemMap;
public:
    static void registerItem(string name, ItemDef def);
    static const ItemDef& get(string name);
};