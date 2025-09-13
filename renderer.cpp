#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"


void create_instance()
{
    window->setFramerateLimit(60);
    window->setKeyRepeatEnabled(false);
}

void render_player()
{
    player_model.setPosition({ player.position.x,player.position.y });
    window->draw(player_model);
}

void render_entities()
{
    for (CircleShape entity : entities) {
        window->draw(entity);
    }
}

void render_world()
{
    window->draw(ground);
}

void render()
{
    window->clear();
    render_world();
    render_entities();
    render_player();
    window->display();
}