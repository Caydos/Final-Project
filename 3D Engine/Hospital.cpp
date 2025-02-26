#include "Hospital.h"
#include "KeyPad.h"
#include "Reading.h"
#include "Pathfinding.h"
#include "Intelligence.h"
#include "LoadingScreen.h"

static bool initialized = false;
static Clock cooldown;

static Map::Stage* stage = nullptr;
Sets::Set* vendingMachine = nullptr;
Sets::Set* elevator = nullptr;
Sets::Set* entryRoom = nullptr;
Sets::Set* exitRoom = nullptr;
Sets::Set* waitingRoom = nullptr;
Sets::Set* playRoom = nullptr;
Sets::Set* giantBedRoom = nullptr;
Sets::Set* breakRoom = nullptr;

const char* cubesPaths[] = {
	"../Sets/HOSPITAL/Gameplay/HSP_N7BB.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N5BV.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N9BR.json",
	"../Sets/HOSPITAL/Gameplay/HSP_N1BJ.json",
};
const char* socketPaths[] = {
	"HSP_CubeStockBlue",
	"HSP_CubeStockGreen",
	"HSP_CubeStockRed",
	"HSP_CubeStockYellow",
};

static Sets::Set* clown = nullptr;
static Sets::Set* sockets[4] = { nullptr };
static Sets::Set* cubes[4] = { nullptr };
bool validCubes[4] = { false };
bool socketsBool[4] = { false };

static bool card = false;

static glm::vec3 socketInPosition(0.09, 0.025, 0.05);
static std::vector<glm::vec3> socketsPositions = {
	glm::vec3(9.35,0.85,10.95),
	glm::vec3(9.35,0.85,11.1),
	glm::vec3(9.35,0.85,11.230),
	glm::vec3(9.35,0.85,11.360),
};
static glm::vec3 clownSpawnPoint(1.150, 1.9, 1.0);
static std::vector<Pathfinding::Cube> nodes;
static std::vector<Bounds::Box> obstacles;
struct Monsters
{
	Sets::Set* set;
	std::vector<float> headings;
	std::vector<glm::vec3> points;
	float speed = 2.0f;
	bool forward = true;
	int currentPoint = 1;  // Add a variable to track the current target point
};
static std::vector<Monsters> monsters;


void updateMonsterPosition(Monsters& monster, float deltaTime)
{
	deltaTime = std::min(deltaTime, 1.0f);
	glm::vec3 currentPosition = monster.set->GetPosition(); // Use GetPosition to get the current position
	glm::vec3 targetPosition = monster.points[monster.currentPoint];
	glm::vec3 direction = glm::normalize(targetPosition - currentPosition);

	// Move the monster towards the target position
	glm::vec3 movement = direction * monster.speed * deltaTime;
	glm::vec3 newPosition = currentPosition + movement;

	if (glm::distance(newPosition, targetPosition) <= 0.5f)
	{
		if (monster.forward)
		{
			std::cout << monster.currentPoint << " " << monster.points[monster.currentPoint].x << " " << monster.points[monster.currentPoint].y << " " << monster.points[monster.currentPoint].z << std::endl;
			monster.set->SetRotation(glm::vec3(0.0, monster.headings[monster.currentPoint] - 90.0f, 0.0), false);
			monster.set->SetPosition(monster.points[monster.currentPoint]);
			monster.currentPoint++;
			if (monster.currentPoint >= monster.points.size() - 1)
			{
				monster.forward = false;
				monster.currentPoint -= 2;
			}
		}
		else
		{
			std::cout << monster.currentPoint << " " << monster.points[monster.currentPoint].x << " " << monster.points[monster.currentPoint].y << " " << monster.points[monster.currentPoint].z << std::endl;
			monster.set->SetRotation(glm::vec3(0.0, monster.headings[monster.currentPoint] - 90.0f, 0.0), false);
			monster.set->SetPosition(monster.points[monster.currentPoint]);
			monster.currentPoint--;
			if (monster.currentPoint < 0)
			{
				monster.forward = true;
				monster.currentPoint = 1;
			}
		}
	}
	else
	{
		monster.set->SetPosition(newPosition);
	}
}

