#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

//flags
extern bool gamePaused;

extern pair<Vector2f, Vector2f> swimmingPath;

void playerMovement();
bool check_move(float dx, float dy);
void check_action();
bool breakingTile();
