#include "Garden.h"
static bool initialized = false;

void Garden::RegisterInteractions()
{
}

void Garden::Initialize(GameData* _gameData)
{

	initialized = true;
}

void Garden::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

}

void Garden::CleanUp(GameData* _gameData)
{
}
