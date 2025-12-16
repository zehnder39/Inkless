#pragma once

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>

using namespace std;
using namespace sf;

pair<pair<Vector2i, Vector2i>, bool> pos_to_chunk_subc(Vector2f pos);
pair<pair<Vector2i, Vector2i>, bool> tile_looking_at();
pair<pair<Vector2i, Vector2i>, bool> InRange(Vector2i chunk, Vector2i subc);
Vector2f chunk_subc_to_pos(Vector2i chunk, Vector2i subc);
float dist(Vector2f a, Vector2f b);

