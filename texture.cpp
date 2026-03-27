#include "texture.hpp"
#include "world.hpp"
#include "player.hpp"

Texture placeHolderTexture;

void load_textures()
{
    //ground
    TextureManager::loadTexture("ground/ground3", "ground3.png");
    TextureManager::get("ground/ground3").setRepeated(true);
    TextureManager::loadTexture("ground/debug", "debug_ground.png");
    TextureManager::get("ground/debug").setRepeated(true);
    //player
    TextureManager::loadTexture("player/standing", "squid_player.png");
    TextureManager::loadTexture("player/jumping", "squid_jump.png");
    player.model.setTexture(TextureManager::get("player/standing"));
    player.model.setTextureRect(IntRect({ 0, 0 }, { 32, 32 }));
    player.model.setScale({ 2.f, 2.f });
    //rock
    TextureManager::loadTexture("assets/passive/rock", "rock2.png");
    //gutter
    TextureManager::loadTexture("assets/passive/gutter/horizontal", "gutter2.png");
    TextureManager::loadTexture("assets/passive/gutter/point", "gutter_point.png");
    TextureManager::loadTexture("assets/passive/gutter/vertical", "gutter_vertical.png");
    TextureManager::loadTexture("assets/passive/gutter/up", "gutter_up.png");
    TextureManager::loadTexture("assets/passive/gutter/left", "gutter_left.png");
    TextureManager::loadTexture("assets/passive/gutter/up_left", "gutter_up_left.png");
    TextureManager::loadTexture("assets/passive/gutter/down_left", "gutter_down_left.png");
    TextureManager::loadTexture("assets/passive/gutter/down", "gutter_down.png");
    TextureManager::loadTexture("assets/passive/gutter/branch_vertical_right", "gutter_branch_vertical.png");
    TextureManager::loadTexture("assets/passive/gutter/branch_horizontal_up", "gutter_branch_horizontal.png");
    TextureManager::loadTexture("assets/passive/gutter/branch_horizontal_down", "gutter_branch_horizontal_down.png");
    //items
	TextureManager::loadTexture("items/resources/stone", "stone_item.png");
}