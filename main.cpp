#include <iostream>
#include <chrono>

#include "definer.hpp"
#include "renderer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "physics.hpp"
#include "saves.hpp"
#include "window.hpp"
#include "player.hpp"
#include "texture.hpp"
#include "items.hpp"
#include "entities.hpp"
#include "tiles.hpp"
#include "craft.hpp"

double accumulatedTime = 0.0;
const double timePerTick = 1000 / 60; //60 tps

void loadAssets()
{
    load_textures();
    createMenus();
    createItems();
    defineCrafts();
}

int main()
{
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto nowTime = std::chrono::high_resolution_clock::now();

    window = new RenderWindow(VideoMode({ 1920, 1080 }), "Inkless", State::Fullscreen);
    window->setVerticalSyncEnabled(true);
	//window->setFramerateLimit(6);

    create_instance();
	loadAssets();
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
            if (KeyInputManager::isActionTapped("escape"))
            {
                if (player.state != playerState::normal)
                {
                    player.state = playerState::normal;
                    player.container = nullptr;
                }
                gamePaused = !gamePaused;
				debug_info.push_back("Toggled pause, gamePaused: " + to_string(gamePaused));
            }

            duration<double, std::milli> calculationTime = high_resolution_clock::now() - nowTime;
            nowTime = high_resolution_clock::now();
            accumulatedTime += calculationTime.count();

            if (gamePaused)
                accumulatedTime = 0;

            while (accumulatedTime >= timePerTick)
            {
                update_world();
                playerMovement();
                check_action();
				accumulatedTime -= timePerTick;
            }

			debug_info.push_back("Game paused: " + to_string(gamePaused));
			debug_info.push_back("Escape Key: " + to_string(KeyInputManager::isActionActive("escape")));

            render();
            break;

        }
        if (shouldClose)
            window->close();

    }
}
