#include "Maze.h"
#include "Set.h"
#include "Props.h"
#include "Clock.h"

void Maze::InitCell(Map::Stage& _stage, int _stageNb, int _mapW, int _nbStage)
{
	for (int mapLine = 0; mapLine < _mapW; mapLine++)
	{
		for (int mapColumn = 0; mapColumn < _mapW; mapColumn++)
		{
			int nbCell = _stage.chunckList[mapColumn + mapLine * _mapW].size;
			float w = _stage.chunckList[mapColumn + mapLine * _mapW].w;
			Map::ManagmentText txt = _stage.chunckList[mapColumn + mapLine * _mapW].txt;

			/////////////////////////////////////////////////////// INIT GROUND //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			for (int line = 0; line < nbCell; line++)
			{
				for (int column = 0; column < nbCell; column++)
				{
					Map::Cell cell;
					Map::Decor ground;

					ground.pos = (glm::vec3(w * column + /*BRICK_W +*/ mapColumn * MAP_W, _stage.sizeOf * CELL_W /* - (BRICK_W / 3)*/, w * line - BRICK_W / 2 + mapLine * MAP_W/* + (OFFSET_STAGE * MAP_W * _stageNb)*/));
					ground.isVisible = true;
					ground.name = txt.ground[_stage.chunckList[mapColumn + mapLine * _mapW].type];

					cell.ground.push_back(ground);
					///////////////////////////////////////////////////// INIT ROOF //////////////////////////////////////////////////////////////////////////////////////////////////////////////
					Map::Decor roof;

					roof.pos = (glm::vec3(w * column + mapColumn * MAP_W, _stage.sizeOf * CELL_W + BRICK_W - BRICK_W / 4/* - CELL_W*/, w * line - BRICK_W / 2 + mapLine * MAP_W + BRICK_W));
					roof.isVisible = true;
					roof.name = txt.roof[_stage.chunckList[mapColumn + mapLine * _mapW].type];
					cell.ground.push_back(roof);

					for (int i = 0; i < 2; i++)
					{
						Map::Decor wall;
						//Map::Decor wallOut;

						if (i == BOTTOM) //BOTTOM WALL
						{
							wall.pos = (glm::vec3(w * column + /*BRICK_W / 2 +*/ mapColumn * MAP_W, /*BRICK_W +*/ _stage.sizeOf * CELL_W, w * line + /*BRICK_W + BRICK_W +*/ w + mapLine * MAP_W/* + (OFFSET_STAGE * MAP_W * _stageNb)*/));
							wall.name = txt.wall[_stage.chunckList[mapColumn + mapLine * _mapW].type];/*"../Sets/HOSPITAL/Map/HSP_Wall2.json"*/;
							wall.rot = glm::vec3(0, 0, 0);
							wall.isVisible = true;

							//wallOut.pos = (glm::vec3(CELL_W * column + /*BRICK_W / 2 +*/ mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W + BRICK_W +*/ CELL_W + mapLine * MAP_W /*+ (OFFSET_STAGE * MAP_W * _stageNb)*/ + BRICK_W / 2));
							//wallOut.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
							//wallOut.isVisible = true;
						}
						else if (i == LEFT) //LEFT WALL
						{
							wall.pos = (glm::vec3(w * column - BRICK_W / 2.f + w + BRICK_W + mapColumn * MAP_W, /*BRICK_W +*/ _stage.sizeOf * CELL_W, w * line /*- BRICK_W/2*/ - BRICK_W / 2.f + mapLine * MAP_W /*+ (OFFSET_STAGE * MAP_W * _stageNb)*/));
							wall.name = txt.wall[_stage.chunckList[mapColumn + mapLine * _mapW].type]/*"../Sets/WallRot.json"*/;
							wall.rot = glm::vec3(0, -90, 0);
							wall.isVisible = true;

							//wallOut.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f + mapColumn * MAP_W + BRICK_W / 2, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line /*- BRICK_W/2*/ - BRICK_W / 2.f + mapLine * MAP_W/* + (OFFSET_STAGE * MAP_W * _stageNb)*/));
							//wallOut.name = "../Sets/EXTERIOR/Ext_Brd_1.json";
							//wallOut.isVisible = true;
						}
						cell.wallList.push_back(wall);
						//cell.wallOutList.push_back(wallOut);
					}

					/////////////////////////////////////////////////////// INIT WALL BORDER /////////////////////////////////////////////////////////////////////////////////////////////////////
					Map::Decor wallMissing;
					//Map::Decor wallOutMissing;
					//for (int hight = 0; hight < _stage.sizeOf; hight++)
					//{
					if (mapLine == 0 && line == 0) //BOTTOM WALL
					{
						wallMissing.pos = (glm::vec3(w * column /*+ BRICK_W / 2*/ + mapColumn * MAP_W, /*BRICK_W +*/ _stage.sizeOf * CELL_W /*+ hight * CELL_W*/, w * line + /*BRICK_W * 2 +*/ mapLine * MAP_W /*+ (OFFSET_STAGE * MAP_W * _stageNb)*/));
						wallMissing.isVisible = true;
						wallMissing.name = txt.wallWindow[_stage.chunckList[mapColumn + mapLine * _mapW].type]/*"../Sets/WallWindow.json"*/;
						wallMissing.rot = glm::vec3(0, 0, 0);

						cell.wallMissingList.push_back(wallMissing);

						//wallOutMissing.pos = (glm::vec3(CELL_W * column /*+ BRICK_W / 2*/ + mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W * 2 +*/ mapLine * MAP_W/* + (OFFSET_STAGE * MAP_W * _stageNb)*/ - BRICK_W / 2));
						//wallOutMissing.isVisible = true;
						//wallOutMissing.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
						//cell.wallOutList.push_back(wallOutMissing);
					}
					if (mapColumn == 0 && column == 0) //LEFT WALL
					{
						wallMissing.pos = (glm::vec3(w * column + BRICK_W / 2.f /*+ BRICK_W / 2*/ + mapColumn * MAP_W, /*BRICK_W +*/_stage.sizeOf * CELL_W /*+ hight * CELL_W*/, w * line - /*BRICK_W +*/BRICK_W / 2.f + mapLine * MAP_W/* + (OFFSET_STAGE * MAP_W * _stageNb)*/));
						wallMissing.isVisible = true;
						//wallMissing.name = _text.wallLeftWindow[_stage.chunckList[mapColumn + mapLine * _mapW].type]/*"../Sets/WallRotWindow.json"*/;
						wallMissing.name = txt.wallWindow[_stage.chunckList[mapColumn + mapLine * _mapW].type]/*"../Sets/WallRotWindow.json"*/;
						wallMissing.rot = glm::vec3(0, -90, 0);
						cell.wallMissingList.push_back(wallMissing);

						//wallOutMissing.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f /*+ BRICK_W / 2*/ - CELL_W + mapColumn * MAP_W - BRICK_W / 2, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line - /*BRICK_W +*/BRICK_W / 2.f + mapLine * MAP_W /*+ (OFFSET_STAGE * MAP_W * _stageNb)*/));
						//wallOutMissing.isVisible = true;
						//wallOutMissing.name = "../Sets/EXTERIOR/Ext_Brd_1.json";
						//cell.wallOutList.push_back(wallOutMissing);
					}
					//}
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

					cell.isVisited = false;
					_stage.chunckList[mapColumn + mapLine * _mapW].cellList.push_back(cell);
				}
			}
		}
	}
}

