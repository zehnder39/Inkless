#pragma once
#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <functional>

#include "tiles.hpp"

class Machine;

using namespace std;
using namespace sf;

extern void defineCrafts();

struct CraftDef {
    string name;
    string description;
    string textureName;
    int duration;

	vector<pair<string, int>> ingredients; // List of (item name, quantity) pairs required for crafting
	vector<pair<string, int>> results; // List of (item name, quantity) pairs produced by crafting

    bool operator==(const CraftDef& other) const {
        return this->name == other.name;// Items are "equal" if their IDs match
    }
};

class CraftManager {
private:
    static unordered_map<string, CraftDef>& craftMap;
public:
    static void registerCraft(string name, CraftDef def);
    static const CraftDef& get(string name);
};

struct CraftSelector {
public:
	inline static Machine* machine;
	inline static vector<string> allowedCrafts;
	inline static constexpr int maxWidth = 6;
	inline static constexpr int maxHeight = 10;
	inline static bool open;

	static void draw();
};