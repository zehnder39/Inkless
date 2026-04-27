#include "window.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "world.hpp"
#include "saves.hpp"
#include "physics.hpp"
#include "texture.hpp"

RenderWindow* window;
TypeMenuScreen titleScreen("Inkless");
TypeMenuScreen worldCreationScreen("Create World");
TypeMenuScreen worldLoadingScreen("Load World");
TypeScreenLockedMenu pauseMenu("Paused");
GameState currentState = GameState::TitleScreen;

//flags
bool shouldClose;

//gui colors
Color baseButtonColor = Color(200, 200, 200);
Color baseHoveredColor = Color(80, 80, 80);
Color basePlaceholderTextColor = Color(100, 100, 100);

void create_instance()
{
    window->setKeyRepeatEnabled(false);
}

Button::Button()
{
    box = RectangleShape({1.f, 1.f});
    text = Text(font, "Button", 1);
    box.setOutlineThickness(4.f);
}

Button::Button(Text txt, float x, float y)
{
	localCenterOffset = Vector2f(x, y);
    text = txt;
    box = RectangleShape(text.getLocalBounds().size + Vector2f(20, 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setOutlineThickness(4.f);
}

Button::Button(Text txt, Vector2f xy)
{
	localCenterOffset = xy;
    text = txt;
	text.setOrigin(text.getLocalBounds().getCenter());
    box = RectangleShape(text.getLocalBounds().size + Vector2f(20, 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setOutlineThickness(4.f);
}

bool Button::checkOnBox()
{
    FloatRect boxBounds = box.getGlobalBounds();
    Vector2f boxPosition = boxBounds.position;
    if (MouseInputManager::globalPosition.x >= boxPosition.x && MouseInputManager::globalPosition.x <= boxPosition.x + boxBounds.size.x &&
        MouseInputManager::globalPosition.y >= boxPosition.y && MouseInputManager::globalPosition.y <= boxPosition.y + boxBounds.size.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Button::draw()
{
	setGlobalPosition();
    if (checkOnBox())
    {
        box.setFillColor(hoveredColor);
        box.setOutlineColor(buttonColor);
    }
    else
    {
        box.setFillColor(buttonColor);
		box.setOutlineColor(hoveredColor);
    }
    window->draw(box);
    text.setPosition(box.getPosition());
    window->draw(text);
}

void Button::setGlobalPosition()
{
    box.setPosition(localCenterOffset + window->getView().getCenter());
}


IconButton::IconButton(string texName, float x, float y)
{
    textureName = texName;
    box = RectangleShape(Vector2f( 32 * guiScale, 32 * guiScale ));
    box.setOutlineThickness(4.f);
    box.setPosition(Vector2f(x, y));
}

IconButton::IconButton(string texName, Vector2f xy)
{
    textureName = texName;
    box = RectangleShape(Vector2f(32 * guiScale, 32 * guiScale));
    box.setOutlineThickness(4.f);
    box.setPosition(xy);
}

void IconButton::draw()
{
    if (checkOnBox())
    {
        box.setFillColor(hoveredColor);
        box.setOutlineColor(buttonColor);
    }
    else
    {
        box.setFillColor(buttonColor);
        box.setOutlineColor(hoveredColor);
    }
    Sprite sprite(TextureManager::get(textureName));
    sprite.setPosition(box.getPosition());
    sprite.scale(Vector2f(guiScale, guiScale));
    window->draw(box);
    window->draw(sprite);
}

bool IconButton::checkOnBox()
{
    Vector2f boxPosition = box.getGlobalBounds().position;
    FloatRect boxBounds = box.getGlobalBounds();
    if (MouseInputManager::globalPosition.x >= boxPosition.x && MouseInputManager::globalPosition.x <= boxPosition.x + boxBounds.size.x &&
        MouseInputManager::globalPosition.y >= boxPosition.y && MouseInputManager::globalPosition.y <= boxPosition.y + boxBounds.size.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

TextBox::TextBox()
{
    box = RectangleShape({ 1.f, 1.f });
    box.setOutlineThickness(4.f);
}

TextBox::TextBox(float x, float y, string placeholderTxt, int charSize)
{
	localCenterOffset = Vector2f(x, y);
	placeholderText = Text(font, placeholderTxt, charSize);
    box = RectangleShape(Vector2f(800.f, placeholderText.getLocalBounds().size.y + 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setOutlineThickness(4.f);
}

TextBox::TextBox(Vector2f xy, string placeHolderTxt, int charSize)
{
	localCenterOffset = xy;
    placeholderText = Text(font, placeHolderTxt, charSize);
    box = RectangleShape(Vector2f(800, placeholderText.getLocalBounds().size.y + 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setOutlineThickness(4.f);
}

bool TextBox::checkOnBox()
{
    Vector2f boxPosition = box.getGlobalBounds().position;
    FloatRect boxBounds = box.getGlobalBounds();
    if (MouseInputManager::globalPosition.x >= boxPosition.x && MouseInputManager::globalPosition.x <= boxPosition.x + boxBounds.size.x &&
        MouseInputManager::globalPosition.y >= boxBounds.position.y && MouseInputManager::globalPosition.y <= boxBounds.position.y + boxBounds.size.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TextBox::write()
{
    while (const optional event = window->pollEvent())
    {
        if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
        {
            if (textEntered->unicode >= 32 && textEntered->unicode <= 126 && !onlyInt) // normal characters
                text += static_cast<char>(textEntered->unicode);

            else if (onlyInt && textEntered->unicode >= 48 && textEntered->unicode <= 57) // only allow digits
                text += static_cast<char>(textEntered->unicode);

            if (textEntered->unicode == 8 && !text.empty()) //backspace
                text.pop_back();

            if (textEntered->unicode == 27) //escape
                selected = false;

        }
    }
}

void TextBox::draw()
{
    box.setFillColor(boxColor);
    box.setOutlineColor(baseHoveredColor);
	setGlobalPosition();
    window->draw(box);
    if (selected)
    {
        box.setOutlineColor(Color::Red);
        window->draw(box);
    }
    if (text.empty())
    {
        placeholderText.setFillColor(basePlaceholderTextColor);
		placeholderText.setOrigin(placeholderText.getLocalBounds().getCenter());
        placeholderText.setPosition(box.getPosition());
        window->draw(placeholderText);
    }
    else
    {
        Text enteredText(font, text, placeholderText.getCharacterSize());
        enteredText.setFillColor(baseButtonColor);
		enteredText.setOrigin(enteredText.getLocalBounds().getCenter());
        enteredText.setPosition(box.getPosition());
        window->draw(enteredText);
    }
}

void TextBox::setGlobalPosition()
{
    box.setPosition(localCenterOffset + window->getView().getCenter());
}

void TypeMenuScreen::draw()
{
    window->clear(backgroundColor);
    Text titleText(font, title, 120);
    titleText.setFillColor(Color::Black);
    Vector2f size = titleText.getLocalBounds().size;
    titleText.setPosition(Vector2f((window->getSize().x / 2 - size.x / 2), window->getSize().y / 4 - size.y / 2));
    for (auto& button : buttons)
    {
		button.draw();
    }
    for (auto& textBox : textBoxes)
    {
        textBox.draw();
    }
    window->draw(titleText);
    window->display();
}

void TypeMenuScreen::update()
{
	view_offset = Vector2f(window->getSize().x / 2, window->getSize().y / 2);
    window->setView(View(view_offset, Vector2f(window->getSize())));

    if (KeyInputManager::isActionTapped("escape"))
        currentState = GameState::TitleScreen;

	draw();
    for (auto& button : buttons)
    {
        if (button.checkOnBox() && MouseInputManager::isActionTapped("break"))
        {
            button.action();
        }
        
    }
    for (auto& textBox : textBoxes)
    {
        if (textBox.checkOnBox() && MouseInputManager::isActionTapped("break"))
        {
            textBox.selected = true;
        }
        else if (MouseInputManager::isActionTapped("break"))
        {
            textBox.selected = false;
        }
        
		if (!textBox.selected) continue;

        textBox.write();
	}
}

void TypeScreenLockedMenu::draw()
{
    Vector2f windowMiddle = Vector2f(window->getSize().x / 2, window->getSize().y / 2);
	
    Text titleText(font, title, 100);
    titleText.setFillColor(Color::Black);
    Vector2f size = titleText.getLocalBounds().size;
    titleText.setPosition(Vector2f((window->getView().getCenter().x - size.x / 2), window->getView().getCenter().y - window->getSize().y / 4 - size.y / 2));
    for (auto& button : buttons)
    {
        button.draw();
    }
    for (auto& textBox : textBoxes)
    {
        textBox.draw();
    }
    window->draw(titleText);
    window->display();
	};

void TypeScreenLockedMenu::update()
{
    draw();
    for (auto& button : buttons)
    {
        if (button.checkOnBox() && MouseInputManager::isActionTapped("break"))
        {
            button.action();
        }
    }
    for (auto& textBox : textBoxes)
    {
        if (textBox.checkOnBox() && MouseInputManager::isActionTapped("break"))
        {
            textBox.selected = true;
        }
        else if (MouseInputManager::isActionTapped("break"))
        {
            textBox.selected = false;
        }
        if (!textBox.selected) continue;

		textBox.write();
    }
}


void createMenus()
{

    Vector2f windowMiddle = Vector2f( 0, 0);

    titleScreen.buttons.push_back(Button(Text(font, "New Game", 30), Vector2f(windowMiddle.x, windowMiddle.y - 50)));
    titleScreen.buttons[0].action = []()
        {
            sleep(milliseconds(100));
			currentState = GameState::WorldCreation;
	    };
    titleScreen.buttons.push_back(Button(Text(font, "Load Game", 30), Vector2f(windowMiddle.x, windowMiddle.y)));
    titleScreen.buttons[1].action = []()
        {
			sleep(milliseconds(100));
            currentState = GameState::WorldLoading;
        };
    titleScreen.buttons.push_back(Button(Text(font, "Quit", 30), Vector2f(windowMiddle.x, windowMiddle.y + 50)));
    titleScreen.buttons[2].action = []()
        {
            window->close();
        };
	worldCreationScreen.textBoxes.push_back(TextBox(Vector2f(windowMiddle.x, windowMiddle.y - 50), "World Name", 30));
    worldCreationScreen.textBoxes.push_back(TextBox(Vector2f(windowMiddle.x, windowMiddle.y + 50), "Seed", 30));
	worldCreationScreen.textBoxes[1].onlyInt = true;
    worldCreationScreen.buttons.push_back(Button(Text(font, "Create World", 30), Vector2f(windowMiddle.x, windowMiddle.y + 150)));
    worldCreationScreen.buttons[0].action = []()
        {
            string worldName = worldCreationScreen.textBoxes[0].text;
            if (worldName.empty())
				worldName = "New World";
            int worldSeed;
            if (worldCreationScreen.textBoxes[1].text.empty())
                worldSeed = time(nullptr);
            else
                worldSeed = stoi(worldCreationScreen.textBoxes[1].text);
            create_world(worldName, worldSeed);
			currentState = GameState::InGame;
        };
    worldCreationScreen.buttons.push_back(Button(Text(font, "Cancel", 30), Vector2f(windowMiddle.x, windowMiddle.y + 200)));
    worldCreationScreen.buttons[1].action = []()
        {
			sleep(milliseconds(100));
            currentState = GameState::TitleScreen;
        };
    for (const auto& entry : fs::directory_iterator("saves"))
    {
		worldLoadingScreen.buttons.push_back(Button(Text(font, entry.path().stem().string(), 30), Vector2f(windowMiddle.x, windowMiddle.y + worldLoadingScreen.buttons.size() * 60 - 100)));
        worldLoadingScreen.buttons.back().action = [entry]()
            {
                world_name = entry.path().stem().string();
                load_world();
                currentState = GameState::InGame;
            };
    }
	pauseMenu.buttons.push_back(Button(Text(font, "Resume", 30), Vector2f(windowMiddle.x, windowMiddle.y - 50)));
    pauseMenu.buttons[0].action = []()
        {
            gamePaused = false;
		};
    pauseMenu.buttons.push_back(Button(Text(font, "Save & Quit", 30), Vector2f(windowMiddle.x, windowMiddle.y)));
    pauseMenu.buttons[1].action = []()
        {
            save_world();
			gamePaused = false;
			sleep(milliseconds(100));
			currentState = GameState::TitleScreen;
        };
    pauseMenu.buttons.push_back(Button(Text(font, "Fast Quit", 30), Vector2f(windowMiddle.x, windowMiddle.y + 50)));
    pauseMenu.buttons[2].action = []()
        {
            gamePaused = false;
			sleep(milliseconds(100));
            currentState = GameState::TitleScreen;
        };
}