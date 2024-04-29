#include "Hospital.h"
#include "Sprite.h"
#include "Interaction.h"
#include "GameObject.h"
#include "KeyPad.h"
#include "Reading.h"

static bool initialized = false;
static Map::Stage* stage = nullptr;
Sets::Set* vendingMachine = nullptr;
Sets::Set* elevator = nullptr;
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
const char* cubesPaths[] = {
	"../Sets/HOSPITAL/Gameplay/HSP_N7BB.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N5BV.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N9BR.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N1BJ.json",
};
const char* socketPaths[] = {
	"../Sets/HOSPITAL/Gameplay/HSP_CubeStockBlue.json",
	"../Sets/HOSPITAL/Gameplay/HSP_CubeStockGreen.json",
	"../Sets/HOSPITAL/Gameplay/HSP_CubeStockRed.json",
	"../Sets/HOSPITAL/Gameplay/HSP_CubeStockYellow.json",
};

Sets::Set* clown = nullptr;
Sets::Set* sockets[4] = { nullptr };
Sets::Set* cubes[4] = { nullptr };
bool validCubes[4] = { false };

glm::vec3 socketInPosition(0.05, 0.05, 0.0);
std::vector<glm::vec3> socketsPositions = {
	glm::vec3(9.35,0.85,10.95),
	glm::vec3(9.35,0.85,11.1),
	glm::vec3(9.35,0.85,11.230),
	glm::vec3(9.35,0.85,11.360),
};
glm::vec3 clownSpawnPoint(1.150, 1.9, 1.0);

void Hospital::RegisterInteractions()
{
	//Interactions::Register("KeyPad", &KeyPad::Interaction);
}

void InteractVendingMachine(Sets::Set* _set)
{
	KeyPad::Interaction();
}
void SocketInteraction(Sets::Set* _set)
{
	std::string socketName = _set->GetName();
	std::string path;
	int index = 0;
	for (size_t i = 0; i < 4; i++)
	{
		if (socketName == socketPaths[i])
		{
			path = cubesPaths[i];
			validCubes[i] = true;
			//cubes[i] = Sets::Create();
			////cubes[i]->SetParent(_set, true);
			////cubes[i]->SetRenderingInstance(_set->GetRenderingInstance());
			//cubes[i]->GenerateRenderingInstance();
			//cubes[i]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[i])), false);
			cubes[i]->SetPosition(sockets[i]->GetWorldPosition() + socketInPosition, true);
			//cubes[i]->SetName(cubesPaths[i]);
			//cubes[i]->CheckVisibility();
			std::cout << cubesPaths[i] << std::endl;
			break;
		}
	}
	GameObjects::UnRegister(_set);
}
void CubePickup(Sets::Set* _set)
{
	std::string cubeName = _set->GetName();
	if (cubeName == cubesPaths[BLUE])
	{
		std::cout << "Picked up : Blue" << std::endl;
		GameObjects::Register(sockets[BLUE], 2.0f, 1000.0, &Scripting::HoveredCrosshair, &SocketInteraction);
	}
	else if (cubeName == cubesPaths[GREEN])
	{
		std::cout << "Picked up : Green" << std::endl;
		GameObjects::Register(sockets[GREEN], 2.0f, 1000.0, &Scripting::HoveredCrosshair, &SocketInteraction);
	}
	else if (cubeName == cubesPaths[RED])
	{
		std::cout << "Picked up : Red" << std::endl;
		GameObjects::Register(sockets[RED], 2.0f, 1000.0, &Scripting::HoveredCrosshair, &SocketInteraction);
	}
	else if (cubeName == cubesPaths[YELLOW])
	{
		std::cout << "Picked up : Yellow" << std::endl;
		GameObjects::Register(sockets[YELLOW], 2.0f, 1000.0, &Scripting::HoveredCrosshair, &SocketInteraction);
	}
	GameObjects::UnRegister(_set);
	//Sets::Erase(_set);
	_set->Move(glm::vec3(0.0, -2.0f, 0.0f), true);
	//_set->Scale(glm::vec3(0.0), true);
}

