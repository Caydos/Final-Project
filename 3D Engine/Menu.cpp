#include "Menu.h"


void Menu::Tick(GameData* _gameData)
{
	MainMenu::Tick(_gameData);
	PauseMenu::Tick(_gameData);
	MenuOptions::Tick(_gameData);
}
