#include <iostream>
#include <chrono>

#include "definer.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "saves.hpp"
#include "window.hpp"

double accumulatedTime = 0.0;
const double timePerTick = 1000 / 60; //60 tps

int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto nowTime = std::chrono::high_resolution_clock::now();

    window = new RenderWindow(VideoMode({ 1920, 1080 }), "Inkless", State::Fullscreen);
    window->setVerticalSyncEnabled(true);
	window->setFramerateLimit(6);

    create_instance();
    load_textures();
	createMenus();
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
			worldLoadingScreen.update();
            break;

        case GameState::InGame:
            if (escapeKey)
            {
                gamePaused = !gamePaused;
				debug_info.push_back("Toggled pause, gamePaused: " + to_string(gamePaused));
                escapeKey = false;
            }

            duration<double, std::milli> calculationTime = high_resolution_clock::now() - nowTime;
            nowTime = high_resolution_clock::now();
            accumulatedTime += calculationTime.count();

            if (gamePaused)
                accumulatedTime = 0;

            while (accumulatedTime >= timePerTick)
            {
                playerMovement();
                check_action();
                update_world();
				accumulatedTime -= timePerTick;
            }

			debug_info.push_back("Game paused: " + to_string(gamePaused));
			debug_info.push_back("Escape Key: " + to_string(escapeKey));

            render();
            break;

        }
        if (shouldClose)
            window->close();

    }
}