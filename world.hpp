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

#include "items.hpp"
#include "tiles.hpp"
#include "entities.hpp"

using namespace std;
using namespace sf;

extern RectangleShape ground;
extern int chunkSize;
extern Vector2i worldSize;

void create_world(string name, int seed);
void update_world();
void update_surroundings(Vector2i chunk, Vector2i subc);

void delete_tile(Tile* tile);
void deleteEntity(Entity* entity);
extern Tile* breaking_mem;
Tile* getTile(Vector2i chunk, Vector2i subc);
class Chunk
{
public:
	Vector2i position = {};
	unique_ptr<Tile> tiles[16][16] = { nullptr };
	RectangleShape ground;
	RectangleShape debugGround;
	void rockdom();
	vector<Tile*> list_tiles();

	Chunk();
	Chunk(Vector2i pos);

	Chunk(const Chunk&) = delete;
	Chunk& operator=(const Chunk&) = delete;

	Chunk(Chunk&&) = default;
	Chunk& operator=(Chunk&&) = default;
};
extern vector<vector<Chunk>> world_chunks;