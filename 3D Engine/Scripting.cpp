#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "MapManager.h"
#include "Collisions.h"
#include "Monster.h"
#include "LoadingScreen.h"
#include "FrustumCulling.h"
#include "Sprite.h"
#include "Audio.h"
#include "Crosshair.h"
#include "Hospital.h"


static bool initialized = false;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lighting::Spot* flashLight;

static Sets::Set* set;
static std::thread mazeThread;
static bool generated = false;

static Clock inputClock;

static glm::vec3 spawnPoint(11.05, 1.850, 21.250);
static float spawnYaw = 0.0f;


void Generation()
{
	Clock loadingClock;
	loadingClock.Restart();
	Map::GenerateMaze(7, 1);
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	


	generated = true;
}

void Scripting::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		Scene::Initialize(_gameData);
		Scene::World::SetSkyboxState(false);

		FPVCam = Scene::World::NewCamera(glm::vec3(spawnPoint), glm::vec3(0.0f, 1.0f, 0.0f), spawnYaw/*Cuz the player ped has been mapped weirdly*/);

		Scene::World::FocusCamera(_gameData, FPVCam);

		player = Players::Create();

		Peds::Ped* playerPed = Peds::Create();
		playerPed->Initialize();
		playerPed->GenerateRenderingInstance();
		playerPed->LoadFromJson(json::parse(Files::GetFileContent("../Sets/MC/MC_CharacterV3.json")));
		playerPed->SetName("Character");
		playerPed->SetPath("../Sets/MC/");

		std::vector<Sets::Set*> children = playerPed->GetChildArray();
		for (size_t i = 0; i < children.size(); i++)
		{
			if (children[i]->GetName() == "MC_HeadV3")
			{
				Sets::Erase(children[i]);
				break;
			}
		}
	
		playerPed->SetCamera(_gameData->camera);
		playerPed->SetBodyType(Physics::Type::RIGID);
		player->SetPed(playerPed);
		playerPed->SetAdditionalRotation(glm::vec3(0.0,-90.0,0.0));

		playerPed->SetPosition(spawnPoint, true);
		playerPed->SetRotation(glm::vec3(0.0, spawnYaw, 0.0), true);
		//playerPed->SetScale(1.6f, true);

		//directionalLight = Scene::Lights::Create();
		//directionalLight->SetType(Lighting::LightType::DIRECTIONAL);
		//directionalLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		//directionalLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		//directionalLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		//directionalLight->SetDirection(glm::vec3(0.0f, 1.0f, 0.0f));
		//directionalLight->SetName("Directional");
		//directionalLight->SetActive(true);

		//flashLight->SetCutOff(90.339f);
		//flashLight->SetOuterCutOff(90.764f);
		//flashLight->SetName("FlashLight");
		//flashLight->SetActive(false);


		flashLight = Scene::Lights::CreateSpot();
		flashLight->activation = 1.0f;
		flashLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		flashLight->diffuse = glm::vec3(0.5f);
		flashLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);
		flashLight->constant = 0.5f;
		flashLight->linear = 0.0f;
		flashLight->quadratic = 0.0f;
		flashLight->cutOff = glm::cos(glm::radians(20.0f));
		flashLight->outerCutOff = glm::cos(glm::radians(40.0f));

		mazeThread = std::thread(Generation);
		mazeThread.detach();

		initialized = true;
	}
	Scene::Tick(_gameData);

	if (generated)
	{
		if (_gameData->window.IsFocused())
		{
			Crosshairs::Draw();
			Crosshairs::Get()->SetColor(Colors::White);
			player->Control(_gameData);
			Peds::Simulate(_gameData);

			flashLight->position = _gameData->camera->Position;
			flashLight->direction = _gameData->camera->Front;
			Lighting::UpdateSpot(flashLight);
			Scene::Lights::UpdateSpot(flashLight);

			// Cheat code
			if (_gameData->window.IsKeyPressed(Keys::F1))
			{
				player->GetPed()->SetBodyType(Physics::Type::GHOST);
			}
			if (_gameData->window.IsKeyPressed(Keys::F2))
			{
				player->GetPed()->SetBodyType(Physics::Type::RIGID);
			}
			if (_gameData->window.IsKeyPressed(Keys::F3))
			{
				player->GetPed()->SetPosition(spawnPoint);
			}
			Hospital::Tick(_gameData);
		}
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}
}

Players::Player* Scripting::GetPlayer()
{
	return player;
}
