#include "Scripting.h"
#include "Scene.h"
#include "Player.h"
#include "MapManager.h"
#include "Collisions.h"
#include "Monster.h"
#include "LoadingScreen.h"
#include "FrustumCulling.h"
#include "Sprite.h"
#include "Audio.h"
#include "Crosshair.h"
#include "Interaction.h"
#include "Hospital.h"
#include "GameObject.h"
#include "MainMenu.h"
#include "Network.h"


static bool initialized = false;
std::thread interactionThread;
static Players::Player* player = nullptr;
static unsigned int FPVCam;
static Lighting::Spot* flashLight;

static Sets::Set* set;
static std::thread mazeThread;
static bool generated = false;
static std::shared_mutex playerLock;

static Clock inputClock;

static glm::vec3 spawnPoint(11.05, 1.850, 21.250);
static float spawnYaw = 0.0f;
static Texture* camOverlays[2] = { nullptr };
static Sprite camOverlay;
static Sprite crosshair;
const float crosshairSize = 10.f;

static Clock ambientClock;
static Audio::Sound* ambientLaugh;
static Audio::Sound* ambient;

static Audio::Sound* footSteps;
static Audio::Sound* footRun;
static Audio::Sound* tampon;
static Audio::Sound* doorOpen;
static Audio::Sound* doorClose;

struct Door
{
	Sets::Set* set = nullptr;
	bool opened = false;
};
std::vector<Door> doors;
static bool connected = false;

void DoorInteraction(Sets::Set* _set)
{
	for (size_t i = 0; i < doors.size(); i++)
	{
		if (doors[i].set == _set)
		{
			if (doors[i].opened)
			{
				doorClose->Play();
				_set->Rotate(glm::vec3(0.0, -90.0f, 0.0));
			}
			else
			{
				doorOpen->Play();
				_set->Rotate(glm::vec3(0.0, 90.0f, 0.0));
			}
			doors[i].opened = !doors[i].opened;
		}
	}
}
void Scripting::HoveredCrosshair()
{
	Scripting::SetCrosshairOpacity(1.0f);
}
void Generation()
{

	Clock loadingClock;
	loadingClock.Restart();
	Map::GenerateMaze(8, 1);
	std::vector<Sets::Set*>* sets = Sets::GetAll();
	for (size_t i = 0; i < sets->size(); i++)
	{
		if (sets->at(i)->GetName() == "Door")
		{
			GameObjects::Register(sets->at(i), 4.0f, 200.0, &Scripting::HoveredCrosshair, &DoorInteraction);
			Door door;
			door.set = sets->at(i);
			door.opened = false;
			doors.push_back(door);
		}
	}
	std::cout << "Loading time : " << loadingClock.GetElapsedTime() / 1000 << " seconds." << std::endl;
	generated = true;
}

void Scripting::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		std::unique_lock<std::shared_mutex> lock(playerLock);
		/*if (!Network::Connection::Create("51.178.46.32", 55301))
		{
			Logger::Write("Failed to connect");
		}*/
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

		//std::vector<Sets::Set*> children = playerPed->GetChildArray();
		//for (size_t i = 0; i < children.size(); i++)
		//{
		//	if (children[i]->GetName() == "MC_HeadV3")
		//	{
		//		Sets::Erase(children[i]);
		//		break;
		//	}
		//}

		playerPed->SetCamera(_gameData->camera);
		playerPed->SetBodyType(Physics::Type::GHOST);
		player->SetPed(playerPed);
		playerPed->SetAdditionalRotation(glm::vec3(0.0, -90.0, 0.0));

		footSteps = Audio::CreateSound();
		footSteps->LoadFromFile("../Sounds/Footsteps2.wav");
		footSteps->Loop(true);
		footSteps->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound(footSteps);

		//tampon = Audio::CreateSound();