void Maze::InitLaby(Map::Stage& _stage, int _mapW)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::LABY)
		{
			int i = 0;
			int iTemp = i;
			int randTemp = 0;
			int wallEraseCount = 0;
			std::vector<int> temp;
			_stage.chunckList[map].cellList[i].isVisited = true;

			while (wallEraseCount < nbCell * nbCell - 1)
			{
				if (_stage.chunckList[map].cellList[i].isVisited)
				{
					if (i - nbCell >= 0)
					{
						if (!_stage.chunckList[map].cellList[i - nbCell].isVisited)
						{
							temp.push_back(UP);
						}
					}

					if (i + nbCell < nbCell * nbCell)
					{
						if (!_stage.chunckList[map].cellList[i + nbCell].isVisited)
						{
							temp.push_back(BOTTOM);
						}
					}

					if (i + 1 < nbCell * nbCell && (i + 1) % nbCell != 0)
					{
						if (!_stage.chunckList[map].cellList[i + 1].isVisited)
						{
							temp.push_back(LEFT);
						}
					}

					if (i - 1 >= 0 && (i % nbCell) != 0 || i == nbCell * nbCell)
					{
						if (!_stage.chunckList[map].cellList[i - 1].isVisited)
						{
							temp.push_back(RIGHT);
						}
					}

					if (temp.size() != 0)
					{
						iTemp = i;
						randTemp = rand() % temp.size();
						if (temp[randTemp] == BOTTOM)
						{
							_stage.chunckList[map].cellList[i + nbCell].isVisited = true;
							_stage.chunckList[map].cellList[i].wallList[BOTTOM].isVisible = false;
							i = i + nbCell;
							_stage.chunckList[map].cellList[i].lastCase = -nbCell;
						}
						if (temp[randTemp] == LEFT)
						{
							_stage.chunckList[map].cellList[i + 1].isVisited = true;
							_stage.chunckList[map].cellList[i].wallList[LEFT].isVisible = false;
							i = i + 1;
							_stage.chunckList[map].cellList[i].lastCase = -1;
						}
						if (temp[randTemp] == RIGHT)
						{
							_stage.chunckList[map].cellList[i - 1].isVisited = true;
							_stage.chunckList[map].cellList[i - 1].wallList[LEFT].isVisible = false;
							i = i - 1;
							_stage.chunckList[map].cellList[i].lastCase = 1;
						}
						if (temp[randTemp] == UP)
						{
							_stage.chunckList[map].cellList[i - nbCell].isVisited = true;
							_stage.chunckList[map].cellList[i - nbCell].wallList[BOTTOM].isVisible = false;
							i = i - nbCell;
							_stage.chunckList[map].cellList[i].lastCase = nbCell;
						}
						wallEraseCount++;
					}
					else
					{
						i += _stage.chunckList[map].cellList[i].lastCase;
					}
					temp.clear();
				}
			}
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			//                                                                     DENSITY 
			////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

			for (int id = 0; id < _stage.chunckList[map].cellList.size(); id++)
			{
				_stage.chunckList[map].cellList[id].ground[1].isVisible = false;

				for (int shortcut = 0; shortcut < _stage.chunckList[map].cellList[id].wallList.size(); shortcut++)
				{
					int randomLuck = rand() % 100;
					//if (randomLuck > SHORTCUT_LUCK)
					//{
					//	_stage.chunckList[map].cellList[id].wallList[shortcut].isVisible = false;
					//}
					if (randomLuck > SHORTCUT_LUCK)
					{
						if ((id + 1) % nbCell == 0 /*|| id > NB_CELL*NB_CELL - NB_CELL - 1*/)
						{
							_stage.chunckList[map].cellList[id].wallList[LEFT].isVisible = false;
						}
						if (id >= nbCell * nbCell - nbCell /*|| id > NB_CELL*NB_CELL - NB_CELL - 1*/)
						{
							_stage.chunckList[map].cellList[id].wallList[BOTTOM].isVisible = false;
						}
					}
				}

				if ((map + 1) % _mapW == 0 && (id + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[id].wallList[LEFT].isVisible = true;
					//_stage.chunckList[map].cellList[id].wallList[LEFT].name = _text.wallLeftWindow[_stage.chunckList[map].type];
					_stage.chunckList[map].cellList[id].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				if (map >= _mapW * _mapW - _mapW && id >= nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[id].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[id].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}
			}
		}
	}
}

