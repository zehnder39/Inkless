#pragma once

#include <string>
#include <vector>
#include <array>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace std;
using namespace sf;

extern vector<CircleShape> debug_draw;
extern vector<string> debug_text, debug_info;
extern Vector2f player_local_pos;
extern Vector2f view_offset;
extern Font font;


void render();
