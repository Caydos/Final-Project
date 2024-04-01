#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "Maze.h"
#include "Collisions.h"
#include "Monster.h"
#include "LoadingScreen.h"
#include "FrustumCulling.h"

static bool initialized = false;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lighting::Light* flashLight;
static Lighting::Light* directionalLight;
static Sets::Set* set;
static std::thread mazeThread;
static bool generated = false;

void Generation()
{
	Clock loadingClock;
	loadingClock.Restart();
	Maze::GenerateMaze(3, 1);
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	Monster::GenerateMonster(); //Pop les mob
	generated = true;
}

void Scritping::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		Scene::Initialize(_gameData);
		Scene::World::SetSkyboxState(true);

		FPVCam = Scene::World::NewCamera(glm::vec3(3.0f, 1.2f, 3.0f));
		Scene::World::FocusCamera(_gameData, FPVCam);


		player = Players::Create();
		Peds::Ped* playerPed = Peds::Create();
		playerPed->Initialize();
		playerPed->GenerateRenderingInstance();
		playerPed->LoadFromJson(json::parse(Files::GetFileContent("../Sets/MC/Character.json")));
		playerPed->SetName("Character");
		playerPed->SetPath("../Sets/MC/");
		playerPed->SetCamera(_gameData->camera);
		playerPed->SetBodyType(Physics::Type::RIGID);
		player->SetPed(playerPed);

		playerPed->SetPosition(glm::vec3(3.650f, 1.850f, 1.1f), true);

		directionalLight = Scene::Lights::Create();
		directionalLight->SetType(Lighting::LightType::DIRECTIONAL);
		directionalLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		directionalLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		directionalLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		directionalLight->SetDirection(glm::vec3(0.0f, 1.0f, 0.0f));
		directionalLight->SetName("Directional");
		directionalLight->SetActive(true);

		flashLight = Scene::Lights::Create();
		flashLight->SetType(Lighting::LightType::SPOT);
		flashLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		flashLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		flashLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		flashLight->SetConstant(1.0f);
		flashLight->SetLinear(0.09f);
		flashLight->SetQuadratic(0.0032f);

		flashLight->SetCutOff(90.339f);
		flashLight->SetOuterCutOff(90.764f);
		flashLight->SetName("FlashLight");
		flashLight->SetActive(false);

		//Scene::Lights::InsertLight(_gameData, flashLight2);
		//Scene::Lights::UpdateShader(_gameData);


		mazeThread = std::thread(Generation);
		mazeThread.detach();

		initialized = true;
	}
	if (generated)
	{
		//std::vector<Lighting::Light>* lights = Scene::Lights::GetLights();
		//for (size_t i = 0; i < lights->size(); i++)
		//{
		//	if (lights->at(i).GetName() == "FlashLight")
		//	{
				Camera* cam = Scene::World::GetCamera();
				//flashLight = &lights->at(i);
				flashLight->SetPosition(cam->Position);
				flashLight->SetDirection(cam->Front);
				Scene::Lights::RefreshLight(flashLight);
				//Scene::Lights::UpdateShader(_gameData);
		//	}
		//}

		//if (IsSphereVisible(*_gameData->camera, flashLight->GetPosition(), calculateLightRange(flashLight->GetConstant(), flashLight->GetLinear(), flashLight->GetQuadratic())))
		//float range = calculateLightRange(flashLight->GetConstant(), flashLight->GetLinear(), flashLight->GetQuadratic());
		//if (!FrustumCulling::IsBoxInFrustum(Scene::World::GetProjection(), Scene::World::GetView(), flashLight->GetPosition() - range, flashLight->GetPosition() + range))
		//{
		//	std::cout << "Light is out of range" << std::endl;
		//}

		player->Control(_gameData);
		Peds::Simulate(_gameData);
		player->GetPed()->DrawBoundingBox();
		Monster::MovementMob(_gameData);


		Scene::Tick(_gameData);


		// Cheat code
		if (_gameData->window.IsKeyPressed(Keys::F1))
		{
			player->GetPed()->SetBodyType(Physics::Type::GHOST);
		}
		if (_gameData->window.IsKeyPressed(Keys::F2))
		{
			player->GetPed()->SetBodyType(Physics::Type::RIGID);
		}
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}
}