void Maze::InitEmpty(Map::Stage& _stage, int _mapW)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		for (int id = 0; id < _stage.chunckList[map].cellList.size(); id++)
		{		
			for (int wall = 0; wall < _stage.chunckList[map].cellList[id].wallList.size(); wall++)
			{
				_stage.chunckList[map].cellList[id].wallList[wall].isVisible = false;
			}
			if ((map + 1) % _mapW == 0 && (id + 1) % nbCell == 0)
			{
				_stage.chunckList[map].cellList[id].wallList[LEFT].isVisible = true;
				//_stage.chunckList[map].cellList[id].wallList[LEFT].name = _text.wallLeftWindow[_stage.chunckList[map].type];
				_stage.chunckList[map].cellList[id].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
			}

			if (map >= _mapW * _mapW - _mapW && id >= nbCell * nbCell - nbCell)
			{
				_stage.chunckList[map].cellList[id].wallList[BOTTOM].isVisible = true;
				_stage.chunckList[map].cellList[id].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
			}
		}
	}
}

void Maze::InitGarden(Map::Stage& _stage, int _mapW, int _stageNb)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::GARDEN)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				_stage.chunckList[map].cellList[cell].ground[1].isVisible = false;
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallMissingList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallMissingList[wall].isVisible = false;
				}
				if ((cell + 1) % nbCell != 0 || (map + 1) % _mapW == 0 && (cell + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = false;
				}
				else
				{
					//_stage.chunckList[map].cellList[cell].wallList[LEFT].name = _text.wallLeftWindow[_stage.chunckList[map].type];
					_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}
				if (cell < nbCell * nbCell - nbCell || map > _mapW * _mapW - _mapW && cell > nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = false;
				}
				else
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}
				//_stage.chunckList[map].cellList[cell].ground[1].isVisible = false;
				if (_stageNb > 0)
				{
					_stage.chunckList[map].cellList[cell].ground[0].isVisible = false;
				}

				if (map > 0)
				{
					if (_stage.chunckList[map - 1].type != Map::GARDEN && map % _mapW != 0) // RIGHT BORDER
					{
						if ((cell + 1) % nbCell == 0 || cell == nbCell * nbCell - 1)
						{
							_stage.chunckList[map - 1].cellList[cell].wallList[LEFT].isVisible = true;
							//_stage.chunckList[map].cellList[cell].wallOutList[LEFT].isVisible = true;
							//_stage.chunckList[map].cellList[cell].wallOutList[LEFT].pos.x -= BRICK_W;
							_stage.chunckList[map - 1].cellList[cell].wallList[LEFT].name = txt.garden[_stageNb]/*text.wallLeftWindow[maze[mapNb - 1].stageList[stageNb].chunk.type]*/;
							//_stage.chunckList[map - 1].cellList[cell].wallList[LEFT].name = _text.gardenRot[_stageNb]/*text.wallLeftWindow[maze[mapNb - 1].stageList[stageNb].chunk.type]*/;
						}
					}
				}
				if (map - _mapW >= 0)
				{
					if (_stage.chunckList[map - _mapW].type != Map::GARDEN) // TOP BORDER
					{
						if (cell >= nbCell * nbCell - nbCell)
						{
							_stage.chunckList[map - _mapW].cellList[cell].wallList[BOTTOM].isVisible = true;
							//_stage.chunckList[map].cellList[cell].wallOutList[BOTTOM].isVisible = true;
							//_stage.chunckList[map].cellList[cell].wallOutList[BOTTOM].pos.z -= BRICK_W;
							_stage.chunckList[map - _mapW].cellList[cell].wallList[BOTTOM].name = txt.garden[_stageNb]/*text.wallBotWindow[maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type]*//*"../Sets/WallWindow.json"*/;
						}
					}
				}
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].name = txt.garden[_stage.type];
				}
			}
		}
	}
}

