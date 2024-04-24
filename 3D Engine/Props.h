#ifndef PROPS_H
#define PROPS_H

#include "Common.h"
#include "Scene.h"
#include "Set.h"
#include "Maze.h"

//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define PROPS_LUCK 85
#define NB_CLUE 3

namespace Props
{
	enum PropsType
	{
		DOOR,
		PANELEXIT,
		BENCH,
		LIGHT,
		RAIL,
		BOOK,
		AD,
	};

	std::vector<Map::Decor> Generate(Map::Cell _cell);
	std::vector<Map::Decor> GenerateRoom(Map::Chunck& _chunk);
	Map::ManagmentTextProps TextChunck(int _stageType);
}
#endif // !OBSTACLE_H
