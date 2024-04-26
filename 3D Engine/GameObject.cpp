#include "GameObject.h"
#include "RayCasting.h"

static std::shared_mutex mutex;
static std::vector<GameObjects::Object> objectArray;

void GameObjects::Register(Sets::Set* _set, float _range)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	Object obj;
	obj.set = _set;
	obj.range = _range;
	objectArray.push_back(obj);
}

void GameObjects::UnRegister(Sets::Set* _set)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	for (size_t objectId = 0; objectId < objectArray.size(); objectId++)
	{
		if (objectArray[objectId].set == _set)
		{
			objectArray.erase(objectArray.begin() + objectId);
			return;
		}
	}
}

void GameObjects::Tick(GameData* _gameData)
{
	std::shared_lock<std::shared_mutex> lock(mutex);
	for (size_t objectId = 0; objectId < objectArray.size(); objectId++)
	{
		//objectArray[objectId].set
	}
}