void Maze::InitExit(Map::Stage& _stage, int _mapW, int _stageNb) // !!!!!!!!! EXIT
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::EXIT || _stage.chunckList[map].type == Map::ENTRANCE)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				if (_stage.type == Map::LABO)
				{
					_stage.chunckList[map].cellList[cell].ground[1].isVisible = false;
				}
				_stage.chunckList[map].cellList[cell].ground[0].isVisible = false;

				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallMissingList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallMissingList[wall].isVisible = false;
				}
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if (_stage.chunckList[map].type == Map::ENTRANCE) //!!!!!!
				{
					_stage.chunckList[map].cellList[cell].ground[0].isVisible = false;
				}

				if (map > 0)
				{
					if (map % _mapW != 0) // RIGHT BORDER
					{
						if ((cell + 1) % nbCell == 0 || cell == nbCell * nbCell - 1)
						{
							_stage.chunckList[map - 1].cellList[cell].wallList[LEFT].isVisible = false;
						}
					}
				}
				if (map - _mapW >= 0)
				{
					if (cell >= nbCell * nbCell - nbCell)
					{
						_stage.chunckList[map - _mapW].cellList[cell].wallList[BOTTOM].isVisible = false;
					}
				}
			}
			_stage.chunckList[map].decor = Props::GenerateRoom(_stage.chunckList[map]);
		}
	}
}