static float cubeUpOff = 0.05;
void PathLoad(GameData* _gameData)
{
	{
		Monsters first;

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(36.7726, clownSpawnPoint.y, 60.1625));

		first.headings.push_back(90);
		first.points.push_back(glm::vec3(36.7726, clownSpawnPoint.y, 31.8185));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(36.7726, clownSpawnPoint.y, 60.1625));

		first.headings.push_back(-45);
		first.points.push_back(glm::vec3(36.7726, clownSpawnPoint.y, 61.1856));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(51.31, clownSpawnPoint.y, 61.1856));

		first.headings.push_back(0.0);
		first.points.push_back(glm::vec3(51.31, clownSpawnPoint.y, 51.0826));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(76.3642, clownSpawnPoint.y, 51.0826));

		first.headings.push_back(-180);
		first.points.push_back(glm::vec3(76.3642, clownSpawnPoint.y, 23.2297));

		first.headings.push_back(0.0);
		first.points.push_back(glm::vec3(62.7376, clownSpawnPoint.y, 23.2297));

		first.set = Sets::Create();
		first.set->GenerateRenderingInstance();
		first.set->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Mobs/Clown/Clown.json")), false);
		first.set->SetRotation(glm::vec3(0.0, 0.0f, 0.0f), false);
		first.set->SetPosition(first.points[0], false);
		first.set->SetRotation(glm::vec3(0.0, first.headings[0] - 90.0f, 0.0), false);
		first.set->SetScale(glm::vec3(0.6), true);
		first.set->SetName("Clown");


		monsters.push_back(first);
	}


	{
		Monsters first;

		first.headings.push_back(90.0);
		first.points.push_back(glm::vec3(52.462, clownSpawnPoint.y, 54.9958));

		first.headings.push_back(0.0);
		first.points.push_back(glm::vec3(57.2422, clownSpawnPoint.y, 66.0108));

		first.headings.push_back(90.0);
		first.points.push_back(glm::vec3(75.5838, clownSpawnPoint.y, 65.9147));

		first.headings.push_back(180.0);
		first.points.push_back(glm::vec3(75.6403, clownSpawnPoint.y, 76.5649));


		first.set = Sets::Create();
		first.set->GenerateRenderingInstance();
		first.set->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Mobs/Clown/Clown.json")), false);
		first.set->SetRotation(glm::vec3(0.0, 0.0f, 0.0f), false);
		first.set->SetPosition(first.points[0], false);
		first.set->SetRotation(glm::vec3(0.0, first.headings[0] - 90.0f, 0.0), false);
		first.set->SetScale(glm::vec3(0.6), true);
		first.set->SetName("Clown");
	first.set = Sets::Create();
	first.set->GenerateRenderingInstance();
	first.set->LoadFromJson(json::parse(Files::GetFileContent("../Sets/HOSPITAL/Mobs/Clown/Clown.json")), false);
	first.set->SetRotation(glm::vec3(0.0, 0.0f, 0.0f), false);
	first.set->SetPosition(first.points[0], false);
	first.set->SetRotation(glm::vec3(0.0, first.headings[0] - 90.0f, 0.0), false);
	first.set->SetScale(glm::vec3(0.6), true);
	first.set->SetName("Clown");


		monsters.push_back(first);
	}
}

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

			cubes[i]->SetPosition(sockets[i]->GetWorldPosition() + socketInPosition, true);
			std::cout << cubesPaths[i] << std::endl;
			break;
		}
	}
	GameObjects::UnRegister(_set);
}
void CubePickup(Sets::Set* _set)
{
	std::string cubeName = _set->GetName();
	if (cubeName == cubesPaths[Hospital::BLUE])
	{
		std::cout << "Picked up : Blue" << std::endl;
		GameObjects::Register(sockets[Hospital::BLUE], 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &SocketInteraction);
		socketsBool[Hospital::BLUE] = true;
	}
	else if (cubeName == cubesPaths[Hospital::GREEN])
	{
		std::cout << "Picked up : Green" << std::endl;
		GameObjects::Register(sockets[Hospital::GREEN], 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &SocketInteraction);
		socketsBool[Hospital::GREEN] = true;

	}
	else if (cubeName == cubesPaths[Hospital::RED])
	{
		std::cout << "Picked up : Red" << std::endl;
		GameObjects::Register(sockets[Hospital::RED], 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &SocketInteraction);
		socketsBool[Hospital::RED] = true;

	}
	else if (cubeName == cubesPaths[Hospital::YELLOW])
	{
		std::cout << "Picked up : Yellow" << std::endl;
		GameObjects::Register(sockets[Hospital::YELLOW], 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &SocketInteraction);
		socketsBool[Hospital::YELLOW] = true;

	}
	GameObjects::UnRegister(_set);
	_set->Move(glm::vec3(0.0, -2.0f, 0.0f), true);
}

