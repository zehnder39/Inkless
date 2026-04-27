#include <SFML/Graphics.hpp>
#include <iostream>

#include "renderer.hpp"
#include "definer.hpp"
#include "world.hpp"
#include "input.hpp"
#include "window.hpp"
#include "physics.hpp"
#include "texture.hpp"
#include "player.hpp"
#include "items.hpp"
#include "entities.hpp"
#include "tiles.hpp"

vector<CircleShape> debug_draw;
vector<string> debug_text, debug_info;

Font font("ARIAL.ttf");

CircleShape entity1(100.f);
RectangleShape ground({ 1024.f, 768.f });

Vector2f view_offset = {0.f, 0.f};
Vector2f player_local_pos;


bool debug_visual = false;


void set_view_offset()
{
    Vector2i window_size = Vector2i(window->getSize());
    view_offset = Vector2f(window_size.x / 2, window_size.y / 2);

    if (player.position.x > window_size.x / 2 and player.position.x < world_chunks.size() * (tile_size.x * 16) - window_size.x / 2)
        view_offset.x = player.position.x;
    else if (player.position.x > world_chunks.size() * (tile_size.x * 16) - window_size.x / 2)
        view_offset.x = world_chunks.size() * (tile_size.x * 16) - window_size.x / 2;

    if (player.position.y > window_size.y / 2 and player.position.y < world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2)
        view_offset.y = player.position.y;
    else if (player.position.y > world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2)
        view_offset.y = world_chunks[0].size() * (tile_size.y * 16) - window_size.y / 2;
    window->setView(View({ view_offset.x, view_offset.y }, Vector2f(window_size)));

	debug_text.push_back("Player offset: " + to_string(view_offset.x) + ", " + to_string(view_offset.y));
    player_local_pos = { player.position.x - view_offset.x + window_size.x / 2, player.position.y - view_offset.y + window_size.y / 2 };
}

void renderTileBelowOnTop(Tile* tile)
{
    if (tile == nullptr)
        return;
    if (!tile->solid)
        return;
    tile->draw();
    Tile* tileBelow = getTile(tile->chunk, { tile->subc.x, tile->subc.y + 1 });
    renderTileBelowOnTop(tileBelow);
}

void render_player()
{
    player.model.setPosition({ player.position.x, player.position.y + player.animation_offset });
    window->draw(player.model);
    Tile* tileBelow = getTile(player.chunk, { player.subc.x, player.subc.y + 1 });
    renderTileBelowOnTop(tileBelow);
}

void render_entities()
{
    for (const auto& entity : entities) {
        entity->draw();
    }
}

void render_world()
{
    // draw all the nearby chunk floor then the tiles on top to avoid superposition
    for (int i = 0; i < int(world_chunks.size()); i++)
    {
        for (Chunk& current : world_chunks[i])
        {
            if (!debug_visual)
                window->draw(current.ground);
            else
                window->draw(current.debugGround);
        }
    }
    for (int i = 0; i < int(world_chunks.size()); i++)
    {
        for (Chunk& current : world_chunks[i])
        {
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
        int txt_size = 20;
        for (auto cir : debug_draw)
        {
            window->draw(cir);
        }
        for (int i = 0; i < int(debug_text.size()); i++)
        {
            float poss_y = player.position.y + i * txt_size;
            Text txt(font);
            txt.setString(debug_text[i]);
            txt.setPosition({ player.position.x , poss_y });
            txt.setFillColor(Color::Red);
            txt.setCharacterSize(txt_size);
            window->draw(txt);
        }
        for (int i = 0; i < int(debug_info.size()); i++)
        {
            float pos_x = view_offset.x + (window->getSize().x / 2) - (debug_info[i].length() * txt_size * 3 / 4);
            float pos_y = view_offset.y - (window->getSize().y / 2) + (i * txt_size);
            Text txt(font);
            txt.setString(debug_info[i]);
            txt.setPosition({ pos_x , pos_y });
            txt.setFillColor(Color::Red);
            txt.setCharacterSize(txt_size);
            window->draw(txt);
        }
    }
	debug_draw.clear();
    debug_text.clear();
    debug_info.clear();
}

void renderMenus()
{
    if (gamePaused)
    {
        RectangleShape overlay(Vector2f(window->getSize()));
        overlay.setFillColor(Color(0, 0, 0, 150));
        overlay.setPosition(window->getView().getCenter() - Vector2f(window->getSize().x / 2, window->getSize().y / 2));
        window->draw(overlay);
        pauseMenu.update();
    }
    if (player.state == playerState::inventory)
    {
        if (player.container)
        {
            int xPlayer = view_offset.x - player.inventory.getWidth();
            player.inventory.draw(xPlayer);
            int xContainer = view_offset.x;
            player.container->drawInterface(xContainer);
            if (CraftSelector::open)
            {
				CraftSelector::draw();
            }
        }
        else
        {
            int x = view_offset.x - player.inventory.getWidth() / 2;
            player.inventory.draw(x);
        }
    }
}

void render()
{
    window->clear();
    set_view_offset();
    render_world();
    render_entities();
    render_player();
    render_debug();
    renderMenus();
    window->display();
}