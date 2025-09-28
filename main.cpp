#include <iostream>

#include "header.hpp"


int main()
{
    window = new RenderWindow(VideoMode({ 1920, 1080 }), "Inkless", State::Fullscreen);
    window->setVerticalSyncEnabled(true);
    create_instance();
    create_world();
    while (window->isOpen())
    {
        input();
        move_player();
        check_action();
        update_world();
        render();
        if (should_close)
            window->close();
    }
}