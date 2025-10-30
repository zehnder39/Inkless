#include <SFML/Graphics.hpp>
#include <iostream>

#include "header.hpp"

Texture gutter_point_texture, gutter_horizontal_texture, gutter_vertical_texture, gutter_up_left_texture, gutter_down_left_texture, gutter_branch_horizontal_down_texture, gutter_branch_horizontal_texture, gutter_branch_vertical_texture, gutter_up_texture, gutter_down_texture, gutter_left_texture;
Texture ground_texture;
Texture rock_texture;
Texture player_texture;
Texture jump_texture;
Sprite player_model(player_texture);

vector<CircleShape> debug_draw;
vector<string> debug_text, debug_info;

Font font("ARIAL.ttf");

vector<CircleShape> entities;

Player player(960, 540);
CircleShape entity1(100.f);
RectangleShape ground({ 1024.f, 768.f });
RenderWindow* window = nullptr;

Vector2f view_offset = {0.f, 0.f};
Vector2f player_local_poss;


bool debug_visual = false;


void load_textures()
{
    //ground
    ground_texture.loadFromFile("ground3.png", false, IntRect({ 0, 0 }, { 128, 128 }));
    ground_texture.setRepeated(true);
    //player
    player_texture.loadFromFile("squid_player.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    jump_texture.loadFromFile("squid_jump.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    player_model.setTexture(player_texture);
    player_model.setTextureRect(IntRect({ 0, 0 }, { 32, 32 }));
    player_model.setScale({ 2.f, 2.f });
    //rock
    rock_texture.loadFromFile("rock2.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    //gutter
    gutter_horizontal_texture.loadFromFile("gutter2.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_point_texture.loadFromFile("gutter_point.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_vertical_texture.loadFromFile("gutter_vertical.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_up_texture.loadFromFile("gutter_up.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_horizontal_texture.loadFromFile("gutter.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_point_texture.loadFromFile("gutter_point.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_up_left_texture.loadFromFile("gutter_up_left.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_left_texture.loadFromFile("gutter_left.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_down_left_texture.loadFromFile("gutter_down_left.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_down_texture.loadFromFile("gutter_down.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_branch_vertical_texture.loadFromFile("gutter_branch_vertical.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_branch_horizontal_texture.loadFromFile("gutter_branch_horizontal.png", false, IntRect({ 0, 0 }, { 32, 32 }));
    gutter_branch_horizontal_down_texture.loadFromFile("gutter_branch_horizontal_down.png", false, IntRect({ 0, 0 }, { 32, 32 }));
}

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
    if (player.subc.y != 15)
        tile_below = get_tile(player.chunk, { player.subc.x, player.subc.y + 1 });
    else
        tile_below = get_tile({ player.chunk.x, player.chunk.y - 1 }, { player.subc.x, 0 });
    if (tile_below != nullptr)
    {
        tile_below->draw();
        CircleShape cir(30.f);
        cir.setFillColor(Color::Green);
        cir.setPosition(Vector2f(tile_below->chunk.x * (tile_size.x * 16) + tile_below->subc.x * tile_size.x, tile_below->chunk.y * (tile_size.y * 16) + tile_below->subc.y * tile_size.y));
        debug_draw.push_back(cir);
    }
}

void render_entities()
{
    for (CircleShape entity : entities) {
        window->draw(entity);
    }
}

void render_world()
{
    // draw all the nearby chunk floor then the tiles on top to avoid superposition
    for (int i = 0; i < int(world_chunks.size()); i++)
    {
        for (Chunk& current : world_chunks[i])
        {
            window->draw(current.ground);
        }
    }
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
            float pos_x = view_offset.x + (window->getSize().x / 2) - (debug_info[i].length() * txt_size / 2);
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