#include <SFML/Graphics.hpp>
#include <numbers>
#include <iostream>
#include <cmath>

#include "renderer.hpp"
#include "definer.hpp"
#include "input.hpp"
#include "window.hpp"
#include "world.hpp"
#include "physics.hpp"

//flags
bool looking_up, looking_down, looking_right, looking_left;
bool moving_left, moving_right, moving_up, moving_down;
bool facing_left, facing_up, escapeKey, mouse_1, mouse_2, use_key, debug_key;
vector<Event::KeyPressed> keyPressedLastFrame;

Vector2i mouse_pos;
Vector2f mouse_vector;


void key_input()
{
    while (const optional event = window->pollEvent())
    {
        if (event->is<Event::Closed>())
            shouldClose = true;
        if (const auto* key = event->getIf<Event::KeyPressed>())
        {
            switch (key->code)
            {
            case Keyboard::Key::Q:
                moving_left = true;
                break;
            case Keyboard::Key::D:
                moving_right = true;
                break;
            case Keyboard::Key::Z:
                moving_up = true;
                break;
            case Keyboard::Key::S:
                moving_down = true;
                break;
            case Keyboard::Key::Escape:
                escapeKey = true;
                break;
            case Keyboard::Key::E:
                use_key = true;
                break;
            case Keyboard::Key::T:
                debug_key = true;
                break;
            }
        }
        if (const auto* key = event->getIf<Event::KeyReleased>())
        {
            switch (key->code)
            {
            case Keyboard::Key::Q:
                moving_left = false;
                break;
            case Keyboard::Key::D:
                moving_right = false;
                break;
            case Keyboard::Key::Z:
                moving_up = false;
                break;
            case Keyboard::Key::S:
                moving_down = false;
                break;
            case Keyboard::Key::Escape:
                escapeKey = false;
                break;
            case Keyboard::Key::E:
                use_key = false;
                break;
            case Keyboard::Key::T:
                debug_key = false;
                break;
            }
        }
    }

    
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
    key_input();
    mouse_input();
}

void Gutter::interact()
{
    player.swimming = true;
    Vector2f tile_pos = chunkSubcToPos(chunk, subc);
    player.position = Vector2f(tile_pos.x + center.x, tile_pos.y + center.y);
    player.updateChunkSubc();
    swimmingPath = make_pair(player.position, player.position);
    swimmingAdvDown = false;
    swimmingAdvLeft = false;
    swimmingAdvRight = false;
    swimmingAdvUp = false;
}