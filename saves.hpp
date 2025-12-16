#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <nlohmann/json.hpp>
#include "world.hpp"

using namespace std;
using namespace sf;

extern string world_name;

unique_ptr<Tile> make_tile(const string& type);

void to_json(nlohmann::json& j, const Chunk& c);
void from_json(const nlohmann::json& j, Chunk& c);

void load_world(const string& filename);
void save_world(const string& filename);