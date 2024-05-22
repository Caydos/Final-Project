#ifndef GARDEN_H
#define GARDEN_H

#include "Common.h"
#include "Scripting.h"


namespace Garden
{
	void RegisterInteractions();
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	void InteractExit(Sets::Set* _set);

	void CleanUp(GameData* _gameData);
}

#endif // !GARDEN_H