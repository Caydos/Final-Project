#include "Playtest.h"
#include "MapManager.h"
#include "Tools.h"
#include "LoadingScreen.h"
#include "Audio.h"
#include "Pathfinding.h"



static bool initialized = false;
static unsigned int FPVCam;
static bool generated = false;
//static glm::vec3 spawnPoint(11.05, 1.850, 21.250);
static glm::vec3 spawnPoint(0.0);
static float spawnYaw = 0.0f;
static Lighting::Spot* flashLight;
static Audio::Sound sound;

std::vector<Sets::Set*> parts;

static void Generation()
{

	Clock loadingClock;
	loadingClock.Restart();
	Map::GenerateMaze(8, 1);

	std::vector<Lighting::Spot*>* spots = Scene::Lights::GetSpots();

	std::cout << spots->size() << " Lights in the scene" << std::endl;
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	generated = true;
}

Sets::Set* startCube;
Sets::Set* endCube;
std::vector<Pathfinding::Cube> path;


void Playtest::Initialize(GameData* _gameData)
{
	Scene::Initialize(_gameData);
	Scene::World::SetSkyboxState(false);

	FPVCam = Scene::World::NewCamera(glm::vec3(spawnPoint), glm::vec3(0.0f, 1.0f, 0.0f), spawnYaw/*Cuz the player ped has been mapped weirdly*/);

	Scene::World::FocusCamera(_gameData, FPVCam);
	//Generation();
	flashLight = Scene::Lights::CreateSpot();
	flashLight->activation = 1.0f;
	flashLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	flashLight->diffuse = glm::vec3(0.5f);
	flashLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);
	flashLight->constant = 0.00001f;
	flashLight->linear = 0.0f;
	flashLight->quadratic = 0.0f;
	flashLight->cutOff = glm::cos(glm::radians(20.0f));
	flashLight->outerCutOff = glm::cos(glm::radians(40.0f));

	sound.LoadFromFile("../Sounds/Footsteps3.wav");
	sound.SetPosition(glm::vec3(spawnPoint.x, spawnPoint.y, spawnPoint.z));
	sound.Loop(true);
	sound.Play();


	{
		startCube = Sets::Create();
		startCube->SetName("startCube");
		startCube->GenerateRenderingInstance();
		startCube->AppplyVisibility();

		Blocks::Block block;
		block.GenerateModel();
		Blocks::MaterialCheck(&block, "BCR_Blue");
		Blocks::BlockType* type = block.GetType();

		block.SetScale(1.0f);
		startCube->InsertBlock(block);
	}
	{
		endCube = Sets::Create();
		endCube->SetName("endCube");
		endCube->GenerateRenderingInstance();
		endCube->AppplyVisibility();

		Blocks::Block block;
		block.GenerateModel();
		Blocks::MaterialCheck(&block, "BCR_Brown");
		Blocks::BlockType* type = block.GetType();

		block.SetScale(1.0f);
		endCube->InsertBlock(block);
	}
	std::vector<Pathfinding::Cube> cubes;
	std::vector<Pathfinding::Cube> obstacles;


	const int gridSize = 10;
	for (size_t i = 0; i < gridSize; i++)
	{
		for (size_t j = 0; j < gridSize; j++)
		{
			Sets::Set* ctPart = Sets::Create();
			ctPart->SetName("ctPart");
			ctPart->GenerateRenderingInstance();
			ctPart->AppplyVisibility();

			Blocks::Block block;
			block.GenerateModel();
			Blocks::MaterialCheck(&block, "HSP_S1");
			Blocks::BlockType* type = block.GetType();

			block.SetScale(1.0f);
			ctPart->InsertBlock(block);

			glm::vec3 position(2.0 * i, 0.0, 2.0 * j);

			ctPart->SetPosition(position);
			cubes.push_back(Pathfinding::Cube(gridSize * i + j, position.x, position.z));
			parts.push_back(ctPart);
		}
	}
	{
		Sets::Set* ctPart = Sets::Create();
		ctPart->SetName("obstacle");
		ctPart->GenerateRenderingInstance();
		ctPart->AppplyVisibility();

		Blocks::Block block;
		block.GenerateModel();
		Blocks::MaterialCheck(&block, "BCR_Red");
		Blocks::BlockType* type = block.GetType();

		block.SetScale(0.4f);
		ctPart->InsertBlock(block);

		glm::vec3 position(2.0 * 1, 1.0, 2.0 * 1);

		ctPart->SetPosition(position);
		obstacles.push_back(Pathfinding::Cube(44, position.x, position.y));
		obstacles.push_back(Pathfinding::Cube(67, position.x, position.y));
	}

	path = Pathfinding::a_star(&cubes[0], &cubes[cubes.size() - 1], cubes, obstacles, 3.5f);


	startCube->SetPosition(glm::vec3(cubes[0].x, 0.0, cubes[0].y));

	for (size_t cubeId = 0; cubeId < path.size(); cubeId++)
	{
		Sets::Set* ctPart = Sets::Create();
		ctPart->SetName("ctPart");
		ctPart->GenerateRenderingInstance();
		ctPart->AppplyVisibility();

		Blocks::Block block;
		block.GenerateModel();
		Blocks::MaterialCheck(&block, "HSP_Green");
		Blocks::BlockType* type = block.GetType();

		block.SetScale(0.4f);
		ctPart->InsertBlock(block);

		ctPart->SetPosition(glm::vec3(path[cubeId].x, 1.2, path[cubeId].y));
		std::cout << path[cubeId].x << " " << path[cubeId].y << std::endl;
	}

	generated = true;

	initialized = true;
}

void Playtest::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	Scene::Tick(_gameData);
	if (generated)
	{
		flashLight->position = _gameData->camera->Position;
		flashLight->direction = _gameData->camera->Front;
		Lighting::UpdateSpot(flashLight);
		Scene::Lights::UpdateSpot(flashLight);
		Tools::Inputs(_gameData);
		follow_path(startCube, path, _gameData->dt);

		//std::cout << entity.position.x << " " << entity.position.y + 1.0 << " " << entity.position.z << std::endl;
		//startCube->SetPosition(glm::vec3(entity.position.x, entity.position.y + 1.0, entity.position.z));
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}
}
