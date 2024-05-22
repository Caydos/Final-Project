#include "Entrance.h"

static bool initialized = false;
static Audio::Sound* tutorial;
static Audio::Sound* circuitBreaker;
static Audio::Sound* door;
static Sets::Set* map;
int interactions = 0;

void Entrance::RegisterInteractions()
{
}



static void ElevatorInteraction(Sets::Set* _set)
{
	Levels::Set(Levels::HOSPITAL);
	GameObjects::UnRegister(_set);
}

static void BreakerInteraction(Sets::Set* _set)
{
	circuitBreaker->SetPosition(_set->GetWorldPosition());
	circuitBreaker->Play();
	std::vector<Sets::Set*> childArray = _set->GetChildArray();
	for (size_t i = 0; i < childArray.size(); i++)
	{
		if (childArray[i]->GetName() == "HSP_Handle")
		{
			childArray[i]->Rotate(glm::vec3(75.0, 0.0, 0.0));
		}
	}
	GameObjects::UnRegister(_set);
	interactions++;
}

static void DoorInteraction(Sets::Set* _set)
{
	if (interactions == 3)
	{
		door->SetPosition(_set->GetWorldPosition());
		door->Play();
		_set->Rotate(glm::vec3(0.0, 90.0, 0.0));
	}
}

void Entrance::Initialize(GameData* _gameData)
{
	tutorial = Audio::CreateSound();
	tutorial->LoadFromFile("../Sounds/TUTORIAL/VOICES/Male Voice/MLong.wav");

	circuitBreaker = Audio::CreateSound();
	circuitBreaker->LoadFromFile("../Sounds/GAMEPLAY/Circuit Breaker/CB.wav");
	
	
	door = Audio::CreateSound();
	door->LoadFromFile("../Sounds/DoorOpen.wav");

	map = Sets::Create();
	map->GenerateRenderingInstance();
	map->LoadFromJson(json::parse(Files::GetFileContent("../Sets/TUTO/Map/TUTO_Map.json")), false);
	map->SetPosition(glm::vec3(0.0, -100.0, 0.0));

	std::vector<Sets::Set*> childArray = map->GetChildArray();
	for (size_t i = 0; i < childArray.size(); i++)
	{
		if (childArray[i]->GetName() == "HSP_CircuitBreaker")
		{
			GameObjects::Register(childArray[i], 2.0, 2000, &Interactions::Overlay::HoveredCrosshair, &BreakerInteraction);
		}
		if (childArray[i]->GetName() == "HSP_DoorExit")
		{
			GameObjects::Register(childArray[i], 2.0, 2000, &Interactions::Overlay::HoveredCrosshair, &DoorInteraction);
		}
		if (childArray[i]->GetName() == "HSP_Radio")
		{
			tutorial->SetPosition(childArray[i]->GetWorldPosition());
			tutorial->SetVolume(0.3);
			tutorial->Play();
		}
		if (childArray[i]->GetName() == "HSP_Elevator")
		{
			GameObjects::Register(childArray[i], 2.0, 2000, &Interactions::Overlay::HoveredCrosshair, &ElevatorInteraction);
		}
	}

	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	if (playerPed != nullptr)
	{
		playerPed->SetPosition(map->GetPosition() + glm::vec3(12.0f,2.0, 1.5f));
		_gameData->camera->Yaw = 90.0f;
	}

	initialized = true;
}

void Entrance::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
}

void Entrance::CleanUp(GameData* _gameData)
{
}
