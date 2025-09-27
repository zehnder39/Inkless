#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"


void input()
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