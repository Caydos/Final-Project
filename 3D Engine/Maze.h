#ifndef MAZE_H
#define MAZE_H

#include "Common.h"
#include "Scene.h"
#include "Set.h"
#include "MapManager.h"

//////////////////////////////////////// CELL PRESET ////////////////////////////////////////
#define NB_CELL 5
#define BRICK_W 0.25
#define NB_CELL_BRICK 10
#define CELL_W ((float)BRICK_W*(float)NB_CELL_BRICK)
//////////////////////////////////////// STAGE PRESET ////////////////////////////////////////
//#define _nbStage 1
#define OFFSET_STAGE 1
//////////////////////////////////////// MAP PRESET ////////////////////////////////////////
//#define _mapW 3
#define MAP_W (CELL_W*NB_CELL)
//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define SHORTCUT_LUCK 75
#define DOOR_LUCK 25

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

	//struct Decor
	//{
	//	Sets::Set* decor;
	//	bool isVisible;
	//	const char* name;
	//	glm::vec3 pos;
	//	glm::vec3 rot;
	//};

	//struct Cell
	//{
	//	bool isVisited;
	//	int lastCase;
	//	std::vector<Decor> wallMissingList;
	//	std::vector<Decor> ground;
	//	std::vector<Decor> wallList;
	//	std::vector<Decor> wallOutList;
	//	std::vector<Decor> props;
	//	std::vector<Decor> bedroom;
	//};

	//struct Chunck
	//{
	//	Sets::Set* parentSet = nullptr;
	//	std::vector<Cell> cellList;
	//	std::vector<Decor> decor;

	//	int type;
	//	float sizeOf;
	//};

	//struct Stage
	//{
	//	Chunck chunk;
	//	int type;
	//	float sizeOf;
	//};

	//struct MegaChunck
	//{
	//	std::vector<Stage> stageList;
	//	//std::vector<Decor> ObjectiveList;
	//};

	//struct ManagmentText
	//{
	//	std::vector<const char*> ground;
	//	std::vector<const char*> wallBot;
	//	std::vector<const char*> wallBotWindow;
	//	std::vector<const char*> wallLeftWindow;
	//	std::vector<const char*> wallLeft;
	//	std::vector<const char*> roof;
	//	std::vector<const char*> propsBuilding;
	//	std::vector<const char*> garden;
	//	std::vector<const char*> gardenRot;
	//};

	//void Generate();
	//void Create();

	//ManagmentText InitProps();
	//void GenerateMaze(int _mapW, int _nbStage);
	//void StageManagment(int _mapColumn, int _mapLine, int _stageNb, Chunck& _chunck, Stage& _stage, int _mapW, int _nbStage);
	void InitCell(Map::Stage& _stage, int _stageNb, int _mapW, int _nbStage);

	void InitLaby(Map::Stage& _stage, int _mapW);
	void InitEmpty(Map::Stage& _stage, int _mapW);
	void InitGarden(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitExit(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitRoom(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitProps(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitPlace(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitLibrary(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitHospital(Map::Stage& _stage, int _mapW, int _stageNb);
	void InitLibraryStairs(Map::Stage& _stage, int _mapW, int _stageNb);
	//void InitHospital(int _mapNb, int _stageNb, int _mapW, int _nbStage);
	//void InitSpecialChuncks(int _mapNb, int _stageNb, int _cell, ManagmentText _text, int _mapW, int _nbStage);
	//void CreateMaze();
}
#endif // !OBSTACLE_H