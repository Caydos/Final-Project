#include "Doors.h"
#include "MapManager.h"
#include "Scripting.h"

static Audio::Sound* doorOpen;
static Audio::Sound* doorClose;
struct Door
{
	Sets::Set* set = nullptr;
	bool opened = false;
};
static std::vector<Door> doors;

void DoorInteraction(Sets::Set* _set)
{
	doorOpen = Audio::CreateSound();
	doorOpen->LoadFromFile("../Sounds/DoorOpen.wav");
	doorClose = Audio::CreateSound();
	doorClose->LoadFromFile("../Sounds/DoorClose.wav");

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

void Doors::RegisterAll()
{
	std::vector<Sets::Set*>* sets = Sets::GetAll();
	for (size_t i = 0; i < sets->size(); i++)
	{
		if (sets->at(i)->GetName() == "Door")
		{
			GameObjects::Register(sets->at(i), 4.0f, 200.0, &Interactions::Overlay::HoveredCrosshair, &DoorInteraction);
			Door door;
			door.set = sets->at(i);
			door.opened = false;
			doors.push_back(door);
		}
	}
}
