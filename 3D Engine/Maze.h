#ifndef MAZE_H
#define MAZE_H

#include "Common.h"
#include "Scene.h"
#include "Set.h"

//////////////////////////////////////// CELL PRESET ////////////////////////////////////////
#define NB_CELL 5
#define CELL_W 2
#define NB_CELL_BRICK 10
#define BRICK_W (CELL_W/NB_CELL_BRICK)
//////////////////////////////////////// STAGE PRESET ////////////////////////////////////////
#define NB_STAGE_H 1
#define OFFSET_STAGE 1
//////////////////////////////////////// MAP PRESET ////////////////////////////////////////
#define NB_MAP_H 3
#define MAP_W (CELL_W*NB_CELL)
//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define SHORTCUT_LUCK 75
#define BUILDING_LUCK 40

namespace Maze
{
	enum WallEnum
	{
		BOTTOM,
		LEFT,
		RIGHT,
		UP,
	};

	enum CellEnum
	{
		BEDROOM,
		CORRIDOR,
	};

	enum ChunckEnum
	{
		LABY,
		GARDEN,
		HOSPITAL,
		EXIT,
		ROOM,
		PARCEL,
		BALCONY,
		EMPTY,
	};

	enum StageEnum
	{
		HOSPITALS,
		FACTORY,
		LABO,
		FIELDS,
	};

	enum TextEnum
	{
		WALL,
		GROUND,
		WALLROT,
		ROOF,
		WALLMISSING,
		WALLMISSINGROT,
	};

	struct Decor
	{
		Sets::Set* decor;
		bool isVisible;
		const char* name;
		glm::vec3 pos;
	};

	struct Cell
	{
		bool isVisited;
		int lastCase;
		std::vector<Decor> wallMissingList;
		std::vector<Decor> ground;
		std::vector<Decor> wallList;
	};

	struct Chunck
	{
		std::vector<Cell> cellList;
		std::vector<Decor> decor;

		int type;
		float sizeOf;
	};

	struct Stage
	{
		Chunck chunk;
		int type;
		float sizeOf;
	};

	struct MegaChunck
	{
		std::vector<Stage> stageList;
	};

	struct ManagmentText
	{
		std::vector<const char*> hallRoomList;
		std::vector<const char*> hospitalRoomList;
		std::vector<const char*> fieldsRoomList;
		std::vector<const char*> laboRoomList;

		std::vector<const char*> hallCellList;
		std::vector<const char*> hospitalCellList;
		std::vector<const char*> fieldsCellList;
		std::vector<const char*> laboCellList;

		std::vector<const char*> exitRoomList;
		std::vector<const char*> propsBuildingRoomList;
	};

	void Generate();
	void Create();
}

#endif // !OBSTACLE_H