#include "craft.hpp"
#include "texture.hpp"
#include "input.hpp"
#include "window.hpp"

unordered_map<string, CraftDef>& CraftManager::craftMap = *(new unordered_map<string, CraftDef>);

void defineCrafts()
{
	CraftManager::registerCraft("stoneCooking", {
		"Stone Cooking",
		"Cook a stone to duplicate it.",
		"items/resources/stone",
		120,
		{
			{"stone", 1}
		},
		{
			{"stone", 2}
		}
		});
}

void CraftSelector::draw()
{
	if (!open) return;
	// Draw background
	allowedCrafts.clear();
	for (const string& craftName : machine->allowedCrafts)
	{
		allowedCrafts.push_back(craftName);
	}
	RectangleShape background(Vector2f(CraftSelector::maxWidth * 32 * guiScale, CraftSelector::maxHeight * 32 * guiScale));
	background.setFillColor(Color(50, 50, 50, 200));
	background.setPosition(window->getView().getCenter() - Vector2f(background.getSize().x / 2, background.getSize().y / 2));
	window->draw(background);
	
	vector<IconButton> buttons;
	for (size_t i = 0; i < allowedCrafts.size(); i++)
	{
		const string craft = allowedCrafts[i];
		const string texture = CraftManager::get(craft).textureName;
		buttons.push_back(IconButton(texture, background.getPosition() + Vector2f((i % CraftSelector::maxWidth) * 32 * guiScale, (i / CraftSelector::maxWidth) * 32 * guiScale)));
	}
	for (auto& button : buttons)
	{
		button.draw();
		if (button.checkOnBox() && MouseInputManager::isActionTapped("break"))
		{
			machine->setCraft(allowedCrafts[&button - &buttons[0]]);
			open = false;
			return;
		}
	}
}

void CraftManager::registerCraft(string name, CraftDef def) {
	craftMap[name] = def;
}

const CraftDef& CraftManager::get(string name) {
	return craftMap.at(name);
}