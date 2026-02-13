#include "definer.hpp"
#include "input.hpp"
#include "saves.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include "window.hpp"

#include <nlohmann/json.hpp>

#include <iostream>
#include <fstream>
#include <string>


string world_name = "New World";

inline unique_ptr<Tile> make_tile(const string& type)
{
    if (type == "Gutter") return make_unique<Gutter>();
	if (type == "Rock") return make_unique<Rock>();

    throw runtime_error("Unknown tile type: " + type);
}

void to_json(nlohmann::json& j, const Chunk& c)
{
    j = nlohmann::json::object();
    j["position"] = { c.position.x, c.position.y };

    // 2D tile array
    nlohmann::json grid = nlohmann::json::array();

    for (int y = 0; y < 16; y++) {
        nlohmann::json row = nlohmann::json::array();

        for (int x = 0; x < 16; x++) {
            if (c.changeables[y][x]) {
                nlohmann::json jt;
                c.changeables[y][x]->to_json(jt);
                jt["type"] = c.changeables[y][x]->type();
                row.push_back(jt);
            }
            else {
                row.push_back(nullptr);
            }
        }

        grid.push_back(row);
    }

    j["tiles"] = grid;
}

void from_json(const nlohmann::json& j, Chunk& c)
{
    // load chunk position
    c.position.x = j.at("position")[0];
    c.position.y = j.at("position")[1];

    c.ground.setPosition(Vector2f((tile_size.x * 16) * c.position.x, (tile_size.y * 16) * c.position.y));
    c.debugGround.setPosition(Vector2f((tile_size.x * 16) * c.position.x, (tile_size.y * 16) * c.position.y));

    const auto& grid = j.at("tiles");

    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 16; x++) {

            const auto& cell = grid.at(y).at(x);

            if (cell.is_null()) {
                c.changeables[y][x] = nullptr;
                continue;
            }

            std::string type = cell.at("type");

            c.changeables[y][x] = make_tile(type);
			c.changeables[y][x]->from_json(cell);
        }
    }
}

namespace sf{
    void to_json(nlohmann::json& j, const Vector2f& v)
    {
        j = { {"x", v.x}, {"y", v.y}};
    }

    void from_json(const nlohmann::json& j, sf::Vector2f& v)
    {
        v.x = j.at("x").get<float>();
        v.y = j.at("y").get<float>();
    }
}

void to_json(nlohmann::json& j, const WorldSave& w)
{
    j = nlohmann::json{
        {"player", {
            {"x", w.playerPosition.x},
            {"y", w.playerPosition.y}
        }},
        {"chunks", w.chunks}
    };
}

void from_json(const nlohmann::json& j, WorldSave& w)
{
    w.playerPosition = j.at("player").get<sf::Vector2f>();
    j.at("chunks").get_to(w.chunks);  // best option
}

void save_world()
{
    try {
        WorldSave save;
        save.chunks = std::move(world_chunks);   // move instead of copy
        save.playerPosition = player.position;

        nlohmann::json j = save;

        world_chunks = std::move(save.chunks);

        ofstream file("saves/" + world_name + ".json"s);
        if (!file) {
            cerr << "Error opening file for writing: " << world_name << endl;
            return;
        }

        file << j.dump(4); // pretty-print
        file.close();

        cout << "World saved to " << world_name << ".json" << endl;
    }
    catch (const exception& e) {
        cerr << "Exception while saving world: " << e.what() << endl;
    }
}

void load_world()
{
    try
    {
        ifstream file("saves/" + world_name + ".json"s);
        if (!file) {
            cerr << "Error opening file for reading: " << world_name << endl;
            return;
        }


        nlohmann::json j;
        file >> j;

		world_chunks.clear();

        WorldSave save = j.get<WorldSave>();
        world_chunks = std::move(save.chunks);
        player.position = save.playerPosition;
        player_model.setOrigin({ 16.f, 16.f });
    }
    catch (const exception& e) {
        cerr << "Exception while loading world: " << e.what() << endl;
    }
}