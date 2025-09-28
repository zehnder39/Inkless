#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"

vector<CircleShape> debug_draw;
vector<string> debug_text;

Font font("ARIAL.ttf");

bool debug_visual = false;

Vector2f view_offset;

void set_view_offset()
{
	view_offset = Vector2f(window->getSize().x / 2, window->getSize().y / 2);
    if (player.position.x > window->getSize().x / 2 and player.position.x < world_chunks.size() * 1024 - window->getSize().x / 2)
        view_offset.x = player.position.x;
    else if (player.position.x > world_chunks.size() * 1024 - window->getSize().x / 2)
		view_offset.x = world_chunks.size() * 1024 - window->getSize().x / 2;
    if (player.position.y > window->getSize().y / 2 and player.position.y < world_chunks[0].size() * 1024 - window->getSize().y / 2)
        view_offset.y = player.position.y;
    else if (player.position.y > world_chunks[0].size() * 1024 - window->getSize().y / 2)
        view_offset.y = world_chunks[0].size() * 1024 - window->getSize().y / 2;
    window->setView(View({ view_offset.x, view_offset.y}, Vector2f(window->getSize().x, window->getSize().y)));
	debug_text.push_back("Player offest: " + to_string(view_offset.x) + ", " + to_string(view_offset.y));
}


void create_instance()
{
    window->setKeyRepeatEnabled(false);
}

void render_player()
{
    player_model.setPosition({ player.position.x, player.position.y + player.jump_offset });
    window->draw(player_model);
    auto tile_below = world_chunks[player.chunk.x][player.chunk.y].changeables[player.sub_c.x][player.sub_c.y + 1];
    if (tile_below != NULL)
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
    CircleShape poss(2.f);
    poss.setFillColor(Color::Red);
    poss.setPosition({ player.position.x , player.position.y });
    window->draw(poss);
    for (auto cir : debug_draw)
    {
        window->draw(cir);
    }
    debug_draw.clear();
    for (int i = 0; i < debug_text.size(); i++)
    {
        float poss_y = player.position.y - i * 10 + 20;
        Text txt(font);
        txt.setString(debug_text[i]);
        txt.setPosition({ player.position.x , poss_y });
        txt.setFillColor(Color::Red);
        txt.setCharacterSize(15);
        window->draw(txt);
    }
    debug_text.clear();
}

void render()
{
    window->clear();
    set_view_offset();
    render_world();
    render_entities();
    render_player();
    if (debug_visual)
        render_debug();
    window->display();
}