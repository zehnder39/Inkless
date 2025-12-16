#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// flags:
extern bool moving_left, moving_right, moving_up, moving_down;
extern bool swimmingAdvUp, swimmingAdvDown, swimmingAdvLeft, swimmingAdvRight;
extern bool facing_left;
extern bool looking_up, looking_down, looking_right, looking_left;
extern bool should_close, mouse_1, mouse_2, use_key, debug_key;

extern Vector2i mouse_pos;
extern Vector2f mouse_vector;

void input();
