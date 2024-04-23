#ifndef MAPMANAGER_H
#define MAPMANAGER_H

#include "Common.h"
#include "Scene.h"
//#include "Maze.h"
#include "Set.h"
//#include "Props.h"

//////////////////////////////////////// DENSITY ////////////////////////////////////////
#define SIZE_HOSPITALS 1
#define SIZE_LABO 2
#define SIZE_FIELDS 4
#define SIZE_LIBRARY 6

namespace Map
{

	enum ChunckSizeEnum
	{
		NORMAL = 1,
		TINY = 2,
	};

	enum ChunckEnum
	{
		EMPTY,
		LABY,
		GARDEN,
		HOSPITAL,
		EXIT,
		ENTRANCE,
		ROOM,
		PLACE,
		LIBRARYSTAIRS,
		LIBRARIES,
	};

	enum StageEnum
	{
		LABO,
		LIBRARY,
		HOSPITALS,
		FIELDS,
	};

	struct ManagmentTextProps
	{
		std::vector<const char*> room;
		std::vector<const char*> library;
		std::vector<const char*> labo;
		std::vector<const char*> hall;
	};

	struct ManagmentText
	{
		std::vector<const char*> ground;
		std::vector<const char*> wall;
		std::vector<const char*> wallWindow;
		std::vector<const char*> roof;
		std::vector<const char*> garden;
		std::vector<const char*> door;
		std::vector<const char*> room;
	};

	struct Decor
	{
		Sets::Set* decor;
		bool isVisible;
		const char* name;
		glm::vec3 pos;
		glm::vec3 rot;
	};

	struct Cell
	{
		bool isVisited;
		int lastCase;
		int type;
		std::vector<Decor> wallMissingList;
		std::vector<Decor> ground;
		std::vector<Decor> wallList;
		std::vector<Decor> wallOutList;
		std::vector<Decor> props;
		std::vector<Decor> bedroom;
	};

	struct Chunck
	{
		Sets::Set* parentSet = nullptr;
		std::vector<Cell> cellList;
		std::vector<Decor> decor;

		int type;
		float size;
		float w;
		ManagmentText txt;
		std::vector<const char*> txtRoom;
	};

	struct Stage
	{
		std::vector<Chunck> chunckList;
		int type;
		float sizeOf;
		int hightScale;
	};

	ManagmentText InitText(int _size);
	void StageManagment(Stage& stage, int _stageNb, int _mapW, int _nbStage);
	void GenerateMaze(int _mapW, int _nbStage);
	void CreateMaze();
	//void Text();
}
#endif // !OBSTACLE_H


