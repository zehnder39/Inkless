#pragma once

#include <string>
#include <vector>
#include <array>
#include <utility>
#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <unordered_map>

using namespace std;
using namespace sf;

extern Texture placeHolderTexture;

void load_textures();

class TextureManager {
private:
    // On déplace la map dans une fonction privée statique
    static unordered_map<string, Texture>& getMap() {
        static unordered_map<string, Texture> textureMap; // Créée UNIQUEMENT au premier appel
        return textureMap;
    }

public:
    static Texture& get(const string& name) {
        // On utilise getMap() au lieu de la variable directe
        return getMap().at(name);
    }

    static void loadTexture(const string& name, const string& filePath) {
        Texture texture;
        if (texture.loadFromFile(filePath)) {
            getMap()[name] = move(texture);
        }
		else cout << "Failed to load texture: " << filePath << endl;
    }
};
