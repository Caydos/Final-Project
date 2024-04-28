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
	Peds::Ped* GetPlayerPed();
	void SetCrosshairOpacity(float _opacity);
}

#endif // !SCRIPTING_H