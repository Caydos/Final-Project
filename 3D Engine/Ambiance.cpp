#include "Ambiance.h"
#include "Audio.h"
#include "Levels.h"
#include "Clock.h"

static bool initialized = false;
static Audio::Sound* exampleSound;
static Audio::Sound* hospitalAmbiance;
static Clock testClock;

void Ambiance::Initialize(GameData* _gameData)
{
	testClock.Restart();
	// Load de tous les sons ICI
	exampleSound = Audio::CreateSound();
	exampleSound->LoadFromFile("../Sounds/Hospital/LaughChild.wav");
	
	hospitalAmbiance = Audio::CreateSound();
	hospitalAmbiance->LoadFromFile("../Sounds/Hospital/AmbiantSound1.wav");
	hospitalAmbiance->Loop(true);
	hospitalAmbiance->SetVolume(0.1f);
	initialized = true;
}

void Ambiance::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	switch (Levels::Get())
	{
	case Levels::Level::ENTRANCE:
	{

		break;
	}
	case Levels::Level::HOSPITAL:
	{
		if (!hospitalAmbiance->IsPlaying())
		{
			hospitalAmbiance->Play();
			std::cout << "Playing" << std::endl;
		}
		hospitalAmbiance->SetPosition(_gameData->camera->Position);
		if (testClock.GetElapsedTime() < 15000)//miliseconds
		{
			break;
		}
		exampleSound->SetPosition(_gameData->camera->Position);
		exampleSound->Play();
		testClock.Restart();
		break;
	}
	case Levels::Level::LIBRARY:
	{

		break;
	}
	case Levels::Level::GARDEN:
	{

		break;
	}
	default:
		break;
	}
}
