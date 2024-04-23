#include "Hospital.h"
#include "Sprite.h"

static bool initialized = false;
static bool displayKeyPad = true;
static Sprite keyPadSprites[13];

void Hospital::Initialize(GameData* _gameData)
{

	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }


}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}

void Hospital::KeyPad(GameData* _gameData)
{

}