void Maze::InitRoom(Map::Stage& _stage, int _mapW, int _stageNb) // !!!!!!!!! ROOM
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{
		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::ROOM)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				if (_stage.type == Map::LABO)
				{
					_stage.chunckList[map].cellList[cell].ground[1].isVisible = false;
				}
				//_stage.chunckList[map].cellList[cell].ground[1].isVisible = false;
				_stage.chunckList[map].cellList[cell].ground[0].isVisible = false;
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallMissingList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallMissingList[wall].isVisible = false;
				}
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if (map > 0)
				{
					if (map % _mapW != 0) // RIGHT BORDER
					{
						if ((cell + 1) % nbCell == 0 || cell == nbCell * nbCell - 1)
						{
							_stage.chunckList[map - 1].cellList[cell].wallList[LEFT].isVisible = false;
						}
					}
				}
				if (map - _mapW >= 0)
				{
					if (cell >= nbCell * nbCell - nbCell)
					{
						_stage.chunckList[map - _mapW].cellList[cell].wallList[BOTTOM].isVisible = false;
					}
				}
			}
			_stage.chunckList[map].decor = Props::GenerateRoom(_stage.chunckList[map]);
		}
	}
}

void Maze::InitPlace(Map::Stage& _stage, int _mapW, int _stageNb) // !!!!!!!!! PILLAR
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::PLACE)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if ((map + 1) % _mapW == 0 && (cell + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				if (map >= _mapW * _mapW - _mapW && cell >= nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				_stage.chunckList[map].cellList[cell].ground[1].isVisible = true;
				_stage.chunckList[map].cellList[cell].type = CORRIDOR;
			}
		}
	}
}

void Maze::InitLibrary(Map::Stage& _stage, int _mapW, int _stageNb)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{
		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;
		float w = _stage.chunckList[map].w;

		int randOpenChunk = rand() % 100;

		if (_stage.chunckList[map].type == Map::LIBRARIES)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				Map::Decor scdFloor;

				scdFloor.pos = _stage.chunckList[map].cellList[cell].ground[0].pos;
				scdFloor.pos.y += CELL_W * (SIZE_LIBRARY / 2);
				scdFloor.pos.x += CELL_W / 2;
				scdFloor.pos.z += CELL_W / 2;
				scdFloor.isVisible = false;
				scdFloor.name = txt.ground[_stage.chunckList[map].type];


				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if (cell == 0 || cell == nbCell * nbCell - nbCell * 2 || cell < nbCell * nbCell - nbCell - 1 && cell >= nbCell * nbCell - nbCell * 2 || (cell + 2) % nbCell == 0 && cell < nbCell * nbCell - nbCell)
				{
					scdFloor.isVisible = true;
				}

				if (cell < nbCell && cell != 0 && cell != nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell + nbCell].wallList[LEFT].isVisible = true;
					scdFloor.isVisible = true;
				}
				if (cell % nbCell == 0 && cell != 0 && cell != nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
					_stage.chunckList[map].cellList[cell + 1].wallList[BOTTOM].isVisible = true;

					scdFloor.isVisible = true;
				}
				if (cell > nbCell * nbCell - nbCell && cell != nbCell * nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell - nbCell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell - nbCell].wallList[LEFT].isVisible = true;
				}
				if ((cell + 1) % nbCell == 0 && cell != nbCell - 1 && cell != nbCell * nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell - 1].wallList[LEFT].isVisible = true;
					_stage.chunckList[map].cellList[cell - 1].wallList[BOTTOM].isVisible = true;
				}

				if ((map + 1) % _mapW == 0 && (cell + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				if (map >= _mapW * _mapW - _mapW && cell >= nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				_stage.chunckList[map].cellList[cell].ground.push_back(scdFloor);
			}
			_stage.chunckList[map].cellList[nbCell * nbCell - (nbCell / 2) - nbCell - 1].wallList[BOTTOM].isVisible = false;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 + 1].wallList[LEFT].isVisible = false;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - 2].wallList[LEFT].isVisible = false;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - 1].wallList[BOTTOM].isVisible = true;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - 1 - nbCell].wallList[BOTTOM].isVisible = true;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - 1 - nbCell].wallList[LEFT].isVisible = true;
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - nbCell].wallList[LEFT].isVisible = true;
			_stage.chunckList[map].cellList[nbCell / 2].wallList[BOTTOM].isVisible = false;
		}
	}
}

