#ifndef LEVELS_H
#define LEVELS_H
#include "Common.h"
#include "Entrance.h"
#include "Hospital.h"
#include "Library.h"
#include "Garden.h"

namespace Levels
{
	enum Level
	{
		ENTRANCE,
		HOSPITAL,
		LIBRARY,
		GARDEN,

		LEVELCOUNT
	};

	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	Level Get();
	void Set(Level _level);
}

#endif // !LEVELS_H