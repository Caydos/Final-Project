#ifndef SCRIPTING_H
#define SCRIPTING_H
#include "Common.h"
#include "Peds.h"
#include "Player.h"
#include "Monster.h"

namespace Scripting
{
	void Tick(GameData* _gameData);
	Players::Player* GetPlayer();
}

#endif // !SCRIPTING_H