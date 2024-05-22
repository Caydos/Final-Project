#include "Ambiance.h"
#include "Audio.h"
#include "Levels.h"
#include "Clock.h"

static bool initialized = false;

//Hospital sound//
static Audio::Sound* laughChild;
static Audio::Sound* ambiance1;
static Audio::Sound* ambiance2;
static Audio::Sound* ambiance3;
static Audio::Sound* clownLaught;
static Audio::Sound* fallingObject1;
static Audio::Sound* fallingObject2;
static Audio::Sound* brokenNeon;
static Audio::Sound* waterDrip;
static Audio::Sound* fridge;
//Garden sound//
static Audio::Sound* branchCrack;
static Audio::Sound* nightSound;
//Library sound//
static Audio::Sound* BookFalling1;
static Audio::Sound* BookFalling2;
static Audio::Sound* TurningPage1;
static Audio::Sound* TurningPage2;
static Audio::Sound* TurningPage3;



static Clock testClock;

void Ambiance::Initialize(GameData* _gameData)
{
	srand(time(NULL));

	testClock.Restart();
	// Load de tous les sons ICI

	/////HOSPITAL SOUND////
	laughChild = Audio::CreateSound();
	laughChild->LoadFromFile("../Sounds/Hospital/LaughChild.wav");
	laughChild->SetVolume(1.0f);

	ambiance1 = Audio::CreateSound();
	ambiance1->LoadFromFile("../Sounds/Hospital/AmbiantSound1.wav");
	ambiance1->SetVolume(1.0f);

	ambiance2 = Audio::CreateSound();
	ambiance2->LoadFromFile("../Sounds/Hospital/AmbiantSound2.wav");
	ambiance2->SetVolume(1.0f);

	ambiance3 = Audio::CreateSound();
	ambiance3->LoadFromFile("../Sounds/Hospital/AmbiantSound4Boucle.wav");
	ambiance3->SetVolume(1.0f);

	clownLaught = Audio::CreateSound();
	clownLaught->LoadFromFile("../Sounds/Hospital/ClowDistantLaugh.wav");
	clownLaught->SetVolume(1.0f);

	fallingObject1 = Audio::CreateSound();
	fallingObject1->LoadFromFile("../Sounds/Hospital/FallingObject1.wav");
	fallingObject1->SetVolume(1.0f);

	fallingObject2 = Audio::CreateSound();
	fallingObject2->LoadFromFile("../Sounds/Hospital/FallingObject2.wav");
	fallingObject2->SetVolume(1.0f);

	brokenNeon = Audio::CreateSound();
	brokenNeon->LoadFromFile("../Sounds/Hospital/Neon_On_Off.wav");
	brokenNeon->SetVolume(1.0f);

	waterDrip = Audio::CreateSound();
	waterDrip->LoadFromFile("../Sounds/Hospital/WaterDrip.wav");
	waterDrip->SetVolume(1.0f);

	fridge = Audio::CreateSound();
	fridge->LoadFromFile("../Sounds/Hospital/Fridge.wav");
	fridge->Loop(true);
	fridge->SetVolume(0.1f);

	/////GARDEN SOUND/////
	branchCrack = Audio::CreateSound();
	branchCrack->LoadFromFile("../Sounds/GARDEN/Ambiant Sounds/BranchCrack1.wav");
	branchCrack->SetVolume(1.0f);

	nightSound = Audio::CreateSound();
	nightSound->LoadFromFile("../Sounds/GARDEN/Ambiant Sounds/NightSounds.wav");
	nightSound->Loop(true);
	nightSound->SetVolume(1.0f);

	/////LIBRARY SOUND/////
	BookFalling1 = Audio::CreateSound();
	BookFalling1->LoadFromFile("../Sounds/LIBRARY/Ambiant Sounds/BookFalling1.wav");
	BookFalling1->SetVolume(1.0f);

	BookFalling2 = Audio::CreateSound();
	BookFalling2->LoadFromFile("../Sounds/LIBRARY/Ambiant Sounds/BookFalling2.wav");
	BookFalling2->SetVolume(1.0f);

	TurningPage1 = Audio::CreateSound();
	TurningPage1->LoadFromFile("../Sounds/LIBRARY/Ambiant Sounds/TurningPage1.wav");
	TurningPage1->SetVolume(1.0f);

	TurningPage2 = Audio::CreateSound();
	TurningPage2->LoadFromFile("../Sounds/LIBRARY/Ambiant Sounds/TurningPage2.wav");
	TurningPage2->SetVolume(1.0f);

	TurningPage3 = Audio::CreateSound();
	TurningPage3->LoadFromFile("../Sounds/LIBRARY/Ambiant Sounds/TurningPage3.wav");
	TurningPage3->SetVolume(1.0f);


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
		//Son d'ambiance constante
		if (!fridge->IsPlaying())
		{
			fridge->Play();
		}
		fridge->SetPosition(_gameData->camera->Position);


		if (testClock.GetElapsedTime() < 15000)//miliseconds
		{
			break;
		}

		int randomHospitalSound = rand() % 9;
		std::cout << "Son hopital numero : " << randomHospitalSound << std::endl;

		//Son d'ambiance aleatoire (sound event)
		if (randomHospitalSound == 0)
		{
			laughChild->SetPosition(_gameData->camera->Position);
			laughChild->Play();
		}
		else if (randomHospitalSound == 1)
		{
			ambiance2->SetPosition(_gameData->camera->Position);
			ambiance2->Play();
		}
		else if (randomHospitalSound == 2)
		{
			ambiance3->SetPosition(_gameData->camera->Position);
			ambiance3->Play();
		}
		else if (randomHospitalSound == 3)
		{
			clownLaught->SetPosition(_gameData->camera->Position);
			clownLaught->Play();
		}
		else if (randomHospitalSound == 4)
		{
			fallingObject1->SetPosition(_gameData->camera->Position);
			fallingObject1->Play();
		}
		else if (randomHospitalSound == 5)
		{
			fallingObject2->SetPosition(_gameData->camera->Position);
			fallingObject2->Play();
		}
		else if (randomHospitalSound == 6)
		{
			brokenNeon->SetPosition(_gameData->camera->Position);
			brokenNeon->Play();
		}
		else if (randomHospitalSound == 7)
		{
			waterDrip->SetPosition(_gameData->camera->Position);
			waterDrip->Play();
		}
		else if (randomHospitalSound == 8)
		{
			ambiance1->SetPosition(_gameData->camera->Position);
			ambiance1->Play();
		}

		testClock.Restart();
		break;
	}
	case Levels::Level::LIBRARY:
	{
		if (testClock.GetElapsedTime() < 15000)//miliseconds
		{
			break;
		}

		int randomLibrarySound = rand() % 7;
		std::cout << "Son library numero : " << randomLibrarySound << std::endl;

		//Son d'ambiance aleatoire (sound event)
		if (randomLibrarySound == 0)
		{
			ambiance1->SetPosition(_gameData->camera->Position);
			ambiance1->Play();
		}
		else if (randomLibrarySound == 1)
		{
			ambiance2->SetPosition(_gameData->camera->Position);
			ambiance2->Play();
		}
		else if (randomLibrarySound == 2)
		{
			BookFalling1->SetPosition(_gameData->camera->Position);
			BookFalling1->Play();
		}
		else if (randomLibrarySound == 3)
		{
			BookFalling2->SetPosition(_gameData->camera->Position);
			BookFalling2->Play();
		}
		else if (randomLibrarySound == 4)
		{
			TurningPage1->SetPosition(_gameData->camera->Position);
			TurningPage1->Play();
		}
		else if (randomLibrarySound == 5)
		{
			TurningPage2->SetPosition(_gameData->camera->Position);
			TurningPage2->Play();
		}
		else if (randomLibrarySound == 6)
		{
			TurningPage3->SetPosition(_gameData->camera->Position);
			TurningPage3->Play();
		}

		testClock.Restart();
		break;
	}
	case Levels::Level::GARDEN:
	{
		///Son d'ambiance constante
		if (!nightSound->IsPlaying())
		{
			nightSound->Play();
		}
		nightSound->SetPosition(_gameData->camera->Position);

		if (testClock.GetElapsedTime() < 15000)//miliseconds
		{
			break;
		}

		branchCrack->SetPosition(_gameData->camera->Position);
		branchCrack->Play();

		testClock.Restart();
		break;
	}
	default:
		break;
	}
}
