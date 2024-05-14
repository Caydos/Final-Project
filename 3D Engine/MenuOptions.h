#ifndef MENU_OPTIONS_H
#define MENU_OPTIONS_H
#include "Common.h"

namespace MenuOptions
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
	bool IsOpen();
	void Open();
	void Close();
}

#endif // !MENU_OPTIONS_H