#ifndef MAIN_MENU_H
#define MAIN_MENU_H
#include "Common.h"

namespace MainMenu
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
	bool IsOpen();
	void Open();
	void Close();
	bool GetOpen();
	void SetOpen(bool _open);
}

#endif // !MAIN_MENU_H