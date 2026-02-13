#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include "world.hpp"

using namespace std;
using namespace sf;

extern string world_name;

unique_ptr<Tile> make_tile(const string& type);

void to_json(nlohmann::json& j, const Chunk& c);
void from_json(const nlohmann::json& j, Chunk& c);

void to_json(nlohmann::json& j, const sf::Vector2f& v);

void from_json(const nlohmann::json& j, sf::Vector2f& v);

struct WorldSave
{
	vector<vector<Chunk>> chunks;
	Vector2f playerPosition;
};

void load_world();
void save_world();