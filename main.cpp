#include <iostream>
#include <chrono>

#include "definer.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "saves.hpp"
#include "window.hpp"


int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    window = new RenderWindow(VideoMode({ 1920, 1080 }), "Inkless", State::Fullscreen);
    window->setVerticalSyncEnabled(true);
	//window->setFramerateLimit(4);

    create_instance();
    load_textures();
	createTypeMenuScreens();
    while (window->isOpen())
    {
        input();
        switch (currentState)
        {
        case GameState::TitleScreen:
            titleScreen.update();
            break;

		case GameState::WorldCreation:
            worldCreationScreen.update();
			break;

        case GameState::WorldLoading:
            break;

        case GameState::InGame:
            auto timeStart = high_resolution_clock::now();
            playerMovement();
            check_action();
            update_world();
            duration<double, std::milli> calculationTime = high_resolution_clock::now() - timeStart;
            render();
            debug_info.push_back("MSPT: " + to_string(calculationTime.count()));
            int uncaped_tps = 1000 / calculationTime.count();
            debug_info.push_back("TPS: " + to_string(uncaped_tps));
            break;
        }
        if (should_close)
            window->close();

    }
}