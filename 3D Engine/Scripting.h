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
	void HoveredCrosshair();
	void SetCrosshairOpacity(float _opacity);

	void PlayerUpdate(char** _args);
	void SetConnectionState(bool _state);
}

#endif // !SCRIPTING_H