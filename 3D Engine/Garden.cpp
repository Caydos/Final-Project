#include "Garden.h"
#include "LoadingScreen.h"
static bool initialized = false;
static Clock cooldown;
void Garden::RegisterInteractions()
{
}

void Garden::Initialize(GameData* _gameData)
{

	cooldown.Restart();
	
	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	playerPed->SetPosition(glm::vec3(18.8791, 4.12687, 15.7454));
	playerPed->SetRotation(glm::vec3(0,-90.3,0));
	initialized = true;
}

void Garden::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	if (cooldown.GetElapsedTime() < 3000)
	{
		LoadingScreen::Render(_gameData);
		return;
	}
}

void Garden::CleanUp(GameData* _gameData)
{
}
