#include <iostream>

#include "header.hpp"


int main()
{
    window = new RenderWindow(VideoMode({ 800, 450 }), "Inkless");
    create_instance();
    create_world();
    while (window->isOpen())
    {
        input();
        move_player();
        update_world();
        render();
        if (should_close)
            window->close();
    }
}