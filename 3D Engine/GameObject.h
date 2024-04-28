#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H
#include "Common.h"
#include "Set.h"
#include "Clock.h"

namespace GameObjects
{
	struct Object
	{
		Sets::Set* set;
		float range;
		std::string interactionName;
		// Cooldown in ms
		float cooldown;
		Clock clock;
		void (*hoveredFunction)();
		void (*interactFunction)(Sets::Set*_set);
	};
	void Register(Sets::Set* _set, float _range, float _cooldown, void (*_hoveredFunction)(), void (*_interactFunction)(Sets::Set* _set));
	void UnRegister(Sets::Set* _set);
	void Tick(GameData* _gameData);
}

#endif // !GAME_OBJECT_H