void Maze::InitLibraryStairs(Map::Stage& _stage, int _mapW, int _stageNb)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{
		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;
		float w = _stage.chunckList[map].w;

		int randOpenChunk = rand() % 100;

		if (_stage.chunckList[map].type == Map::LIBRARYSTAIRS)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				Map::Decor scdFloor;

				scdFloor.pos = _stage.chunckList[map].cellList[cell].ground[0].pos;
				scdFloor.pos.y += CELL_W * (SIZE_LIBRARY / 2);
				scdFloor.pos.x += CELL_W / 2;
				scdFloor.pos.z += CELL_W / 2;
				scdFloor.isVisible = false;
				scdFloor.name = txt.ground[_stage.chunckList[map].type];


				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if (cell == 0 || cell == nbCell * nbCell - nbCell * 2 || cell < nbCell * nbCell - nbCell - 1 && cell >= nbCell * nbCell - nbCell * 2 || (cell + 2) % nbCell == 0 && cell < nbCell * nbCell - nbCell)
				{
					scdFloor.isVisible = true;
				}

				if (cell < nbCell && cell != 0 && cell != nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					scdFloor.isVisible = true;
				}
				if (cell % nbCell == 0 && cell != 0 && cell != nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;

					scdFloor.isVisible = true;
				}
				if (cell > nbCell * nbCell - nbCell && cell != nbCell * nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell - nbCell].wallList[BOTTOM].isVisible = true;
				}
				if ((cell + 1) % nbCell == 0 && cell != nbCell - 1 && cell != nbCell * nbCell - 1)
				{
					_stage.chunckList[map].cellList[cell - 1].wallList[LEFT].isVisible = true;
				}

				if ((map + 1) % _mapW == 0 && (cell + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				if (map >= _mapW * _mapW - _mapW && cell >= nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				_stage.chunckList[map].cellList[cell].ground.push_back(scdFloor);
			}
		}
		int randomDoor = rand() % 4;
		if (randomDoor == BOTTOM)
		{
			_stage.chunckList[map].cellList[nbCell * nbCell - (nbCell / 2) - nbCell - 1].wallList[BOTTOM].isVisible = false;
		}
		else if (randomDoor == LEFT)
		{
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 + 1].wallList[LEFT].isVisible = false;
		}
		else if (randomDoor == RIGHT)
		{
			_stage.chunckList[map].cellList[(nbCell * nbCell) / 2 - 2].wallList[LEFT].isVisible = false;
		}
		else if (randomDoor == UP)
		{
			_stage.chunckList[map].cellList[nbCell / 2].wallList[BOTTOM].isVisible = false;
		}
	}
}

