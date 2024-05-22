#include "VictoryDefeat.h"
#include "Sprite.h"

static Sprite winSprite;
static Sprite looseSprite;
static bool initialized = false;


void VictoryDefeat::Initialize(GameData* _gameData)
{
	winSprite.Load("../Textures/ScreenWin.png", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0), 1);
	looseSprite.Load("../Textures/ScreenLoose.png", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0), 1);
	initialized = true;
}

void VictoryDefeat::Tick(GameData* _gameData, int _gameState)
{
	if (!initialized)
	{
		Initialize(_gameData);
	}
	if (_gameState == 1)
	{
		winSprite.Draw();
	}
	else if (_gameState == -1)
	{
		looseSprite.Draw();
	}
}
