#include <SFML/Graphics.hpp>
#include <numbers>
#include <iostream>
#include <cmath>
#include <algorithm>

#include "renderer.hpp"
#include "definer.hpp"
#include "input.hpp"
#include "window.hpp"
#include "world.hpp"
#include "physics.hpp"
#include "player.hpp"

//flags
bool facing_left;

list<sf::Keyboard::Key> KeyInputManager::keyPressedThisFrame;
list<sf::Keyboard::Key> KeyInputManager::keyTapped;

list<sf::Mouse::Button> MouseInputManager::buttonTapped;
list<sf::Mouse::Button> MouseInputManager::buttonPressedThisFrame;
Vector2f MouseInputManager::vector;
Vector2i MouseInputManager::globalPosition;
Vector2i MouseInputManager::localPosition;

void KeyInputManager::update(const optional<Event> event)
{
    if (const auto* key = event->getIf<Event::KeyPressed>())
    {
        keyPressedThisFrame.push_back(key->code);
		keyTapped.push_back(key->code);
    }
    if (const auto* key = event->getIf<Event::KeyReleased>())
    {
		keyPressedThisFrame.remove(key->code);
    }
}

bool KeyInputManager::isActionActive(string action)
{
	Keyboard::Key key = KeyInputManager::keyBinds.at(action);
    if (find(keyPressedThisFrame.begin(), keyPressedThisFrame.end(), key) != keyPressedThisFrame.end())
		return true;
    return false;
}

bool KeyInputManager::isActionTapped(string action) {
    Keyboard::Key key = keyBinds.at(action);
    return find(keyTapped.begin(), keyTapped.end(), key) != keyTapped.end();
}

void MouseInputManager::update(const optional<Event> event)
{
    
    if (const auto* mouseEvent = event->getIf<Event::MouseButtonPressed>())
    {
        buttonPressedThisFrame.push_back(mouseEvent->button);
        buttonTapped.push_back(mouseEvent->button);
    }
    if (const auto* mouseEvent = event->getIf<Event::MouseButtonReleased>())
    {
        buttonPressedThisFrame.remove(mouseEvent->button);
    }
}

bool MouseInputManager::isActionActive(string action)
{
    Mouse::Button button = MouseInputManager::keyBinds.at(action);
    if (find(buttonPressedThisFrame.begin(), buttonPressedThisFrame.end(), button) != buttonPressedThisFrame.end())
        return true;
    return false;
}

bool MouseInputManager::isActionTapped(string action)
{
    Mouse::Button button = MouseInputManager::keyBinds.at(action);
    return find(buttonTapped.begin(), buttonTapped.end(), button) != buttonTapped.end();
}


void input()
{
    KeyInputManager::keyTapped.clear();
    MouseInputManager::buttonTapped.clear();
    while (const optional event = window->pollEvent())
    {
        if (event->is<Event::Closed>())
            shouldClose = true;
        MouseInputManager::update(event);
        KeyInputManager::update(event);
    }
    MouseInputManager::localPosition = Vector2i(Mouse::getPosition(*window).x, Mouse::getPosition(*window).y);
    MouseInputManager::globalPosition = Vector2i(Mouse::getPosition(*window).x + view_offset.x - window->getSize().x / 2, Mouse::getPosition(*window).y + view_offset.y - window->getSize().y / 2);
    MouseInputManager::vector = Vector2f(MouseInputManager::globalPosition.x - player.position.x, -MouseInputManager::globalPosition.y + player.position.y);
    
}

void Gutter::interact()
{
	player.state = playerState::swimming;
    Vector2f tile_pos = chunkSubcToPos(chunk, subc);
    player.position = Vector2f(tile_pos.x + center.x, tile_pos.y + center.y);
    player.updateChunkSubc();
    player.gutterPathing.swimmingPath = make_pair(player.position, player.position);
    player.gutterPathing.swimmingAdvDown = false;
    player.gutterPathing.swimmingAdvLeft = false;
    player.gutterPathing.swimmingAdvRight = false;
    player.gutterPathing.swimmingAdvUp = false;
}