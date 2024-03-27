#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "Maze.h"

static bool initialized = false;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lightning::Light* flashLight;

void Scritping::Tick(GameData* _gameData)
{
	if (!initialized)
	{

		Scene::Initialize(_gameData);
		Scene::World::SetSkyboxState(false);

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
		player->SetPed(playerPed);

		playerPed->SetPosition(glm::vec3(1.0f,1.8f,1.0f));


		Lightning::Light flashLight2;
		flashLight2.SetType(Lightning::LightType::SPOT);
		flashLight2.SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
		flashLight2.SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
		flashLight2.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
		flashLight2.SetConstant(1.0f);
		flashLight2.SetLinear(0.09f);
		flashLight2.SetQuadratic(0.0032f);

		flashLight2.SetCutOff(90.5f);
		flashLight2.SetOuterCutOff(90.5f);
		flashLight2.SetName("FlashLight");
		flashLight2.SetActive(true);
		Scene::Lights::InsertLight(_gameData, flashLight2);

		Scene::Lights::UpdateShader(_gameData);


		Maze::GenerateMaze(3,1);
		initialized = true;
	}
	std::vector<Lightning::Light>* lights = Scene::Lights::GetLights();
	for (size_t i = 0; i < lights->size(); i++)
	{
		if (lights->at(i).GetName() == "FlashLight")
		{
			Camera* cam = Scene::World::GetCamera();
			flashLight = &lights->at(i);
			flashLight->SetPosition(cam->Position);
			flashLight->SetDirection(cam->Front);
			Scene::Lights::UpdateShader(_gameData);
		}
	}
	Scene::Tick(_gameData);
	player->Control(_gameData);

}