void Maze::InitHospital(Map::Stage& _stage, int _mapW, int _stageNb)
{
	for (int map = 0; map < _mapW * _mapW; map++)
	{

		int nbCell = _stage.chunckList[map].size;
		Map::ManagmentText txt = _stage.chunckList[map].txt;

		if (_stage.chunckList[map].type == Map::HOSPITAL)
		{
			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				_stage.chunckList[map].cellList[cell].type = CORRIDOR;
			}

			for (int nbBedroom = 0; nbBedroom < 3; nbBedroom++)
			{
				int randBedroomLine;
				int randBedroomColumn;
				int randExpLine;
				int randExpColumn;

				randBedroomLine = rand() % (nbCell - 1);
				randBedroomColumn = rand() % (nbCell - 1);
				randExpLine = (rand() % nbCell / 2) + 2;
				randExpColumn = (rand() % nbCell / 2) + 2;

				_stage.chunckList[map].cellList[randBedroomColumn + randBedroomLine * nbCell].type = BEDROOM;

				for (int line = 0; line < nbCell; line++)
				{
					for (int column = 0; column < nbCell; column++)
					{
						if (line >= randBedroomLine && line <= randBedroomLine + randExpLine && column >= randBedroomColumn && column <= randExpColumn)
						{
							_stage.chunckList[map].cellList[column + line * nbCell].type = BEDROOM;
						}
					}
				}
			}

			for (int cell = 0; cell < nbCell * nbCell; cell++)
			{
				int randDoor;

				for (int wall = 0; wall < _stage.chunckList[map].cellList[cell].wallList.size(); wall++)
				{
					_stage.chunckList[map].cellList[cell].wallList[wall].isVisible = false;
				}

				if (_stage.chunckList[map].cellList[cell].type == BEDROOM)
				{
					////////////////////////////////////////////////////////////////////////////////////////////////////
					if (map > 0 && map % _mapW != 0)
					{
						randDoor = rand() % 100;
						if (nbCell < _stage.chunckList[map - 1].size)
						{
							if (cell % nbCell == 0)
							{
								_stage.chunckList[map - 1].cellList[cell * 4 + _stage.chunckList[map - 1].size - 1].wallList[LEFT].isVisible = true;
								_stage.chunckList[map - 1].cellList[cell * 4 + _stage.chunckList[map - 1].size - 1 + _stage.chunckList[map - 1].size].wallList[LEFT].isVisible = true;

								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - 1].cellList[cell + _stage.chunckList[map - 1].size - 2].wallList[LEFT].name != _stage.chunckList[map - 1].txt.door[_stage.chunckList[map].type])
								{
									_stage.chunckList[map - 1].cellList[cell + _stage.chunckList[map - 1].size - 1].wallList[LEFT].name = _stage.chunckList[map - 1].txt.door[_stage.chunckList[map].type];
								}
							}
						}

						if (nbCell > _stage.chunckList[map - 1].size)
						{
							if (cell % nbCell == 0)
							{
								_stage.chunckList[map - 1].cellList[cell / 4 + _stage.chunckList[map - 1].size - 3].wallList[LEFT].isVisible = true;
								//std::cout << cell / 4 + _stage.chunckList[map - 1].size - 3 << std::endl;

								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - 1].cellList[cell / 4 + _stage.chunckList[map - 1].size - 4].wallList[LEFT].name != _stage.chunckList[map - 1].txt.door[_stage.chunckList[map - 1].type])
								{
									_stage.chunckList[map - 1].cellList[cell / 4 + _stage.chunckList[map - 1].size - 3].wallList[LEFT].name = _stage.chunckList[map - 1].txt.door[_stage.chunckList[map - 1].type];
								}
							}
						}

						if (nbCell == _stage.chunckList[map - 1].size)
						{
							if (cell % nbCell == 0)
							{
								_stage.chunckList[map - 1].cellList[cell + nbCell - 1].wallList[LEFT].isVisible = true;

								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - 1].cellList[cell + nbCell - 2].wallList[LEFT].name != txt.door[_stage.chunckList[map].type])
								{
									_stage.chunckList[map - 1].cellList[cell + nbCell - 1].wallList[LEFT].name = txt.door[_stage.chunckList[map].type];
								}
							}
						}
					}

					if (cell % nbCell != 0 && _stage.chunckList[map].cellList[cell - 1].type == CORRIDOR)
					{
						randDoor = rand() % 100;

						_stage.chunckList[map].cellList[cell - 1].wallList[LEFT].isVisible = true;

						if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL)/* && _stage.chunckList[map].cellList[cell - 2].wallList[LEFT].name != txt.door[_stage.chunckList[map].type]*/)
						{
							_stage.chunckList[map].cellList[cell - 1].wallList[LEFT].name = txt.door[_stage.chunckList[map].type];
						}
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////
					if ((cell + 1) % nbCell == 0)
					{
						randDoor = rand() % 100;

						_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;

						if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map].cellList[cell - 1].wallList[LEFT].name != txt.door[_stage.chunckList[map].type])
						{
							_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.door[_stage.chunckList[map].type];
						}
					}

					if (cell + 1 < nbCell * nbCell)
					{
						randDoor = rand() % 100;

						if (/*(cell + 1) % _mapW != 0 && */_stage.chunckList[map].cellList[cell + 1].type == CORRIDOR)
						{
							_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
						}

						if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) /*&& _stage.chunckList[map].cellList[cell - 1].wallList[LEFT].name != txt.door[_stage.chunckList[map].type]*/)
						{
							_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.door[_stage.chunckList[map].type];
						}
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////
					if (cell >= nbCell * nbCell - nbCell)
					{
						randDoor = rand() % 100;

						_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;

						if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map].cellList[cell - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map].type])
						{
							_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.door[_stage.chunckList[map].type];
						}
					}

					if (cell + nbCell < nbCell * nbCell)
					{
						if (_stage.chunckList[map].cellList[cell + nbCell].type == CORRIDOR)
						{
							randDoor = rand() % 100;

							_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
							if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map].cellList[cell - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map].type])
							{
								_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.door[_stage.chunckList[map].type];
							}
						}
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////
					if (map - _mapW >= 0)
					{
						if (cell < nbCell)
						{
							randDoor = rand() % 100;
							if (nbCell < _stage.chunckList[map - _mapW].size)
							{
								_stage.chunckList[map - _mapW].cellList[cell * 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size].wallList[BOTTOM].isVisible = true;
								_stage.chunckList[map - _mapW].cellList[cell * 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size + 1].wallList[BOTTOM].isVisible = true;
								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - _mapW].cellList[cell * 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map - _mapW].type])
								{
									_stage.chunckList[map - _mapW].cellList[cell * 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size].wallList[BOTTOM].name = txt.door[_stage.chunckList[map - _mapW].type];
								}
							}

							if (nbCell > _stage.chunckList[map - _mapW].size)
							{
								_stage.chunckList[map - _mapW].cellList[cell / 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size].wallList[BOTTOM].isVisible = true;
								//_stage.chunckList[map - _mapW].cellList[cell + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size].wallList[BOTTOM].isVisible = true;
								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - _mapW].cellList[cell / 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map - _mapW].type])
								{
									_stage.chunckList[map - _mapW].cellList[cell / 2 + _stage.chunckList[map - _mapW].size * _stage.chunckList[map - _mapW].size - _stage.chunckList[map - _mapW].size].wallList[BOTTOM].name = txt.door[_stage.chunckList[map - _mapW].type];
								}
							}

							if (nbCell == _stage.chunckList[map - _mapW].size)
							{
								_stage.chunckList[map - _mapW].cellList[cell + nbCell * nbCell - nbCell].wallList[BOTTOM].isVisible = true;
								if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) && _stage.chunckList[map - _mapW].cellList[cell + nbCell * nbCell - nbCell - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map].type])
								{
									_stage.chunckList[map - _mapW].cellList[cell + nbCell * nbCell - nbCell].wallList[BOTTOM].name = txt.door[_stage.chunckList[map].type];
								}
							}
						}
					}

					if (cell >= nbCell && _stage.chunckList[map].cellList[cell - nbCell].type == CORRIDOR)
					{
						randDoor = rand() % 100;

						_stage.chunckList[map].cellList[cell - nbCell].wallList[BOTTOM].isVisible = true;
						if (randDoor <= DOOR_LUCK / (nbCell / NB_CELL) /*&& _stage.chunckList[map].cellList[cell - nbCell - 1].wallList[BOTTOM].name != txt.door[_stage.chunckList[map].type]*/)
						{
							_stage.chunckList[map].cellList[cell - nbCell].wallList[BOTTOM].name = txt.door[_stage.chunckList[map].type];
						}
					}
					////////////////////////////////////////////////////////////////////////////////////////////////////
					_stage.chunckList[map].cellList[cell].ground[1].isVisible = true;//!!!!!!!!!!!!!!!!!!!! test

					if (_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible && _stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible)
					{
						_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wall[_stage.chunckList[map].type];
						_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wall[_stage.chunckList[map].type];
					}
				}

				if ((map + 1) % _mapW == 0 && (cell + 1) % nbCell == 0)
				{
					_stage.chunckList[map].cellList[cell].wallList[LEFT].isVisible = true;
					//_stage.chunckList[map].cellList[cell].wallList[LEFT].name = _text.wallLeftWindow[_stage.chunckList[map].type];
					_stage.chunckList[map].cellList[cell].wallList[LEFT].name = txt.wallWindow[_stage.chunckList[map].type];
				}

				if (map >= _mapW * _mapW - _mapW && cell >= nbCell * nbCell - nbCell)
				{
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].isVisible = true;
					_stage.chunckList[map].cellList[cell].wallList[BOTTOM].name = txt.wallWindow[_stage.chunckList[map].type];
				}
			}

		}
	}
	//for (int map = 0; map < _mapW * _mapW; map++)
	//{

	//	int nbCell = _stage.chunckList[map].size;
	//	if (_stage.chunckList[map].type == Map::HOSPITAL)
	//	{
	//		for (int cell = 0; cell < nbCell * nbCell; cell++)
	//		{
	//			_stage.chunckList[map].cellList[cell].props = Props::Generate(_stage.chunckList[map].cellList[cell]);
	//		}
	//	}

	//	int randomColumnProps = 0;
	//	randomColumnProps = rand() % 50;
	//	for (int cell = 0; cell < nbCell * nbCell; cell++)
	//	{
	//		if (_stage.chunckList[map].type == Map::PLACE)
	//		{
	//			_stage.chunckList[map].cellList[cell].props = Props::GeneratePropsForCell(_stage.chunckList[map].cellList[cell], randomColumnProps);
	//		}
	//	}
	//}
}

void Maze::InitProps(Map::Stage& _stage, int _mapW, int _stageNb)
{
	for (int map = 0; map < _stage.chunckList.size(); map++)
	{
		Props::GenerateProps(_stage.chunckList[map]);
	}
}