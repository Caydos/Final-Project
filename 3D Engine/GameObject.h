#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Common.h"
#include "Set.h"

namespace GameObjects
{
	struct Object
	{
		Sets::Set* set;
		float range;
	};
	void Register(Sets::Set* _set, float _range);
	void UnRegister(Sets::Set* _set);
	void Tick(GameData* _gameData);
}

#endif // !GAME_OBJECT_H