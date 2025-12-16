#include "window.hpp"
#include "renderer.hpp"
#include "input.hpp"
#include "world.hpp"

RenderWindow* window;
TypeMenuScreen titleScreen("Inkless");
TypeMenuScreen worldCreationScreen("Create World");
GameState currentState = GameState::TitleScreen;

//gui colors
Color baseButtonColor = Color(200, 200, 200);
Color baseHoveredColor = Color(80, 80, 80);
Color basePlaceholderTextColor = Color(100, 100, 100);

Texture gutter_point_texture, gutter_horizontal_texture, gutter_vertical_texture, gutter_up_left_texture, gutter_down_left_texture, gutter_branch_horizontal_down_texture, gutter_branch_horizontal_texture, gutter_branch_vertical_texture, gutter_up_texture, gutter_down_texture, gutter_left_texture;
Texture ground_texture, debugGroundTexture;
Texture rock_texture;
Texture player_texture;
Texture jump_texture;
Sprite player_model(player_texture);

void create_instance()
{
    window->setKeyRepeatEnabled(false);
}

void load_textures()
{
    //ground
    ground_texture.loadFromFile("ground3.png", false, IntRect({ 0, 0 }, { 128, 128 }));
    ground_texture.setRepeated(true);
    debugGroundTexture.loadFromFile("debug_ground.png", false, IntRect({ 0, 0 }, { 64, 64 }));
    debugGroundTexture.setRepeated(true);
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

Button::Button()
{
    box = RectangleShape({1.f, 1.f});
    text = Text(font, "Button", 1);
    box.setOutlineThickness(4.f);
}

Button::Button(Text txt, float x, float y)
{
    text = txt;
    box = RectangleShape(text.getLocalBounds().size + Vector2f(20, 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition({x, y});
    box.setOutlineThickness(4.f);
}

Button::Button(Text txt, Vector2f xy)
{
    text = txt;
	text.setOrigin(text.getLocalBounds().getCenter());
    box = RectangleShape(text.getLocalBounds().size + Vector2f(20, 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(xy);
    box.setOutlineThickness(4.f);
}

bool Button::checkOnBox()
{
    Vector2i mousePos = Mouse::getPosition(*window);
    FloatRect boxBounds = box.getGlobalBounds();
    if (mousePos.x >= boxBounds.position.x && mousePos.x <= boxBounds.position.x + boxBounds.size.x &&
        mousePos.y >= boxBounds.position.y && mousePos.y <= boxBounds.position.y + boxBounds.size.y)
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

TextBox::TextBox()
{
    box = RectangleShape({ 1.f, 1.f });
    box.setOutlineThickness(4.f);
}

TextBox::TextBox(float x, float y, string placeholderTxt, int charSize)
{
	placeholderText = Text(font, placeholderTxt, charSize);
    box = RectangleShape(Vector2f(800.f, placeholderText.getLocalBounds().size.y + 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition({ x, y });
    box.setOutlineThickness(4.f);
}

TextBox::TextBox(Vector2f xy, string placeHolderTxt, int charSize)
{
    placeholderText = Text(font, placeHolderTxt, charSize);
    box = RectangleShape(Vector2f(800, placeholderText.getLocalBounds().size.y + 20));
    box.setOrigin(box.getLocalBounds().getCenter());
    box.setPosition(xy);
    box.setOutlineThickness(4.f);
}

bool TextBox::checkOnBox()
{
    Vector2i mousePos = Mouse::getPosition(*window);
    FloatRect boxBounds = box.getGlobalBounds();
    if (mousePos.x >= boxBounds.position.x && mousePos.x <= boxBounds.position.x + boxBounds.size.x &&
        mousePos.y >= boxBounds.position.y && mousePos.y <= boxBounds.position.y + boxBounds.size.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void TextBox::draw()
{
    box.setFillColor(boxColor);
    box.setOutlineColor(baseHoveredColor);
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
	draw();
    for (auto& button : buttons)
    {
        if (button.checkOnBox() && mouse_1)
        {
            button.action();
        }
        
    }
    for (auto& textBox : textBoxes)
    {
        if (textBox.checkOnBox() && mouse_1)
        {
            textBox.selected = true;
        }
        else if (mouse_1)
        {
            textBox.selected = false;
        }
        
		if (!textBox.selected) continue;


        while (const optional event = window->pollEvent())
        {
            if (const auto* textEntered = event->getIf<sf::Event::TextEntered>())
            {
				if (textEntered->unicode >= 32 && textEntered->unicode <= 126 && !textBox.onlyInt) // normal characters
                    textBox.text += static_cast<char>(textEntered->unicode);

                else if (textBox.onlyInt && textEntered->unicode >= 48 && textEntered->unicode <= 57) // only allow digits
                    textBox.text += static_cast<char>(textEntered->unicode);

                if (textEntered->unicode == 8 && !textBox.text.empty()) //backspace
					textBox.text.pop_back();

				if (textEntered->unicode == 27 && !textBox.onlyInt) //escape
					textBox.selected = false;

            }
        }

	}
}


void createTypeMenuScreens()
{

    Vector2f windowMiddle = Vector2f(window->getSize().x / 2, window->getSize().y / 2);

    titleScreen.buttons.push_back(Button(Text(font, "New Game", 30), Vector2f(windowMiddle.x, windowMiddle.y - 50)));
    titleScreen.buttons[0].action = []()
        {
            sleep(milliseconds(100));
			currentState = GameState::WorldCreation;
	    };
    titleScreen.buttons.push_back(Button(Text(font, "Load Game", 30), Vector2f(windowMiddle.x, windowMiddle.y)));
    titleScreen.buttons[1].action = []()
        {
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
            currentState = GameState::TitleScreen;
        };
}