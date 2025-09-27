#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"

vector<CircleShape> debug_draw;
vector<string> debug_text;

Font font("ARIAL.ttf");

bool debug_visual = true;

void create_instance()
{
    window->setFramerateLimit(60);
    window->setKeyRepeatEnabled(false);
}

void render_player()
{
    player_model.setPosition({ player.position.x, player.position.y + player.jump_offset });
    window->draw(player_model);
    auto tile_below = world_chunks[player.chunk.x][player.chunk.y].changeables[player.sub_c.x][player.sub_c.y + 1];
    if (tile_below != NULL)
        tile_below->draw();
    CircleShape poss(2.f);
	poss.setFillColor(Color::Red);
	poss.setPosition({ player.position.x, player.position.y });
	window->draw(poss);
}

void render_entities()
{
    for (CircleShape entity : entities) {
        window->draw(entity);
    }
}

void render_world()
{
    for (int i = 0; i < world_chunks.size(); i++)
    {
        for (Chunk current : world_chunks[i])
        {
            window->draw(current.ground);
            for (Tile* tile : current.list_tiles())
            {
                if (tile != NULL)
                    tile->draw();
            }
        }
    }
}

void render_debug()
{
    for (auto cir : debug_draw)
    {
        window->draw(cir);
    }
    debug_draw.clear();
    for (int i = 0; i < debug_text.size(); i++)
    {
        float poss_y = player.position.x + i * 10;
        Text txt(font);
        txt.setString(debug_text[i]);
        txt.setPosition({ player.position.x, poss_y });
        txt.setFillColor(Color::Red);
        txt.setCharacterSize(10);
        window->draw(txt);
    }
    debug_text.clear();
}

void render()
{
    window->clear();
    render_world();
    render_entities();
    render_player();
    if (debug_visual)
        render_debug();
    window->display();
}