#include "Exploration.h"
#include "Map.h"
static bool intialized = false;


void Exploration::Initialize(GameData* _gameData)
{
	Map::GenerateRandomly(_gameData->shaders[3]);
	intialized = true;
}

void Exploration::Inputs(GameData* _gameData)
{
	if (_gameData->window.IsKeyPressed(Keys::ESCAPE))
	{
		_gameData->window.Close(true);
	}
	if (_gameData->window.IsKeyPressed(Keys::W))
	{
		_gameData->camera->ProcessKeyboard(FORWARD, _gameData->dt);
	}
	if (_gameData->window.IsKeyPressed(Keys::S))
	{
		_gameData->camera->ProcessKeyboard(BACKWARD, _gameData->dt);
	}
	if (_gameData->window.IsKeyPressed(Keys::A))
	{
		_gameData->camera->ProcessKeyboard(LEFT, _gameData->dt);
	}
	if (_gameData->window.IsKeyPressed(Keys::D))
	{
		_gameData->camera->ProcessKeyboard(RIGHT, _gameData->dt);
	}
	if (_gameData->window.IsKeyPressed(Keys::SPACE))
	{
		_gameData->camera->ProcessKeyboard(UP, _gameData->dt);
	}
	if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
	{
		_gameData->camera->ProcessKeyboard(DOWN, _gameData->dt);
	}
}

void Exploration::Tick(GameData* _gameData)
{
	if (!intialized) { Initialize(_gameData); }
	Inputs(_gameData);

	_gameData->window.Clear();
	_gameData->camera->ProcessMouseMovement(_gameData->window.xoffset, _gameData->window.yoffset);

	// pass projection matrix to shader (note that in this case it could change every frame)
	glm::mat4 projection = glm::perspective(glm::radians(_gameData->camera->Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	_gameData->shaders[3]->setMat4("projection", projection);
	// camera/view transformation
	glm::mat4 view = _gameData->camera->GetViewMatrix();
	_gameData->shaders[3]->setMat4("view", view);

	Map::Render();
	_gameData->window.Events();
}

void Exploration::CleanUp()
{
}
