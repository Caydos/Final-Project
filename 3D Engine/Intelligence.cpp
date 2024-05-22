#include "Intelligence.h"


static std::vector<Intelligence::AI*> intelligences;
static std::shared_mutex mutex;

Intelligence::AI::AI()
{
}

Intelligence::AI::~AI()
{
}

void Intelligence::AI::SetStartPosition(glm::vec3 _position)
{
	// Search for closest node then teleport the set on it
	Pathfinding::Cube* cube = nullptr;
	float distance = 10000;
	for (size_t i = 0; i < this->nodes->size(); i++)
	{
		Pathfinding::Cube* tempCube = &this->nodes->at(i);
		float newDist = glm::distance(_position, glm::vec3(tempCube->x, _position.y, tempCube->y));
		if (newDist < distance)
		{
			cube = tempCube;
			distance = newDist;
		}
	}
	if (cube != nullptr)
	{
		std::cout << "Found start node at : " << cube->x << " " << cube->y << std::endl;
	}
	this->position = *cube;
}

void Intelligence::AI::SetDestination(glm::vec3 _destination)
{
	// Search for closest node then save it's position to destination
	Pathfinding::Cube* cube = nullptr;
	float distance = 10000;
	for (size_t i = 0; i < this->nodes->size(); i++)
	{
		Pathfinding::Cube* tempCube = &this->nodes->at(i);
		float newDist = glm::distance(_destination, glm::vec3(tempCube->x, _destination.y, tempCube->y));
		if (newDist < distance)
		{
			cube = tempCube;
			distance = newDist;
		}
	}
	if (cube != nullptr)
	{
		std::cout << "Found end node at : " << cube->x << " " << cube->y  << " " << distance << std::endl;
	}
	this->destination = *cube;
}

void Intelligence::AI::CalculatePath(std::vector < Bounds::Box > obstacles)
{
	std::cout << "Obstacle count : " << obstacles.size() << std::endl;
	path = Pathfinding::a_star(&this->position, &this->destination, *this->nodes, obstacles,4.5f);
	for (size_t cubeId = 0; cubeId < path.size(); cubeId++)
	{
		Sets::Set* ctPart = Sets::Create();
		ctPart->SetName("ctPart");
		ctPart->GenerateRenderingInstance();
		ctPart->AppplyVisibility();

		Blocks::Block block;
		block.GenerateModel();
		Blocks::MaterialCheck(&block, "HSP_Green");
		Blocks::BlockType* type = block.GetType();

		block.SetScale(0.5f);
		ctPart->InsertBlock(block);

		ctPart->SetPosition(glm::vec3(path[cubeId].x, 1.2, path[cubeId].y));
		std::cout << path[cubeId].x << " " << path[cubeId].y << std::endl;
	}

}

void Intelligence::AI::Update(GameData* _gameData)
{


}

void Intelligence::AI::BindToNodes(std::vector<Pathfinding::Cube>* _nodes)
{
	this->nodes = _nodes;
}

Intelligence::AI* Intelligence::Create()
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	AI* intelligence = new AI;
	intelligences.push_back(intelligence);
	return intelligence;
}

void Intelligence::Erase(AI* _ai)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	for (size_t i = 0; i < intelligences.size(); i++)
	{
		if (intelligences[i] == _ai)
		{
			delete intelligences[i];
			return;
		}
	}
}

void Intelligence::Tick(GameData* _gameData)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	for (size_t i = 0; i < intelligences.size(); i++)
	{
		intelligences[i]->Update(_gameData);
	}
}
