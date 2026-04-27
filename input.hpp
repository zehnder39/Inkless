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

	static void update(const optional<Event> event);
	static bool isActionActive(string action);
	static bool isActionTapped(string action);

	static list<Keyboard::Key> keyTapped;
	static list<Keyboard::Key> keyPressedThisFrame;
};

class MouseInputManager
{
public:
	inline static const unordered_map<string, Mouse::Button> keyBinds = {
		{"break", Mouse::Button::Left},
		{"place", Mouse::Button::Right}
	};
	static Vector2i localPosition;
	static Vector2i globalPosition;
	static Vector2f vector;

	static void update(const optional<Event> event);
	static bool isActionActive(string action);
	static bool isActionTapped(string action);

	static list<Mouse::Button> buttonTapped;
	static list<Mouse::Button> buttonPressedThisFrame;
};


