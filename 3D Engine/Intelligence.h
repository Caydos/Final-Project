#ifndef INTELLIGENCE_H
#define INTELLIGENCE_H
#include "Pathfinding.h"
#include "Set.h"

namespace Intelligence
{
	class AI
	{
	public:
		AI();
		~AI();

		void SetStartPosition(glm::vec3 _position); // According to the node placements
		void SetDestination(glm::vec3 _destination);
		void CalculatePath(std::vector < Bounds::Box > obstacles);
		void Update(GameData* _gameData);

		void BindToNodes(std::vector<Pathfinding::Cube>* _nodes);

	private:
		std::vector<Pathfinding::Cube>* nodes;
		Sets::Set* boundSet;
		Pathfinding::Cube position;
		Pathfinding::Cube destination;
		std::vector<Pathfinding::Cube> path;
	};

	AI* Create();
	void Erase(AI* _ai);
	void Tick(GameData* _gameData);
}

#endif // !INTELLIGENCE_H