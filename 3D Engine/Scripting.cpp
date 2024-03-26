#include "Scripting.h"
#include "Scene.h"
#include "Player.h"

static bool initialized = false;
static Players::Player* player = nullptr;

void Scritping::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		Scene::Initialize(_gameData);
		Scene::World::SetSkyboxState(true);

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
		initialized = true;
	}
	Scene::Tick(_gameData);
	player->Control(_gameData);

}
