#include "Garden.h"
#include "LoadingScreen.h"
#include "VictoryDefeat.h"

static bool initialized = false;
static Clock cooldown;
static Sets::Set* elevator = nullptr;
static Map::Stage* stage = nullptr;
static Sets::Set* exitRoom = nullptr;
static Sets::Set* door1 = nullptr;
static Sets::Set* door2 = nullptr;


void Garden::RegisterInteractions()
{
}

void Garden::Initialize(GameData* _gameData)
{

	cooldown.Restart();
	Peds::Ped* playerPed = Scripting::GetPlayerPed();

	playerPed->SetPosition(glm::vec3(18.8791, 4.12687, 15.7454));
	playerPed->SetRotation(glm::vec3(0, -90.3, 0));


	stage = Map::GetStage(Map::StageEnum::LABO);

	if (stage == nullptr)
	{
		Logger::Write("Unable to find Garden level stage\n");
		return;
	}

	exitRoom = Map::SearchForSetInStage(stage, "../Sets/GARDEN/Map/GRD_EXIT.json");
	if (exitRoom != nullptr)
	{
		std::vector<Sets::Set*> children = exitRoom->GetChildArray();

		for (size_t childId = 0; childId < children.size(); childId++)
		{

			if (children[childId]->GetName() == "HSP_Elevator")
			{
				elevator = children[childId];
				GameObjects::Register(elevator, 2.0f, 1000.0, &Interactions::Overlay::HoveredCrosshair, &InteractExit);
				break;
			}
		}
		if (elevator == nullptr)
		{
			Logger::Write("Unable to find elevator in Garden stage\n");
		}
	}
	else
	{
		Logger::Write("Unable to find exit room in Garden stage\n");
	}




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

	if (_gameData->window.IsKeyPressed(Keys::KP_8))
	{
		Scripting::SetgameState(1);
	}



}

void  Garden::InteractExit(Sets::Set* _set)
{
	Scripting::SetgameState(1);
}

void Garden::CleanUp(GameData* _gameData)
{
}
