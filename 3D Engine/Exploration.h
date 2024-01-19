#ifndef EXPLORATION_H
#define EXPLORATION_H
#include "Common.h"

namespace Exploration
{
	void Initialize(GameData* _gameData);
	void Inputs(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
}

#endif // !EXPLORATION_H