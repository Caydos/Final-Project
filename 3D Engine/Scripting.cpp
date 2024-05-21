#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "MapManager.h"
#include "LoadingScreen.h"
#include "Audio.h"
#include "Network.h"
#include "Ambiance.h"
#include "Cheats.h"
#include "Doors.h"

static bool initialized = false;
std::thread interactionThread;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lighting::Spot* flashLight;

static std::thread mazeThread;
static bool generated = false;
static std::shared_mutex playerLock;

static Clock inputClock;

static glm::vec3 spawnPoint(11.05, 1.850, 21.250);
static float spawnYaw = 0.0f;
static Texture* camOverlays[2] = { nullptr };
static Sprite camOverlay;


static Audio::Sound* footSteps;
static Audio::Sound* footRun;
static Audio::Sound* tampon;


static bool connected = false;
static int gameState = 0;
static std::shared_mutex gameStateMutex;


void Generation()
{
	Clock loadingClock;
	loadingClock.Restart();
	Map::GenerateMaze(7, 2);
	Doors::RegisterAll();
	std::vector<Lighting::Spot*>* spots = Scene::Lights::GetSpots();

	std::cout << spots->size() << " Lights in the scene" << std::endl;
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;

	loadingClock.Restart();
	//PathLoad(GetGameData());


	std::cout << "Node gen time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	generated = true;
}

void Scripting::Initialize(GameData* _gameData)
{
	std::unique_lock<std::shared_mutex> lock(playerLock);
	//if (!Network::Connection::Create("51.178.46.32", 55301))
	//{
	//	//Logger::Write("Failed to connect");
	//}
	// WARNING : ZOMBIE THREAD RN
	interactionThread = std::thread(&Interactions::Thread, true);
	interactionThread.detach();

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
	playerPed->SetScale(glm::vec3(1.35f), true);

	playerPed->SetCamera(_gameData->camera);
	playerPed->SetBodyType(Physics::Type::GHOST);
	player->SetPed(playerPed);
	playerPed->SetAdditionalRotation(glm::vec3(0.0, -90.0, 0.0));

	footSteps = Audio::CreateSound();
	footSteps->SetName("Walk");
	footSteps->LoadFromFile("../Sounds/MC/Walk.wav");
	footSteps->Loop(true);
	footSteps->SetPosition(playerPed->GetPosition());
	player->SetFootStepSound(footSteps);

	footRun = Audio::CreateSound();
	footRun->SetName("Run");
	footRun->LoadFromFile("../Sounds/MC/Run.wav");
	footRun->Loop(true);
	footRun->SetPosition(playerPed->GetPosition());
	player->SetFootStepSound2(footRun);



	playerPed->SetPosition(spawnPoint, true);
	playerPed->SetRotation(glm::vec3(0.0, spawnYaw, 0.0), true);


	flashLight = Scene::Lights::CreateSpot();
	flashLight->activation = 1.0f;
	flashLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
	flashLight->diffuse = glm::vec3(0.5f);
	flashLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);
	flashLight->constant = 0.00001f;
	flashLight->linear = 0.0f;
	flashLight->quadratic = 0.0f;
	flashLight->cutOff = glm::cos(glm::radians(20.0f));
	flashLight->outerCutOff = glm::cos(glm::radians(40.0f));

	mazeThread = std::thread(Generation);
	mazeThread.detach();
	//Generation();


	Hospital::RegisterInteractions();
	camOverlays[0] = new Texture;
	camOverlays[0]->LoadFromFile("../Textures/OverlayOff.png");
	camOverlays[1] = new Texture;
	camOverlays[1]->LoadFromFile("../Textures/Overlay.png");



	camOverlay.Load("", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0), 1);

	initialized = true;
}

void Scripting::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	Scene::Tick(_gameData);

	if (generated)
	{
		std::shared_lock<std::shared_mutex> lock(gameStateMutex);
		if (gameState == 0)
		{
			lock.unlock();
			if (_gameData->window.IsFocused())
			{
				player->Control(_gameData);
				Peds::Simulate(_gameData);

				flashLight->position = _gameData->camera->Position;
				flashLight->direction = _gameData->camera->Front;
				Lighting::UpdateSpot(flashLight);
				Scene::Lights::UpdateSpot(flashLight);
				
				Cheats::Tick(_gameData);
				GameObjects::Tick(_gameData);
				Interactions::Overlay::Draw(_gameData); // Crosshair
			}
			Levels::Tick(_gameData);

			Menu::Tick(_gameData);
		}
		else
		{
			VictoryDefeat::Tick(_gameData, gameState);
		}
		camOverlay.SetTexture(camOverlays[0]);
		if (connected)
		{
			camOverlay.SetTexture(camOverlays[1]);
		}
		camOverlay.Draw();
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


Peds::Ped* Scripting::GetPlayerPed()
{
	std::shared_lock<std::shared_mutex> lock(playerLock);
	if (player == nullptr)
	{
		return nullptr;
	}
	return player->GetPed();
}


void Scripting::SetConnectionState(bool _state)
{
	connected = _state;
}
void Scripting::SetgameState(int _state)
{
	std::unique_lock<std::shared_mutex> lock(gameStateMutex);
	gameState = _state;
}