void InteractExit(Sets::Set* _set)
{
	Levels::Set(Levels::GARDEN);
	GameObjects::UnRegister(_set);
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
			glm::vec3(0.750,0.45 + cubeUpOff,2.85),
			glm::vec3(5.05,0.3 + cubeUpOff,2.65),
			glm::vec3(7.45,0.4 + cubeUpOff,5.7),
			glm::vec3(7.45,0.4 + cubeUpOff,5.7),
			glm::vec3(10.05,0.3 + cubeUpOff,6.45),
		};

		//{// Fucked up because not following the enum
		//	for (size_t i = 0; i < 4; i++)
		//	{
		//		sockets[i] = Sets::Create();
		//		sockets[i]->SetParent(playRoom, true);
		//		sockets[i]->SetRenderingInstance(playRoom->GetRenderingInstance());
		//		sockets[i]->LoadFromJson(json::parse(Files::GetFileContent(socketPaths[i])), false);
		//		sockets[i]->SetPosition(socketsPositions[i], true);
		//		sockets[i]->SetName(socketPaths[i]);
		//	}
		//}
		cubes[BLUE] = Sets::Create();
		cubes[BLUE]->GenerateRenderingInstance();
		cubes[BLUE]->LoadFromJson(json::parse(Files::GetFileContent(cubesPaths[BLUE])), false);
		cubes[BLUE]->SetPosition(playRoom->GetWorldPosition() + (playRoomPositions[rand() % playRoomPositions.size()]), true);
		cubes[BLUE]->SetName(cubesPaths[BLUE]);
		std::vector<Sets::Set*> childArray = playRoom->GetChildArray();
		for (size_t i = 0; i < childArray.size(); i++)
		{
			for (size_t socketId = 0; socketId < 4; socketId++)
			{
				if (childArray[i]->GetName() == socketPaths[socketId])
				{
					sockets[socketId] = childArray[i];
					break;
				}
			}
		}
	}
	else
	{
		Logger::Write("Unable to find play room in hospital stage\n");
	}
	giantBedRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_GiantBedRoom.json");
	if (giantBedRoom != nullptr)
	{
		std::vector<glm::vec3> giantBedroomPositions = {
			glm::vec3(2.6,0.55 + cubeUpOff,3.85),
			glm::vec3(1.25, 0.55 + cubeUpOff, 11.4),
			glm::vec3(11.5,0.5 + cubeUpOff,11.9),
		};

		cubes[GREEN] = Sets::Create();
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
			glm::vec3(8.850,1.2 + cubeUpOff,0.15),
			glm::vec3(4.450,0.55 + cubeUpOff,1.2),
			glm::vec3(9.4,0.28 + cubeUpOff,3.55),
		};


		cubes[RED] = Sets::Create();
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
			glm::vec3(1.5,0.45 + cubeUpOff,0.15),
			glm::vec3(2.25,0.8 + cubeUpOff,3.85),
			glm::vec3(3.65,0.7 + cubeUpOff,3.25),
		};


		cubes[YELLOW] = Sets::Create();
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
		GameObjects::Register(vendingMachine, 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &InteractVendingMachine);
	}

	for (size_t i = 0; i < 4; i++)
	{
		if (cubes[i] != nullptr)
		{
			GameObjects::Register(cubes[i], 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &CubePickup);
		}
	}

	KeyPad::Initialize(_gameData);

	PathLoad(_gameData);


	cooldown.Restart();
	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	if (cooldown.GetElapsedTime() < 3000)
	{
		LoadingScreen::Render(_gameData);
		return;
	}
	KeyPad::Tick(_gameData);
	if (_gameData->window.IsKeyPressed(Keys::F8))
	{
		std::cout << " x: " << _gameData->camera->Position.x << " y: " << _gameData->camera->Position.y << " z: " << _gameData->camera->Position.z << " w: " << _gameData->camera->Yaw << std::endl;
	}
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
				//playerPed->SetPosition(elevator->GetWorldPosition(), true);
				playerPed->SetPosition(18.8492, 1.62687, 14.7918, true);
			}
		}

	}

	if (_gameData->window.IsKeyPressed(Keys::KP_4) && monsters.size() > 0 && monsters[0].set != nullptr)
	{
		Peds::Ped* playerPed = Scripting::GetPlayerPed();
		playerPed->SetPosition(monsters[0].set->GetPosition());
	}
	for (size_t i = 0; i < monsters.size(); i++)
	{
		updateMonsterPosition(monsters[i], _gameData->dt);
	}
}


bool* Hospital::GetSocketsBool()
{
	return socketsBool;
}

void Hospital::UnlockExit()
{
	GameObjects::Register(elevator, 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &InteractExit);
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
	card = true;
}

bool Hospital::GetCard()
{
	return card;
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}
