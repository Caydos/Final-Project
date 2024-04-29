#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "Common.h"

namespace MainMenu
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
	void Open();
	void Close();
}

#endif // !MAIN_MENU_H