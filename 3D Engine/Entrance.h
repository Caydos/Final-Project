#ifndef ENTRANCE_H
#define ENTRANCE_H

#include "Common.h"
#include "Scripting.h"


namespace Entrance
{
	void RegisterInteractions();
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	void CleanUp(GameData* _gameData);
}

#endif // !ENTRANCE_H