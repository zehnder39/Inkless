#include <iostream>
#include <chrono>

#include "header.hpp"


int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    window = new RenderWindow(VideoMode({ 1920, 1080 }), "Inkless", State::Fullscreen);
    window->setVerticalSyncEnabled(true);

    create_instance();
    load_textures();
    create_world();
    while (window->isOpen())
    {
        auto timeStart = high_resolution_clock::now();
        input();
        playerMovement();
        check_action();
        update_world();
        duration<double, std::milli> calculationTime = high_resolution_clock::now() - timeStart;
        render();
        if (should_close)
            window->close();
        debug_info.push_back("MSPT: " + to_string(calculationTime.count()));
        int uncaped_tps = 1000 / calculationTime.count();
        debug_info.push_back("TPS: " + to_string(uncaped_tps));

    }
}