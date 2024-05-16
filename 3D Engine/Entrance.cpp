#include "Entrance.h"

static bool initialized = false;

void Entrance::RegisterInteractions()
{
}

void Entrance::Initialize(GameData* _gameData)
{

	initialized = true;
}

void Entrance::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

}

void Entrance::CleanUp(GameData* _gameData)
{
}
