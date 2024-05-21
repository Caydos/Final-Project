#include "Intelligence.h"


static std::vector<Intelligence::AI*> intelligences;

Intelligence::AI::AI()
{
}

Intelligence::AI::~AI()
{
}

void Intelligence::AI::SetStartPosition()
{
	// Search for closest node then teleport the set on it
}

void Intelligence::AI::SetDestination(glm::vec3 _destination)
{
	// Search for closest node then save it's position to destination
}

void Intelligence::AI::Update(GameData* _gameData)
{


}

Intelligence::AI* Intelligence::Create()
{
	return nullptr;
}

void Intelligence::Erase(AI* _ai)
{
}

void Intelligence::Tick(GameData* _gameData)
{
	for (size_t i = 0; i < intelligences.size(); i++)
	{
		intelligences[i]->Update(_gameData);
	}
}
