#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

#include "items.hpp"
#include "craft.hpp"
#include "window.hpp"

using namespace std;
using namespace sf;

extern Vector2i tile_size;
extern vector<pair<Vector2i, Vector2i>> tickingTiles;

class Tile {
public:
	bool solid;
	bool breakable;
	int durability;
	float break_offset = 0;
	Vector2i subc;
	Vector2i chunk;
	Vector2f baseScale = { 1.f, 1.f };
	Vector2f baseOffset = { 0.f, 0.f };
	string textureName;

	virtual ~Tile() = default;

	virtual void interact() = 0;
	virtual void update() = 0;
	virtual void draw();
	virtual void dropLoot() = 0;

	// Needed for unique_ptr polymorphic copy (during deserialization)
	virtual unique_ptr<Tile> clone() const = 0;

	// Used to identify derived type when serializing
	virtual string type() const = 0;

	// Serialization hooks
	virtual void to_json(nlohmann::json& j) const = 0;
	virtual void from_json(const nlohmann::json& j) = 0;
};

class Rock : public Tile
{
public:

	void interact() override {}
	void update() override {}
	explicit Rock();
	explicit Rock(Vector2i sub, Vector2i chun);
	void dropLoot();


	string type() const override { return "Rock"; }
	unique_ptr<Tile> clone() const override {
		return make_unique<Rock>(*this);
	}

	void to_json(nlohmann::json& j) const override {
		j["sub"] = { subc.x, subc.y };
		j["chunk"] = { chunk.x, chunk.y };
	}
	void from_json(const nlohmann::json& j) override {
		subc = { j.at("sub")[0], j.at("sub")[1] };
		chunk = { j.at("chunk")[0], j.at("chunk")[1] };
	}
};

class Gutter : public Tile
{
public:
	bool up;
	bool down;
	bool left;
	bool right;
	Vector2f center;

	void interact() override;
	void update() override;
	void dropLoot() override {}

	explicit Gutter();
	explicit Gutter(Vector2i sub, Vector2i chun);

	string type() const override { return "Gutter"; }

	unique_ptr<Tile> clone() const override {
		return make_unique<Gutter>(*this);
	}

	void to_json(nlohmann::json& j) const override {
		j["up"] = up;
		j["down"] = down;
		j["left"] = left;
		j["right"] = right;
		j["center"] = { center.x, center.y };
		j["sub"] = { subc.x, subc.y };
		j["chunk"] = { chunk.x, chunk.y };
		j["baseScale"] = { baseScale.x, baseScale.y };
		j["baseOffset"] = { baseOffset.x, baseOffset.y };
		j["textureName"] = textureName;
	}

	void from_json(const nlohmann::json& j) override {
		up = j.at("up");
		down = j.at("down");
		left = j.at("left");
		right = j.at("right");
		center = { j.at("center")[0], j.at("center")[1] };
		subc = { j.at("sub")[0], j.at("sub")[1] };
		chunk = { j.at("chunk")[0], j.at("chunk")[1] };
		baseScale = { j.at("baseScale")[0], j.at("baseScale")[1] };
		baseOffset = { j.at("baseOffset")[0], j.at("baseOffset")[1] };
		textureName = j.at("textureName");
	}
};

class Container : public Tile
{
public:
	virtual void drawInterface(float x) = 0;
	void interact() override;
	vector<string> allowedItems;


	virtual int addItem(string item, int quantity) = 0;
	virtual pair<string, int> removeItem(int quantity) = 0;
	virtual int removeItem(string item, int quantity) = 0;

	void dropLoot() override {}

	virtual ~Container() = default;

	virtual void to_json(nlohmann::json& j) const override = 0;
	virtual void from_json(const nlohmann::json& j) override = 0;
};

class Chest : public Container
{
public:
	Inventory inventory{ 32, 8, string("Chest") };

	void update() override {}

	void drawInterface(float x) override {
		inventory.draw(x);
	}
	int addItem(string item, int quantity) override {
		return inventory.addItem(item, quantity);
	}
	int removeItem(string item, int quantity) override {
		return inventory.removeItem(item, quantity);
	}
	pair<string, int> removeItem(int quantity) override;

	explicit Chest();
	explicit Chest(Vector2i chunk, Vector2i subc);

	string type() const override { return "Chest"; }

	unique_ptr<Tile> clone() const override {
		return make_unique<Chest>(*this);
	}

	void to_json(nlohmann::json& j) const override {
		j["sub"] = { subc.x, subc.y };
		j["chunk"] = { chunk.x, chunk.y };
	}

	void from_json(const nlohmann::json& j) override {
		subc = { j.at("sub")[0], j.at("sub")[1] };
		chunk = { j.at("chunk")[0], j.at("chunk")[1] };
	}
};

class Machine : public Container
{
public:
	vector<InventorySlot> inputSlots;
	vector<InventorySlot> outputSlots;
	string craftName;
	float progress = 0.f;
	int duration = 0;
	float progressSpeed = 1.f;
	int maxWidth = 4;
	vector<string> allowedCrafts;
	bool running = true;

	void update() override;
	void drawInterface(float x) override;
	int addItem(string item, int quantity) override;
	int removeItem(string item, int quantity) override;
	pair<string, int> removeItem(int quantity) override;
	void setCraft(string craftName);

	virtual ~Machine() = default;

	void to_json(nlohmann::json& j) const override {}
	void from_json(const nlohmann::json& j) override {}
};

class Furnace : public Machine
{
	public:
	Furnace(Vector2i chunk, Vector2i subc) {
		this->subc = subc;
		this->chunk = chunk;
		allowedCrafts = { "stoneCooking" };
		inputSlots.resize(maxWidth);
		outputSlots.resize(maxWidth);
		textureName = "assets/passive/chest";
		solid = true;
		breakable = true;
		durability = 45;
		break_offset = 0;
		tickingTiles.push_back(make_pair(chunk, subc));

	}
	string type() const override { return "Furnace"; }
	unique_ptr<Tile> clone() const override {
		return make_unique<Furnace>(*this);
	}
};