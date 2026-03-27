#include <SFML/Graphics.hpp>
#include <numbers>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "renderer.hpp"
#include "definer.hpp"
#include "input.hpp"
#include "window.hpp"
#include "world.hpp"
#include "physics.hpp"
#include "player.hpp"

//flags
bool looking_up, looking_down, looking_right, looking_left;
bool mouse_1, mouse_2;
bool facing_left;

list<sf::Keyboard::Key> KeyInputManager::keyPressedThisFrame;
list<sf::Keyboard::Key> KeyInputManager::keyTapped;

Vector2i mouse_pos;
Vector2f mouse_vector;


void KeyInputManager::update()
{
    keyTapped.clear();
    while (const optional event = window->pollEvent())
    {
        if (event->is<Event::Closed>())
            shouldClose = true;
        if (const auto* key = event->getIf<Event::KeyPressed>())
        {
            keyPressedThisFrame.push_back(key->code);
			keyTapped.push_back(key->code);
        }
        if (const auto* key = event->getIf<Event::KeyReleased>())
        {
			keyPressedThisFrame.remove(key->code);
        }
    }
}

bool KeyInputManager::isActionActive(string action)
{
	Keyboard::Key key = KeyInputManager::keyBinds.at(action);
    if (find(keyPressedThisFrame.begin(), keyPressedThisFrame.end(), key) != keyPressedThisFrame.end())
		return true;
    return false;
}

bool KeyInputManager::isActionTapped(string action) {
    Keyboard::Key key = keyBinds.at(action);
    return find(keyTapped.begin(), keyTapped.end(), key) != keyTapped.end();
}

void get_mouse_look()
{
    mouse_vector = { mouse_pos.x - player.position.x, - mouse_pos.y + player.position.y };
    float angle = atan2f(mouse_vector.y, mouse_vector.x);
    debug_text.push_back(to_string(angle));
    looking_down = false;
    looking_up = false;
    looking_left = false;
    looking_right = false;
	if (angle >= - numbers::pi / 3 and angle <= numbers::pi / 3)
        looking_right = true;
    if (angle >= numbers::pi / 6 and angle <= numbers::pi * 5 / 6)
		looking_up = true;
    if (angle >= numbers::pi * 2 / 3 or angle <= -numbers::pi * 2 / 3)
        looking_left = true;
    if (angle >= - numbers::pi * 5 / 6 and angle <=  - numbers::pi / 6)
        looking_down = true;
}

void mouse_input()
{
    if (currentState == GameState::InGame)
        mouse_pos = Vector2i(Mouse::getPosition(*window).x + view_offset.x - window->getSize().x / 2, Mouse::getPosition(*window).y + view_offset.y - window->getSize().y / 2);
    else
		mouse_pos = Vector2i(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);
    if (Mouse::isButtonPressed(Mouse::Button::Left))
        mouse_1 = true;
    else
        mouse_1 = false;
    if (Mouse::isButtonPressed(Mouse::Button::Right))
        mouse_2 = true;
    else
        mouse_2 = false;
	get_mouse_look();

}

void input()
{
    KeyInputManager::update();
    mouse_input();
}

void Gutter::interact()
{
	player.state = playerState::swimming;
    Vector2f tile_pos = chunkSubcToPos(chunk, subc);
    player.position = Vector2f(tile_pos.x + center.x, tile_pos.y + center.y);
    player.updateChunkSubc();
    swimmingPath = make_pair(player.position, player.position);
    swimmingAdvDown = false;
    swimmingAdvLeft = false;
    swimmingAdvRight = false;
    swimmingAdvUp = false;
}