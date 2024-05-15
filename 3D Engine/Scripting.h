#ifndef SCRIPTING_H
#define SCRIPTING_H
#include "Common.h"
#include "Audio.h"
#include "GameObject.h"
#include "Menu.h"
#include "VictoryDefeat.h"

#include "Peds.h"
#include "Player.h"
#include "Monster.h"


namespace Scripting
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void HoveredCrosshair();
	void SetCrosshairOpacity(float _opacity);

	void PlayerUpdate(char** _args);
	void SetConnectionState(bool _state);
	void SetgameState(int _state);

	Players::Player* GetPlayer();
	Peds::Ped* GetPlayerPed();

}

#endif // !SCRIPTING_H