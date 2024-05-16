#include "Library.h"
static bool initialized = false;

void Library::RegisterInteractions()
{
}

void Library::Initialize(GameData* _gameData)
{

	initialized = true;
}

void Library::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

}

void Library::CleanUp(GameData* _gameData)
{
}
