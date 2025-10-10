#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"

vector<CircleShape> debug_draw;
vector<string> debug_text;

Font font("ARIAL.ttf");

bool debug_visual = false;

Vector2f view_offset = {0.f, 0.f};
Vector2f player_local_poss;

void set_view_offset()
{
    Vector2i window_size = Vector2i(window->getSize().x, window->getSize().y);
    view_offset = Vector2f(window_size.x / 2, window_size.y / 2);
    if (player.position.x > window_size.x / 2 and player.position.x < world_chunks.size() * (tile_size.x * 16) - window_size.x / 2)
        view_offset.x = player.position.x;
    else if (player.position.x > world_chunks.size() * (tile_size.x * 16) - window_size.x / 2)
        view_offset.x = world_chunks.size() * (tile_size.x * 16) - window_size.x / 2;
    if (player.position.y > window_size.y / 2 and player.position.y < world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2)
        view_offset.y = player.position.y;
    else if (player.position.y > world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2)
        view_offset.y = world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2;
    window->setView(View({ view_offset.x, view_offset.y}, Vector2f(window_size.x, window_size.y)));
	debug_text.push_back("Player offest: " + to_string(view_offset.x) + ", " + to_string(view_offset.y));
    player_local_poss = { player.position.x - view_offset.x + window_size.x / 2, player.position.y - view_offset.y + window_size.y / 2 };
}


void create_instance()
{
    window->setKeyRepeatEnabled(false);
}

void render_player()
{
    player_model.setPosition({ player.position.x, player.position.y + player.jump_offset });
    window->draw(player_model);
    Tile* tile_below;
    if (player.subc.y < 15)
        tile_below = world_chunks[player.chunk.x][player.chunk.y].changeables[player.subc.x][player.subc.y + 1].get();
    else
        tile_below = world_chunks[player.chunk.x][player.chunk.y + 1].changeables[player.subc.x][0].get();
    if (tile_below != nullptr)
        tile_below->draw();
}

void render_entities()
{
    for (CircleShape entity : entities) {
        window->draw(entity);
    }
}

void render_world()
{
    for (int i = 0; i < int(world_chunks.size()); i++)
    {
        for (Chunk& current : world_chunks[i])
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
    if (debug_visual)
    {
        CircleShape poss(2.f);
        poss.setFillColor(Color::Red);
        poss.setPosition({ player.position.x , player.position.y });
        window->draw(poss);
        for (auto cir : debug_draw)
        {
            window->draw(cir);
        }
        for (int i = 0; i < int(debug_text.size()); i++)
        {
            float poss_y = player.position.y - i * 20 + 20;
            Text txt(font);
            txt.setString(debug_text[i]);
            txt.setPosition({ player.position.x , poss_y });
            txt.setFillColor(Color::Red);
            txt.setCharacterSize(20);
            window->draw(txt);
        }
    }
	debug_draw.clear();
    debug_text.clear();
}

void render()
{
    window->clear();
    if (debug_key)
    {
        debug_visual = !debug_visual;
        debug_key = false;
    }
    set_view_offset();
    render_world();
    render_entities();
    render_player();
    render_debug();
    window->display();
}