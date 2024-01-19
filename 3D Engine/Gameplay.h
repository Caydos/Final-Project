#ifndef GAMEPLAY_H
#define GAMEPLAY_H
#include "Common.h"

namespace Gameplay
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp(void);
}

#endif // !GAMEPLAY_H