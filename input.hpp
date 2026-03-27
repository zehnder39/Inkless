#pragma once
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <unordered_map>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

// flags:
extern bool swimmingAdvUp, swimmingAdvDown, swimmingAdvLeft, swimmingAdvRight;
extern bool facing_left;
extern bool looking_up, looking_down, looking_right, looking_left;
extern bool mouse_1, mouse_2;

extern Vector2i mouse_pos;
extern Vector2f mouse_vector;

void input();

class KeyInputManager
{
public:
	inline static const unordered_map<string, Keyboard::Key> keyBinds = {
		{"move_left", Keyboard::Key::Q},
		{"move_right", Keyboard::Key::D},
		{"move_up", Keyboard::Key::Z},
		{"move_down", Keyboard::Key::S},
		{"escape", Keyboard::Key::Escape},
		{"use", Keyboard::Key::E},
		{"debug", Keyboard::Key::T},
		{"inventory", Keyboard::Key::Tab},
	};

	static void update();
	static bool isActionActive(string action);
	static bool isActionTapped(string action);
private:
	static list<Keyboard::Key> keyTapped;
	static list<Keyboard::Key> keyPressedThisFrame;
};

