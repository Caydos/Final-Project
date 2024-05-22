#include "Entrance.h"

static bool initialized = false;
static Audio::Sound* tutorial;
static Sets::Set* map;

void Entrance::RegisterInteractions()
{
}

void Entrance::Initialize(GameData* _gameData)
{
	tutorial = Audio::CreateSound();
	tutorial->LoadFromFile("../Sounds/TUTORIAL/VOICES/Male Voice/MLong.wav");

	map = Sets::Create();
	map->GenerateRenderingInstance();
	map->LoadFromJson(json::parse(Files::GetFileContent("../Sets/TUTO/Map/TUTO_Map.json")), false);
	map->SetPosition(glm::vec3(0.0, -100.0, 0.0));

	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	if (playerPed != nullptr)
	{
		playerPed->SetPosition(map->GetPosition() + glm::vec3(12.0f,2.0, 1.5f));
		//playerPed->SetRotation(glm::vec3(0.0, 180.0, 0.0));
		_gameData->camera->Yaw = 90.0f;
	}

	initialized = true;
}

void Entrance::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	if (playerPed != nullptr)
	{
		tutorial->SetPosition(playerPed->GetPosition());
	}
}

void Entrance::CleanUp(GameData* _gameData)
{
}
