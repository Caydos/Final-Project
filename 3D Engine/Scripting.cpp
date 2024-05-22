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
static Audio::Sound* fastBreathing;


static bool connected = false;
static int gameState = 0;
static std::shared_mutex gameStateMutex;
static Sprite bloc1;
static Sprite bloc2;
static Sprite bloc3;
static Sprite bloc4;
static Sprite hud;
static Sprite hudCard;
static Sprite card;


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


	if (Levels::Level::HOSPITAL)
	{
		footSteps = Audio::CreateSound();
		footSteps->LoadFromFile("../Sounds/MC/Walk/ConcreteWalk.wav");
		footSteps->Loop(true);
		footSteps->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound(footSteps);

		footRun = Audio::CreateSound();
		footRun->LoadFromFile("../Sounds/MC/Run/ConcreteRun.wav");
		footRun->Loop(true);
		footRun->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound2(footRun);
	}
	else if (Levels::Level::GARDEN)
	{
		footSteps = Audio::CreateSound();
		footSteps->LoadFromFile("../Sounds/MC/Walk/GravelWalk.wav");
		footSteps->Loop(true);
		footSteps->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound(footSteps);

		footRun = Audio::CreateSound();
		footRun->LoadFromFile("../Sounds/MC/Run/GravelRun.wav");
		footRun->Loop(true);
		footRun->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound2(footRun);
	}

	fastBreathing = Audio::CreateSound();
	fastBreathing->LoadFromFile("../Sounds/MC/MC after run.wav");
	fastBreathing->Loop(true);
	fastBreathing->SetPosition(playerPed->GetPosition());
	player->SetFastBreathing(fastBreathing);




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
	camOverlays[0]->LoadFromFile("../Textures/Overlay.png");

	hud.Load("../Textures/Hud.png", glm::vec3(157, 965, 0), glm::vec3(300, 65, 0), 1);
	hudCard.Load("../Textures/Hud_card.png", glm::vec3(1620, 940, 0), glm::vec3(150, 76, 0), 1);
	bloc1.Load("../Textures/Bloc1.png", glm::vec3(167, 974, 0), glm::vec3(51, 46, 0), 1);
	bloc2.Load("../Textures/Bloc2.png", glm::vec3(243, 974, 0), glm::vec3(52, 46, 0), 1);
	bloc3.Load("../Textures/Bloc3.png", glm::vec3(319, 974, 0), glm::vec3(52, 46, 0), 1);
	bloc4.Load("../Textures/Bloc4.png", glm::vec3(395, 974, 0), glm::vec3(52, 46, 0), 1);
	card.Load("../Textures/Card.png", glm::vec3(1623, 943, 0), glm::vec3(144, 70, 0), 1);


	camOverlay.Load("", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0), 1);
	Levels::Set(Levels::ENTRANCE);
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
			if (!MainMenu::IsOpen())
			{
				Levels::Tick(_gameData);
			}

			Menu::Tick(_gameData);
		}
		else
		{
			VictoryDefeat::Tick(_gameData, gameState);
		}
		camOverlay.SetTexture(camOverlays[0]);
		if (GetGameState() == 0)
		{
			camOverlay.Draw();
		}
		if (Levels::Get() == Levels::HOSPITAL)
		{

			if (_gameData->window.IsFocused())
			{
				hud.Draw();
				hudCard.Draw();
				bool* boolSockets = Hospital::GetSocketsBool();
				if (boolSockets[Hospital::BLUE])
				{
					bloc1.Draw();
				}
				if (boolSockets[Hospital::GREEN])
				{
					bloc2.Draw();
				}
				if (boolSockets[Hospital::RED])
				{
					bloc3.Draw();
				}
				if (boolSockets[Hospital::YELLOW])
				{
					bloc4.Draw();
				}
				if (Hospital::GetCard())
				{
					card.Draw();
				}
			}
		}
	}
	else
	{
		LoadingScreen::Render(_gameData);
	}

	if (_gameData->window.IsKeyPressed(Keys::KP_9))
	{
		Scripting::SetgameState(-1);
	}

	if (_gameData->window.IsKeyPressed(Keys::KP_6) && Levels::Get() == Levels::GARDEN)
	{
		
		player->GetPed()->SetPosition(64.0045, 4.12687, 55.9758);
	}

	if (_gameData->window.IsKeyPressed(Keys::F11))
	{
		Scripting::SetgameState(0);
		Levels::Set(Levels::HOSPITAL);
		player->GetPed()->SetPosition(53.8328, 1.62688 ,28.5161, true);

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

int Scripting::GetGameState()
{
	return gameState;
}
