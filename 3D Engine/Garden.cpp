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
static float y = 2.5f;

void Garden::RegisterInteractions()
{
}

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
static void PathLoad(GameData* _gameData)
{
	{
		Monsters first;

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(36.7726, y, 60.1625));

		first.headings.push_back(90);
		first.points.push_back(glm::vec3(36.7726, y, 31.8185));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(36.7726, y, 60.1625));

		first.headings.push_back(-45);
		first.points.push_back(glm::vec3(36.7726, y, 61.1856));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(51.31, y, 61.1856));

		first.headings.push_back(0.0);
		first.points.push_back(glm::vec3(51.31, y, 51.0826));

		first.headings.push_back(-90);
		first.points.push_back(glm::vec3(76.3642, y, 51.0826));

		first.headings.push_back(-180);
		first.points.push_back(glm::vec3(76.3642, y, 23.2297));

		first.headings.push_back(0.0);
		first.points.push_back(glm::vec3(62.7376, y, 23.2297));

		first.set = Sets::Create();
		first.set->GenerateRenderingInstance();
		first.set->LoadFromJson(json::parse(Files::GetFileContent("../Sets/GARDEN/Mobs/Sheep_Body.json")), false);
		first.set->SetRotation(glm::vec3(0.0, 0.0f, 0.0f), false);
		first.set->SetPosition(first.points[0], false);
		first.set->SetRotation(glm::vec3(0.0, first.headings[0] - 90.0f, 0.0), false);
		first.set->SetScale(glm::vec3(0.6), true);
		first.set->SetName("Sheep");


		monsters.push_back(first);
	}
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

	PathLoad(_gameData);


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
	if (_gameData->window.IsKeyPressed(Keys::KP_4))
	{
		Scripting::GetPlayerPed()->SetPosition(monsters[0].set->GetPosition());
	}



}

void  Garden::InteractExit(Sets::Set* _set)
{
	Scripting::SetgameState(1);
}

void Garden::CleanUp(GameData* _gameData)
{
}
