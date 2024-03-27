#include "Scene.h"
#include "Clock.h"
#include "Editor.h"
#include "Crosshair.h"
#include "Maze.h"
#include "Blocks.h"
#include "Inventory.h"
#include "Set.h"

static bool initialized = false;
static Colors::Color clearColor = Colors::LimedSpruce;

Colors::Color Scene::GetClearColor()
{
	return clearColor;
}

void Scene::SetClearColor(Colors::Color _color)
{
	clearColor = _color;
}

static Sets::Set* set = nullptr;
void Scene::Initialize(GameData* _gameData)
{
	Blocks::Initialize();

	initialized = true;
}

void Scene::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	

	_gameData->window.Clear(clearColor);

	Scene::World::Render(_gameData);
}

void Scene::CleanUp()
{
}

