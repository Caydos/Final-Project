#include "Hospital.h"
#include "Sprite.h"
#include "Interaction.h"
#include "KeyPad.h"


static bool initialized = false;
static Map::Stage* stage = nullptr;
void Hospital::RegisterInteractions()
{
	Interactions::Register("KeyPad", &KeyPad::Interaction);
}

void Hospital::Initialize(GameData* _gameData)
{
	if (initialized) { return; }
	Players::Player* player = Scripting::GetPlayer();
	Peds::Ped* playerPed = Scripting::GetPlayerPed();
	stage = Map::GetStage(Map::StageEnum::LIBRARY);
	if (stage == nullptr)
	{
		Logger::Write("Unable to find hospital level stage\n");
		return;
	}
	Sets::Set* entryRoom = nullptr;
	Sets::Set* exitRoom = nullptr;

	exitRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");
	entryRoom = Map::SearchForSetInStage(stage, "../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
	playerPed->SetPosition(entryRoom->GetPosition() + glm::vec3(3,2,0.65), true);
	playerPed->SetHeading(0.0f); // Force to look front

	initialized = true;
}

void Hospital::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	KeyPad::Tick(_gameData);
	if (_gameData->window.IsKeyPressed(Keys::F4))
	{
		Interactions::Trigger("KeyPad", "");
	}
}

void Hospital::CleanUp(GameData* _gameData)
{

	initialized = false;
}
