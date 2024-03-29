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
	enum PropsType
	{
		CORRIDOR,
		DOOR,
		PANELEXIT,
		BENCH,
		LIGHT,
		RAIL,
	};

	struct ManagmentText
	{
		std::vector<const char*> hospital;
		std::vector<const char*> library;
		std::vector<const char*> labo;
		std::vector<const char*> hall;
	};

	Maze::Decor Generate(int _chunckType, glm::vec3 _cellPos, int _line, int _column, int _type);
	std::vector<Maze::Decor> GenerateBedRoom(int _chunckType, glm::vec3 _cellPos, int _line, int _column);
	//void Text();
}
#endif // !OBSTACLE_H
