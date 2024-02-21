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
#define NB_STAGE_H 8
#define OFFSET_STAGE 1
//////////////////////////////////////// MAP PRESET ////////////////////////////////////////
#define NB_MAP_H 4
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
		FACTORY,
		LABO,
		FIELDS,
		HOSPITALS,
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
		//Scene::Component* decor;
		Sets::Set* decor;
		bool isVisible;
	};

	struct Cell
	{
		//Scene::Component* ground;
		//Scene::Component* roof;
		//Sets::Set* ground;
		//Sets::Set* roof;

		bool isVisited;
		int lastCase;
		std::vector<Decor> wallMissingList;
		std::vector<Decor> ground;
		std::vector<Decor> wallList;
	};

	struct Chunck
	{
		std::vector<Cell> cellList;
		//Scene::Component* decor;
		//Sets::Set* decor;
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
		std::vector<std::string> hallRoomList;
		std::vector<std::string> hospitalRoomList;
		std::vector<std::string> fieldsRoomList;
		std::vector<std::string> laboRoomList;

		std::vector<std::string> hallCellList;
		std::vector<std::string> hospitalCellList;
		std::vector<std::string> fieldsCellList;
		std::vector<std::string> laboCellList;

		std::vector<std::string> exitRoomList;
		std::vector<std::string> propsBuildingRoomList;
	};

	void Generate();
}

#endif // !OBSTACLE_H