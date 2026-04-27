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
pair<pair<Vector2i, Vector2i>, bool> tileLookingAt();
pair<pair<Vector2i, Vector2i>, bool> InRange(Vector2i chunk, Vector2i subc);
Vector2f chunkSubcToPos(Vector2i chunk, Vector2i subc);
float dist(Vector2f a, Vector2f b);

template<typename T, typename V>
void removeFromVector(vector<unique_ptr<T>>& vec, V* target) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i].get() == target) {
            if (vec[i] == vec.back())
            {
                vec.pop_back();
                return;
            }
            vec[i] = move(vec.back());
            vec.pop_back();

            i--;
        }
    }
}
template<typename T, typename V>
void removeFromVector(vector<T>& vec, V target) {
    for (int i = 0; i < vec.size(); i++) {
        if (vec[i] == target) {
            if (vec[i] == vec.back())
            {
                vec.pop_back();
                return;
            }
            vec[i] = move(vec.back());
            vec.pop_back();

            i--;
        }
    }
}

