#include "Playtest.h"
#include "MapManager.h"
#include "Tools.h"
#include "LoadingScreen.h"

static bool initialized = false;
static unsigned int FPVCam;
static bool generated = false;
static glm::vec3 spawnPoint(11.05, 1.850, 21.250);
static float spawnYaw = 0.0f;
static Lighting::Spot* flashLight;

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
void Playtest::Initialize(GameData* _gameData)
{
	Scene::Initialize(_gameData);
	Scene::World::SetSkyboxState(false);

	FPVCam = Scene::World::NewCamera(glm::vec3(spawnPoint), glm::vec3(0.0f, 1.0f, 0.0f), spawnYaw/*Cuz the player ped has been mapped weirdly*/);

	Scene::World::FocusCamera(_gameData, FPVCam);
	Generation();
	//flashLight = Scene::Lights::CreateSpot();
	//flashLight->activation = 1.0f;
	//flashLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	//flashLight->diffuse = glm::vec3(0.5f);
	//flashLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);
	//flashLight->constant = 0.00001f;
	//flashLight->linear = 0.0f;
	//flashLight->quadratic = 0.0f;
	//flashLight->cutOff = glm::cos(glm::radians(20.0f));
	//flashLight->outerCutOff = glm::cos(glm::radians(40.0f));

	initialized = true;
}

void Playtest::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	Scene::Tick(_gameData);
	if (generated)
	{
		//flashLight->position = _gameData->camera->Position;
		//flashLight->direction = _gameData->camera->Front;
		//Lighting::UpdateSpot(flashLight);
		//Scene::Lights::UpdateSpot(flashLight);
		Tools::Inputs(_gameData);
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}
}
