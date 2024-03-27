#ifndef PROPS_H
#define PROPS_H

#include "Common.h"
#include "Scene.h"
#include "Maze.h"
#include "Set.h"

//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define PROPS_LUCK 75

namespace Props
{

	struct ManagmentText
	{
		std::vector<const char*> hospital;
		std::vector<const char*> library;
		std::vector<const char*> labo;
		std::vector<const char*> hall;
	};

	std::vector<Maze::Decor> Generate(int _chunckType, glm::vec3 _cellPos, int _line, int _column);
	//void Text();
}
#endif // !OBSTACLE_H
