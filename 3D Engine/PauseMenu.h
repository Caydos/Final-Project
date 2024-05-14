#ifndef PAUSEMENU_H
#define PAUSEMENU_H
#include "Common.h"

namespace PauseMenu
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
	void Open();
	void Close();
	bool GetOpen();
	void SetOpen(bool _open);
}

#endif // !PAUSEMENU_H