#ifndef MAZE_H
#define MAZE_H

#include "Common.h"
#include "Scene.h"

//////////////////////////////////////// CELL PRESET ////////////////////////////////////////
#define NB_CELL 6
#define CELL_W 2.5
#define NB_CELL_BRICK 10
#define BRICK_W (CELL_W/NB_CELL_BRICK)
//////////////////////////////////////// CHUNK PRESET ////////////////////////////////////////
#define NB_CHUNK_H 3
#define CHUNK_W (CELL_W*NB_CELL)
//////////////////////////////////////// STAGE PRESET ////////////////////////////////////////
#define NB_STAGE_H 6
//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define SHORTCUT_LUCK 75

namespace Maze
{
	enum WallEnum
	{
		BOTTOM,
		LEFT,
		RIGHT,
		UP,
	};

	enum ChunckEnum
	{
		LABY,
		GARDEN,
		HOSPITAL,
		TOWER,
		ROOM,
		EXTERIOR,
	};

	struct Wall
	{
		Scene::World::Component* decor;
		bool isVisible;
	};

	struct Cell
	{
		Scene::World::Component* ground;
		Scene::World::Component* roof;

		bool isVisited;
		int lastCase;
		std::vector<Wall> wallMissingList;
		std::vector<Wall> wallList;
	};

	struct Chunck
	{
		std::vector<Cell> cellList;
		int type;
	};

	struct Stage
	{
		std::vector<Chunck> chunkList;
		int type;
	};

	void Generate();
}

#endif // !OBSTACLE_H