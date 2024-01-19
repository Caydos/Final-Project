#include "Gameplay.h"
#include "Map.h"
#include <GL/glew.h>
#include "Camera.h"

static bool intialized = false;

void Gameplay::Initialize(GameData* _gameData)
{
	_gameData->window.setMouseCursorVisible(false);
	Map::GenerateRandomly();
	intialized = true;
}

void Gameplay::Tick(GameData* _gameData)
{
	if (!intialized)
	{
		Initialize(_gameData);
	}

	Camera::MouseMovement(sf::Mouse::getPosition(_gameData->window), _gameData->settings.sentivity, &_gameData->window);
	Map::Render(&_gameData->window);
}

void Gameplay::CleanUp(void)
{

	intialized = false;
}
