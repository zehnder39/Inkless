#pragma once
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <stdexcept>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace std;
using namespace sf;

extern CircleShape entity1;
extern RectangleShape ground;
extern vector<CircleShape> entities;
extern Vector2i tile_size;

void create_world(string name, int seed);
void update_world();
void update_surroundings(Vector2i chunk, Vector2i subc);

class Player
{
public:
	Vector2f position = {};
	Vector2i chunk = {};
	Vector2i subc = {};
	int base_speed = 3.5;
	int speed;
	int baseSwimSpeed = 8;
	bool swimming;
	int animation_state = 0;
	float animation_offset = 0;
	Player(float set_x, float set_y);
	void updateChunkSubc();
	void swim(Vector2f movement);
	void walk(Vector2f movement);
	void move(Vector2f movement);
	void walkAnimation(bool moving, float dx);
};
extern Player player;

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
	Texture texture;

	virtual ~Tile() = default;

	virtual void interact() = 0;
	virtual void update() = 0;
	virtual void draw();

	// Needed for unique_ptr polymorphic copy (during deserialization)
	virtual std::unique_ptr<Tile> clone() const = 0;

	// Used to identify derived type when serializing
	virtual std::string type() const = 0;

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
	}

	void from_json(const nlohmann::json& j) override {
		up = j.at("up");
		down = j.at("down");
		left = j.at("left");
		right = j.at("right");
		center = { j.at("center")[0], j.at("center")[1] };
		subc = { j.at("sub")[0], j.at("sub")[1] };
		chunk = { j.at("chunk")[0], j.at("chunk")[1] };
	}
};

void delete_tile(Tile* tile);
extern Tile* breaking_mem;
Tile* get_tile(Vector2i chunk, Vector2i subc);
class Chunk
{
public:
	Vector2i position = {};
	unique_ptr<Tile> changeables[16][16] = { nullptr };
	RectangleShape ground;
	RectangleShape debugGround;
	void rockdom();
	vector<Tile*> list_tiles();

	Chunk();
	Chunk(Vector2i pos);
};
extern vector<vector<Chunk>> world_chunks;