void InteractExit(Sets::Set* _set)
{
	std::cout << "WINNNN" << std::endl;
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
	if (exitRoom != nullptr)
	{
		std::vector<Sets::Set*> children = exitRoom->GetChildArray();
		for (size_t childId = 0; childId < children.size(); childId++)
		{
			if (children[childId]->GetName() == "HSP_Elevator")
			{
				elevator = children[childId];
				break;
			}
		}
		if (elevator == nullptr)
		{
			Logger::Write("Unable to find elevator in hospital stage\n");
		}
	}
	else
	{
		Logger::Write("Unable to find exit room in hospital stage\n");
	}


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

		{// Fucked up because not following the enum
			for (size_t i = 0; i < 4; i++)
			{
				sockets[i] = Sets::Create();
				sockets[i]->SetParent(playRoom, true);
				sockets[i]->SetRenderingInstance(playRoom->GetRenderingInstance());
				sockets[i]->LoadFromJson(json::parse(Files::GetFileContent(socketPaths[i])), false);
				sockets[i]->SetPosition(socketsPositions[i], true);
				sockets[i]->SetName(socketPaths[i]);
			}
		}
		cubes[BLUE] = Sets::Create();
		//cubes[BLUE]->SetParent(playRoom, true);
		//cubes[BLUE]->SetRenderingInstance(playRoom->GetRenderingInstance());
		cubes[BLUE]->GenerateRenderingInstance();
		cubes[BLUE]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[BLUE])), false);
		cubes[BLUE]->SetPosition(playRoom->GetWorldPosition() + (playRoomPositions[rand() % playRoomPositions.size()]), true);
		cubes[BLUE]->SetName(cubesPaths[BLUE]);
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
		//cubes[GREEN]->SetParent(giantBedRoom, true);
		//cubes[GREEN]->SetRenderingInstance(giantBedRoom->GetRenderingInstance());
		cubes[GREEN]->GenerateRenderingInstance();
		cubes[GREEN]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[GREEN])), false);
		cubes[GREEN]->SetPosition(giantBedRoom->GetWorldPosition() + giantBedroomPositions[rand() % giantBedroomPositions.size()], true);
		cubes[GREEN]->SetName(cubesPaths[GREEN]);
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
		//cubes[RED]->SetParent(waitingRoom, true);
		//cubes[RED]->SetRenderingInstance(waitingRoom->GetRenderingInstance());
		cubes[RED]->GenerateRenderingInstance();
		cubes[RED]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[RED])), false);
		cubes[RED]->SetPosition(waitingRoom->GetWorldPosition() + waitingRoomPositions[rand() % waitingRoomPositions.size()], true);
		cubes[RED]->SetName(cubesPaths[RED]);
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
		//cubes[YELLOW]->SetParent(breakRoom, true);
		//cubes[YELLOW]->SetRenderingInstance(breakRoom->GetRenderingInstance());
		cubes[YELLOW]->GenerateRenderingInstance();
		cubes[YELLOW]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[YELLOW])), false);
		cubes[YELLOW]->SetPosition(breakRoom->GetWorldPosition() + breakRoomPositions[rand() % breakRoomPositions.size()], true);
		cubes[YELLOW]->SetName(cubesPaths[YELLOW]);
	}
	else
	{
		Logger::Write("Unable to find break room in hospital stage\n");
	}


	if (vendingMachine != nullptr)
	{
		GameObjects::Register(vendingMachine, 2.0f, 1000.0, &Scripting::HoveredCrosshair, &InteractVendingMachine);
	}

	for (size_t i = 0; i < 4; i++)
	{
		if (cubes[i] != nullptr)
		{
			GameObjects::Register(cubes[i], 2.0f, 1000.0, &Scripting::HoveredCrosshair, &CubePickup);
		}
	}

	KeyPad::Initialize(_gameData);
	clown = Sets::Create();
	clown->GenerateRenderingInstance();
	clown->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Mobs/Clown/Clown.json")), false);
	clown->SetRotation(glm::vec3(0.0, 90.0f, 0.0f), false);
	clown->SetPosition(clownSpawnPoint, false);
	clown->SetScale(glm::vec3(0.6),true);
	clown->SetName("Clown");
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
		if (_gameData->window.IsKeyPressed(Keys::KP_1))
		{
			Peds::Ped* playerPed = Scripting::GetPlayerPed();
			for (size_t i = 0; i < 4; i++)
			{
				if (cubes[i] != nullptr && cubes[i]->GetPosition().y >= 0.0 && !validCubes[i])
				{
					playerPed->SetPosition(cubes[i]->GetWorldPosition(), true);
				}
			}
		}
		if (playRoom != nullptr)
		{
			if (_gameData->window.IsKeyPressed(Keys::KP_2))
			{
				Peds::Ped* playerPed = Scripting::GetPlayerPed();
				playerPed->SetPosition(playRoom->GetWorldPosition(), true);
			}
		}
		if (elevator != nullptr)
		{
			if (_gameData->window.IsKeyPressed(Keys::KP_3))
			{
				Peds::Ped* playerPed = Scripting::GetPlayerPed();
				playerPed->SetPosition(elevator->GetWorldPosition(), true);
			}
		}
		if (clown != nullptr)
		{
			if (_gameData->window.IsKeyPressed(Keys::KP_4))
			{
				Peds::Ped* playerPed = Scripting::GetPlayerPed();
				playerPed->SetPosition(clown->GetWorldPosition(), true);
			}
		}
	}
}

void Hospital::UnlockExit()
{
	GameObjects::Register(elevator, 2.0f, 1000.0, &Scripting::HoveredCrosshair, &InteractExit);
	GameObjects::UnRegister(vendingMachine);
	std::vector<Sets::Set*> children = vendingMachine->GetChildArray();
	for (size_t childId = 0; childId < children.size(); childId++)
	{
		if (children[childId]->GetName() == "HSP_Card")
		{
			children[childId]->Move(glm::vec3(0.0, -1.0, 0.0), true);
			break;
		}
	}
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}


void Hospital::ClownUpdate(char** _args)
{
	int x = ToFloat(_args[0]);
	int y = ToFloat(_args[1]);
	int z = ToFloat(_args[2]);
	int heading = ToFloat(_args[3]);

	if (clown != nullptr)
	{
		std::cout << "Setting clown pos : " << x << " " << y << " " << z << std::endl;
		clown->SetPosition(glm::vec3(x, y, z),false);
		clown->SetRotation(glm::vec3(0.0, heading, 0.0),true);
	}
}

void Hospital::TeddyUpdate(char** _args)
{
	int x = ToFloat(_args[0]);
	int y = ToFloat(_args[1]);
	int z = ToFloat(_args[2]);
	int heading = ToFloat(_args[3]);

	std::cout << heading << std::endl;
}
