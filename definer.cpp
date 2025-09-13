#include "header.hpp"


bool moving_left = false, moving_right = false, moving_up = false, moving_down = false;
bool should_close = false;
vector<CircleShape> entities;

Player player(0, 0);
CircleShape player_model(7.f);
CircleShape entity1(100.f);
RectangleShape ground({1024.f, 1024.f});
RenderWindow* window = nullptr;
Texture ground_texture;