#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <SFML/Graphics.hpp>

#include "world.hpp"

using namespace std;
using namespace sf;

class Player;
extern Player player;

//flags
extern bool gamePaused;

extern pair<Vector2f, Vector2f> swimmingPath;

void playerMovement();
bool check_move(float dx, float dy);
void check_action();
bool breakingTile();

class GutterPathing
{
public:
	pair<Vector2f, Vector2f> swimmingPath;

	void updateSwimmingPath(Gutter* gutter);
	pair<Tile*, bool> nextSwimmingTile();
	bool playerOutOffSwimmingPath();

	GutterPathing() = default;
};
