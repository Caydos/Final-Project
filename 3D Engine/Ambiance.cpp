#include "Ambiance.h"
#include "Audio.h"
#include "Levels.h"
#include "Clock.h"

static bool initialized = false;
static Audio::Sound* exampleSound;
static Clock testClock;

void Ambiance::Initialize(GameData* _gameData)
{
	testClock.Restart();
	// Load de tous les sons ICI
	exampleSound = Audio::CreateSound();
	exampleSound->LoadFromFile("../Sounds/Hospital/LaughChild.wav");


	initialized = true;
}

void Ambiance::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	if (testClock.GetElapsedTime() < 15000)//miliseconds
	{
		return;
	}

	switch (Levels::Get())
	{
	case Levels::Level::ENTRANCE:
	{

		break;
	}
	case Levels::Level::HOSPITAL:
	{
		exampleSound->Play();
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
	testClock.Restart();
}