//tampon->LoadFromFile("../Sounds/Tampon.wav");
// 
		doorOpen = Audio::CreateSound();
		doorOpen->LoadFromFile("../Sounds/DoorOpen.wav");

		ambientLaugh = Audio::CreateSound();
		ambientLaugh->LoadFromFile("../Sounds/LaughChild.wav");

		doorClose = Audio::CreateSound();
		doorClose->LoadFromFile("../Sounds/DoorClose.wav");

		footRun = Audio::CreateSound();
		footRun->LoadFromFile("../Sounds/Footsteps.wav");
		footRun->Loop(true);
		footRun->SetPosition(playerPed->GetPosition());
		player->SetFootStepSound2(footRun);

		ambient = Audio::CreateSound();
		ambient->LoadFromFile("../Sounds/AmbiantSound.wav");
		ambient->Loop(true);
		ambient->Play();


		playerPed->SetPosition(spawnPoint, true);
		playerPed->SetRotation(glm::vec3(0.0, spawnYaw, 0.0), true);
		//playerPed->SetScale(1.6f, true);

		//directionalLight = Scene::Lights::Create();
		//directionalLight->SetType(Lighting::LightType::DIRECTIONAL);
		//directionalLight->SetAmbient(glm::vec3(0.228f, 0.228f, 0.228f));
		//directionalLight->SetDiffuse(glm::vec3(0.0f, 0.0f, 0.0f));
		//directionalLight->SetSpecular(glm::vec3(0.0f, 0.0f, 0.0f));
		//directionalLight->SetDirection(glm::vec3(0.0f, 1.0f, 0.0f));
		//directionalLight->SetName("Directional");
		//directionalLight->SetActive(true);


		flashLight = Scene::Lights::CreateSpot();
		flashLight->activation = 1.0f;
		flashLight->ambient = glm::vec3(0.0f, 0.0f, 0.0f);
		flashLight->diffuse = glm::vec3(0.5f);
		flashLight->specular = glm::vec3(0.0f, 0.0f, 0.0f);
		flashLight->constant = 0.00001f;
		//flashLight->constant = 0.00f;
		flashLight->linear = 0.0f;
		flashLight->quadratic = 0.0f;
		flashLight->cutOff = glm::cos(glm::radians(20.0f));
		flashLight->outerCutOff = glm::cos(glm::radians(40.0f));

		mazeThread = std::thread(Generation);
		mazeThread.detach();


		Hospital::RegisterInteractions();
		camOverlays[0] = new Texture;
		camOverlays[0]->LoadFromFile("../Textures/OverlayOff.png");
		camOverlays[1] = new Texture;
		camOverlays[1]->LoadFromFile("../Textures/Overlay.png");

		camOverlay.Load("", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0), 1);
		crosshair.Load("../Textures/RoundCrossHair.png",
			glm::vec3(_gameData->resolution[0] / 2 - crosshairSize / 2, _gameData->resolution[1] / 2 - crosshairSize / 2, 0.0),
			glm::vec3(crosshairSize, crosshairSize, 0.0), 1);

		initialized = true;
	}
	Scene::Tick(_gameData);

	if (generated)
	{


		crosshair.SetOpacity(0.5f);
		if (_gameData->window.IsFocused())
		{

			if (ambientClock.GetElapsedTime() > 25000)
			{
				ambientLaugh->SetPosition(glm::vec3(rand() % 50, 1.5f, rand() % 50));
				ambientLaugh->Play();
				ambientClock.Restart();
			}




			//Crosshairs::Draw();
			Crosshairs::Get()->SetColor(Colors::White);
			player->Control(_gameData);
			Peds::Simulate(_gameData);

			flashLight->position = _gameData->camera->Position;
			flashLight->direction = _gameData->camera->Front;
			Lighting::UpdateSpot(flashLight);
			Scene::Lights::UpdateSpot(flashLight);

			// Cheat code
			if (_gameData->window.IsKeyPressed(Keys::F1))
			{
				player->GetPed()->SetBodyType(Physics::Type::GHOST);
			}
			if (_gameData->window.IsKeyPressed(Keys::F2))
			{
				player->GetPed()->SetBodyType(Physics::Type::RIGID);
			}
			if (_gameData->window.IsKeyPressed(Keys::F3))
			{
				player->GetPed()->SetPosition(spawnPoint);
			}
			GameObjects::Tick(_gameData);
			crosshair.Draw();
		}
		Hospital::Tick(_gameData);

		MainMenu::Tick(_gameData);
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

void Scripting::SetCrosshairOpacity(float _opacity)
{
	crosshair.SetOpacity(_opacity);
}

void Scripting::PlayerUpdate(char** _args)
{
	int x = ToFloat(_args[0]);
	int y = ToFloat(_args[1]);
	int z = ToFloat(_args[2]);
	int heading = ToFloat(_args[3]);

	std::cout << heading << std::endl;
}
