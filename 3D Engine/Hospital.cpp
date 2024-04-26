#include "Hospital.h"
#include "Sprite.h"
#include "Interaction.h"
#include "KeyPad.h"


static bool initialized = false;
static Map::Stage* stage = nullptr;
Sets::Set* vendingMachine = nullptr;
void Hospital::RegisterInteractions()
{
	Interactions::Register("KeyPad", &KeyPad::Interaction);
}

void Hospital::Initialize(GameData* _gameData)
{
	if (initialized) { return; }
	Players::Player* player = Scripting::GetPlayer();
	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	stage = Map::GetStage(Map::StageEnum::HOSPITALS);
	if (stage == nullptr)
	{
		Logger::Write("Unable to find hospital level stage\n");
		return;
	}
	Sets::Set* entryRoom = nullptr;
	Sets::Set* exitRoom = nullptr;
	Sets::Set* waitingRoom = nullptr;

	exitRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");
	entryRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
	waitingRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_WaitingRoom.json");
	if (waitingRoom != nullptr)
	{
		std::vector<Sets::Set*> children = waitingRoom->GetChildArray();
		for (size_t childId = 0; childId < children.size(); childId++)
		{
			if (children[childId]->GetName() == "HSP_VendinMachinEmpty")
			{
				vendingMachine = children[childId];
				break;
			}
		}
		if (vendingMachine == nullptr)
		{
			Logger::Write("Unable to find vending machine in hospital stage\n");
		}
	}
	else
	{
		Logger::Write("Unable to find waiting room in hospital stage\n");
	}
	if (entryRoom != nullptr)
	{
		playerPed->SetPosition(entryRoom->GetPosition() + glm::vec3(3, 2, 0.65), true);
		playerPed->SetHeading(0.0f); // Force to look front
	}
	else
	{
		Logger::Write("Unable to find spawn room in hospital stage\n");
	}

	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	KeyPad::Tick(_gameData);
	if (_gameData->window.IsKeyPressed(Keys::F4))
	{
		Interactions::Trigger("KeyPad", "");
	}
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}
