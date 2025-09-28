#include <SFML/Graphics.hpp>
#include <numbers>
#include <iostream>
#include <cmath>

#include "header.hpp"

bool looking_up;
bool looking_down;
bool looking_right;
bool looking_left;
bool moving_left, moving_right, moving_up, moving_down;
bool facing_left = false, facing_up = false;
bool break_key = false;
bool should_close = false;
bool left_click, right_click;

Vector2i mouse_pos;

void key_input()
{
    while (const optional event = window->pollEvent())
    {
        if (event->is<Event::Closed>())
            should_close = true;
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
                should_close = true;
                break;
            case Keyboard::Key::E:
                break_key = true;
                break;
            case Keyboard::Key::T:
                player_model.setRotation(degrees(0));
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
            case Keyboard::Key::E:
                break_key = false;
            }
        }
    }
}

void get_mouse_look()
{
    Vector2f dir = { mouse_pos.x - player.position.x, -mouse_pos.y + player.position.y };
    float angle = atan2f(dir.y, dir.x);
	if (angle >= -1 * numbers::pi / 3 and angle <= numbers::pi / 3)
        looking_right = true;
    else
		looking_right = false;
    if (angle >= numbers::pi / 6 and angle <= numbers::pi * 5 / 6)
		looking_up = true;
	else
		looking_up = false;
    if (angle >= numbers::pi * 2 / 3 or angle <= - numbers::pi * 2 / 3)
        looking_left = true;
	else
		looking_left = false;
    if (angle >= - numbers::pi * 5 / 6 and angle <=  - numbers::pi / 6)
        looking_down = true;
    else
		looking_down = false;
}

void mouse_input()
{
    mouse_pos = Mouse::getPosition(*window);
    if (Mouse::isButtonPressed(Mouse::Button::Left))
        {
        CircleShape cir(10.f);
        cir.setFillColor(Color::Red);
        cir.setPosition({ static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y) });
        debug_draw.push_back(cir);
	}
	get_mouse_look();

}

void input()
{
    key_input();
    mouse_input();
}