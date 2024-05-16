#ifndef LIBRARY_H
#define LIBRARY_H

#include "Common.h"
#include "Scripting.h"


namespace Library
{
	void RegisterInteractions();
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	void CleanUp(GameData* _gameData);
}

#endif // !LIBRARY_H