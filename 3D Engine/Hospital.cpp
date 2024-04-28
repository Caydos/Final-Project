#include "Hospital.h"
#include "Sprite.h"
#include "Interaction.h"
#include "GameObject.h"
#include "KeyPad.h"

static bool initialized = false;
static Map::Stage* stage = nullptr;
Sets::Set* vendingMachine = nullptr;
Sets::Set* entryRoom = nullptr;
Sets::Set* exitRoom = nullptr;
Sets::Set* waitingRoom = nullptr;
Sets::Set* playRoom = nullptr;
Sets::Set* giantBedRoom = nullptr;
Sets::Set* breakRoom = nullptr;

enum SocketNames
{
	BLUE,
	GREEN,
	RED,
	YELLOW
};
Sets::Set* sockets[4] = { nullptr };
Sets::Set* cubes[4] = { nullptr };

glm::vec3 socketInPosition(0.05, 0.05, 0.0);
std::vector<glm::vec3> socketsPositions = {
	glm::vec3(9.35,0.85,10.95),
	glm::vec3(9.35,0.85,11.1),
	glm::vec3(9.35,0.85,11.230),
	glm::vec3(9.35,0.85,11.360),
};


void Hospital::RegisterInteractions()
{
	//Interactions::Register("KeyPad", &KeyPad::Interaction);
}

void HoveredVendingMachine()
{
	Scripting::SetCrosshairOpacity(1.0f);
}
void InteractVendingMachine(Sets::Set* _set)
{
	KeyPad::Interaction();
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

	exitRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");
	entryRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
	if (entryRoom != nullptr)
	{
		playerPed->SetPosition(entryRoom->GetPosition() + glm::vec3(3, 2, 0.65), true);
		playerPed->SetHeading(0.0f); // Force to look front
	}
	else
	{
		Logger::Write("Unable to find spawn room in hospital stage\n");
	}


	playRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_PlayRoom.json");
	if (playRoom != nullptr)
	{
		std::vector<glm::vec3> playRoomPositions = {
			glm::vec3(0.750,0.45,2.85),
			glm::vec3(5.05,0.3,2.65),
			glm::vec3(7.45,0.4,5.7),
			glm::vec3(7.45,0.4,5.7),
			glm::vec3(10.05,0.3,6.45),
		};

		const char* paths[] = {
			"../Sets/HOSPITAL/Gameplay/HSP_CubeStockBlue.json",
			"../Sets/HOSPITAL/Gameplay/HSP_CubeStockGreen.json",
			"../Sets/HOSPITAL/Gameplay/HSP_CubeStockRed.json",
			"../Sets/HOSPITAL/Gameplay/HSP_CubeStockYellow.json",
		};
		for (size_t i = 0; i < 4; i++)
		{
			sockets[i] = Sets::Create();
			sockets[i]->SetParent(playRoom, true);
			sockets[i]->SetRenderingInstance(playRoom->GetRenderingInstance());
			sockets[i]->LoadFromJson(json::parse(Files::GetFileContent(paths[i])), false);
			sockets[i]->SetPosition(socketsPositions[i], true);
		}
		cubes[BLUE] = Sets::Create();
		cubes[BLUE]->SetParent(playRoom, true);
		cubes[BLUE]->SetRenderingInstance(playRoom->GetRenderingInstance());
		cubes[BLUE]->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Gameplay/HSP_N3BB.json")), false);
		cubes[BLUE]->SetPosition(playRoomPositions[rand() % playRoomPositions.size()], true);
	}
	else
	{
		Logger::Write("Unable to find play room in hospital stage\n");
	}
	giantBedRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_GiantBedRoom.json");
	if (giantBedRoom != nullptr)
	{
		std::vector<glm::vec3> giantBedroomPositions = {
			glm::vec3(2.6,0.55,3.85),
			glm::vec3(1.25, 0.55, 11.4),
			glm::vec3(11.5,0.5,11.9),
		};

		cubes[GREEN] = Sets::Create();
		cubes[GREEN]->SetParent(giantBedRoom, true);
		cubes[GREEN]->SetRenderingInstance(giantBedRoom->GetRenderingInstance());
		cubes[GREEN]->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Gameplay/HSP_N3BV.json")), false);
		cubes[GREEN]->SetPosition(giantBedroomPositions[rand() % giantBedroomPositions.size()], true);
	}
	else
	{
		Logger::Write("Unable to find giant bedroom in hospital stage\n");
	}

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

		std::vector<glm::vec3> waitingRoomPositions = {
			glm::vec3(8.850,1.2,0.15),
			glm::vec3(4.450,0.55,1.2),
			glm::vec3(9.4,0.28,3.55),
		};


		cubes[RED] = Sets::Create();
		cubes[RED]->SetParent(waitingRoom, true);
		cubes[RED]->SetRenderingInstance(waitingRoom->GetRenderingInstance());
		cubes[RED]->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Gameplay/HSP_N3BR.json")), false);
		cubes[RED]->SetPosition(waitingRoomPositions[rand() % waitingRoomPositions.size()], true);
	}
	else
	{
		Logger::Write("Unable to find waiting room in hospital stage\n");
	}


	breakRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_BreakRoom.json");
	if (breakRoom != nullptr)
	{
		std::vector<glm::vec3> breakRoomPositions = {
			glm::vec3(1.5,0.45,0.15),
			glm::vec3(2.25,0.8,3.85),
			glm::vec3(3.65,0.7,3.25),
		};


		cubes[YELLOW] = Sets::Create();
		cubes[YELLOW]->SetParent(breakRoom, true);
		cubes[YELLOW]->SetRenderingInstance(breakRoom->GetRenderingInstance());
		cubes[YELLOW]->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Gameplay/HSP_N3BJ.json")), false);
		cubes[YELLOW]->SetPosition(breakRoomPositions[rand() % breakRoomPositions.size()], true);
	}
	else
	{
		Logger::Write("Unable to find break room in hospital stage\n");
	}


	if (vendingMachine != nullptr)
	{
		GameObjects::Register(vendingMachine, 2.0f, 1000.0, &HoveredVendingMachine, &InteractVendingMachine);
	}

	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	KeyPad::Tick(_gameData);
	if (_gameData->window.IsFocused())
	{
		if (vendingMachine != nullptr)
		{
			if (_gameData->window.IsKeyPressed(Keys::KP_0))
			{
				Peds::Ped* playerPed = Scripting::GetPlayerPed();
				playerPed->SetPosition(vendingMachine->GetWorldPosition(), true);
			}
		}
	}
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}
