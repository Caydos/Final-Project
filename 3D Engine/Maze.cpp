#include "Maze.h"
#include "Set.h"
#include "Props.h"
#include "Clock.h"

std::vector<Maze::MegaChunck> maze;

Maze::ManagmentText Maze::InitProps()
{
	ManagmentText text;
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //LABY
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //GARDEN
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //HOSPITAL
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EXIT
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //ROOM
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //PARCEL
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //BALCONY
	text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EMPTY

	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
	text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");

	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
	text.wallBot.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");

	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
	text.wallLeft.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");

	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.wallBotWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");

	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.wallLeftWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");

	text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");

	text.gardenRot.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.gardenRot.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.gardenRot.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");
	text.gardenRot.push_back("../Sets/HOSPITAL/Map/HSP_Window1.json");

	return text;
}

void Maze::StageManagment(int _mapColumn, int _mapLine, int _stageNb, Chunck& _chunck, Stage& _stage, int _mapW, int _nbStage)
{

	int randomBalcony = 0;
	randomBalcony = rand() % 100;
	//_chunck.type = PARCEL;

	if (_stageNb == FACTORY)
	{
		_chunck.type = LABY;
		_stage.sizeOf = CELL_W;
	}
	else if (_stageNb == LABO)
	{
		_chunck.type = LABY;
		_stage.sizeOf = CELL_W;
	}
	else if (_stageNb == FIELDS)
	{
		_chunck.type = LABY;
		_stage.sizeOf = CELL_W;
	}
	else if (_stageNb == HOSPITALS)
	{
		_chunck.type = HOSPITAL;
		_stage.sizeOf = CELL_W;
	}
	if (_stageNb != 0 && _mapLine == 0 && randomBalcony >= BUILDING_LUCK)
	{
		_chunck.type = BALCONY;
	}
	if (_nbStage - _stageNb >= 0 && _mapColumn + _mapLine * _mapW - _mapW >= 0)
	{
		if (maze[_mapColumn + _mapLine * _mapW - _mapW].stageList[_stageNb].chunk.type == BALCONY && _mapLine - 1 == 0 || _mapLine - 1 != 0)
		{
			if (_mapLine == 4 - _stageNb && _mapColumn == 1)
			{
				_chunck.type = GARDEN;
			}
		}
	}
	if (_stageNb == 0 && _mapLine == 3 && _mapColumn == 4)
	{
		_chunck.type = EXIT;
	}
	if (_stageNb == 1 && _mapLine == 4 && _mapColumn == 2)
	{
		_chunck.type = EXIT;
	}
	if (_stageNb == 2 && _mapLine == 3 && _mapColumn == 3)
	{
		_chunck.type = EXIT;
	}


}

void Maze::InitCell(int _mapColumn, int _mapLine, int _stageNb, Chunck& _chunck, ManagmentText _text, int _mapW, int _nbStage)
{
	/////////////////////////////////////////////////////// INIT GROUND //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (int line = 0; line < NB_CELL; line++)
	{
		for (int column = 0; column < NB_CELL; column++)
		{
			Cell cell;
			Decor ground;

			ground.pos = (glm::vec3(CELL_W * column + /*BRICK_W +*/ _mapColumn * MAP_W, 0 + CELL_W * _stageNb - (BRICK_W / 3), CELL_W * line - BRICK_W / 2 + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
			ground.isVisible = true;
			ground.name = _text.ground[_chunck.type]/*"../Sets/Ground.json"*/;

			//cell.props.push_back(Props::Generate(_chunck.type, ground.pos, line, column, Props::BED));

			if (_chunck.type == GARDEN && _stageNb != 0)
			{
				ground.isVisible = false;
				int randomBridge = 0;
				randomBridge = rand() % 100;
				if (randomBridge < 40)
				{
					if (column == NB_CELL - 1)
					{
						ground.isVisible = true;
						if (line == 0)
						{
							Decor balconyBridge;

							balconyBridge.pos = (glm::vec3(CELL_W * column /*+ BRICK_W / 2*/ + _mapColumn * MAP_W - CELL_W, CELL_W + CELL_W * _stageNb - CELL_W /*+ BRICK_W*/, CELL_W * line - BRICK_W / 2 + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
							balconyBridge.name = "../Sets/HOSPITAL/Map/HSF_Wd_2.json";
							balconyBridge.isVisible = false;
							_chunck.decor.push_back(balconyBridge);
						}
					}
				}
			}
			cell.ground.push_back(ground);

			/////////////////////////////////////////////////////// INIT ROOF //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (_chunck.type != GARDEN && _chunck.type != EXIT && _chunck.type != BALCONY)
			{
				Decor roof;

				roof.pos = (glm::vec3(CELL_W * column + /*BRICK_W +*/ _mapColumn * MAP_W, BRICK_W /*/ 2*/ + CELL_W * _stageNb - BRICK_W / 2, CELL_W * line - BRICK_W / 2 + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
				roof.isVisible = true;
				roof.name = _text.roof[_chunck.type]/*"../Sets/Ground.json"*/;
				cell.ground.push_back(roof);
			}

			/////////////////////////////////////////////////////// INIT PROPS BUILDING //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (_chunck.type != GARDEN && _chunck.type != BALCONY)
			{
				if (_mapLine == 0 && _stageNb < _nbStage - 1 || _stageNb == _nbStage - 1)
				{
					if (column == NB_CELL - 1 && line == 0)
					{
						Decor tower;

						tower.name = /*"../Sets/HOSPITAL/Map/HSF_Wd_2.json"*/"../Sets/HOSPITAL/Map//HSF_Wd_2.json";
						tower.pos = (glm::vec3(CELL_W * column + /*BRICK_W +*/ _mapColumn * MAP_W - MAP_W - BRICK_W / 2, CELL_W /*/ 2*/ + CELL_W * _stageNb - BRICK_W / 3, CELL_W * line - BRICK_W / 2 /*+ BRICK_W * 2*/ + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						tower.isVisible = true;
						_chunck.decor.push_back(tower);
					}
				}
			}

			/////////////////////////////////////////////////////// INIT EXIT //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (_chunck.type == EXIT && column == NB_CELL - 1 && line == 0)
			{
				Decor exit;

				exit.pos = (glm::vec3(CELL_W * column + /*BRICK_W / 2 +*/ _mapColumn * MAP_W, CELL_W + CELL_W * _stageNb - CELL_W, CELL_W * line + /*BRICK_W * 2 +*/ _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
				exit.isVisible = true;
				exit.name = "../Sets/HOSPITAL/Map/HSP_Room.json";
				_chunck.decor.push_back(exit);
			}

			/////////////////////////////////////////////////////// INIT BALCONY //////////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (_chunck.type == BALCONY && column == NB_CELL - 1 && line == 0)
			{
				Decor balcony;

				balcony.pos = (glm::vec3(CELL_W * column + /*BRICK_W / 2 +*/ _mapColumn * MAP_W, CELL_W + CELL_W * _stageNb - CELL_W /*+ BRICK_W*/, CELL_W * line + /*BRICK_W * 2 +*/ _mapLine * MAP_W - BRICK_W / 2 + (OFFSET_STAGE * MAP_W * _stageNb)));
				balcony.isVisible = true;
				balcony.name = "../Sets/EXTERIOR/Balcony.json"/*"../Sets/Balcony.json"*/;
				_chunck.decor.push_back(balcony);
			}

			/////////////////////////////////////////////////////// INIT WALL LABY ///////////////////////////////////////////////////////////////////////////////////////////////////////////
			if (_chunck.type == LABY || _chunck.type == GARDEN || _chunck.type == BALCONY || _chunck.type == HOSPITAL || _chunck.type == PARCEL)
			{
				for (int i = 0; i < 2; i++)
				{
					Decor wall;
					Decor wallOut;

					if (i == BOTTOM) //BOTTOM WALL
					{
						wall.pos = (glm::vec3(CELL_W * column + /*BRICK_W / 2 +*/ _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W + BRICK_W +*/ CELL_W + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::DOOR));
						wallOut.pos = (glm::vec3(CELL_W * column + /*BRICK_W / 2 +*/ _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W + BRICK_W +*/ CELL_W + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb) + BRICK_W / 2));

						if (_mapLine == _mapW - 1 && line == NB_CELL - 1)
						{
							wall.name = _text.wallBotWindow[_chunck.type]/*"../Sets/WallWindow.json"*/;
							wallOut.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
						}
						else if (line == (int)(NB_CELL / 2) || line == (int)(NB_CELL / 2) - 1)
						{
							wall.name = _text.wallBot[_chunck.type]/*"../Sets/WallDoor.json"*/;
							wallOut.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
						}
						else
						{
							if (_chunck.type == HOSPITAL)
							{
								wall.name = "../Sets/HOSPITAL/Map/HSF_Wd_2.json";
							}
							wallOut.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
						}
						wall.isVisible = true;
						if (_mapLine == _mapW - 1 && line == NB_CELL - 1)
						{
							if (_chunck.type != BALCONY)
							{
								wallOut.isVisible = true;
							}
						}
						else
						{
							wallOut.isVisible = false;
						}
					}
					else if (i == LEFT) //LEFT WALL
					{
						wall.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f + _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line /*- BRICK_W/2*/ - BRICK_W / 2.f + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::CORRIDOR));
						cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::LIGHT));
						cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::PANELEXIT));
						cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::RAIL));
						cell.bedroom = Props::GenerateBedRoom(_chunck.type, wall.pos, line, column);

						//if (cell.props.size() <= 1)
						//{
						//	cell.props.push_back(Props::Generate(_chunck.type, wall.pos, line, column, Props::BENCH));
						//}
						wallOut.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f + _mapColumn * MAP_W + BRICK_W / 2, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line /*- BRICK_W/2*/ - BRICK_W / 2.f + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						if (_mapColumn == _mapW - 1 && column == NB_CELL - 1)
						{
							wall.name = _text.wallLeftWindow[_chunck.type]/*"../Sets/WallRotWindow.json"*/;
							wallOut.name = "../Sets/EXTERIOR/Ext_Brd_1.json";
						}
						else
						{
							wall.name = _text.wallLeft[_chunck.type]/*"../Sets/WallRot.json"*/;
							wallOut.name = "../Sets/EXTERIOR/Ext_Brd_1.json";
						}
						wall.isVisible = true;
						if (_mapColumn == _mapW - 1 && column == NB_CELL - 1)
						{
							if (_chunck.type != BALCONY)
							{
								wallOut.isVisible = true;
							}
						}
						else
						{
							wallOut.isVisible = false;
						}
					}
					/////////////////////////////////////////////////////// INIT WALL PRESET ///////////////////////////////////////////////////////////////////////////////////////////////////////////
					if (_chunck.type == GARDEN || _chunck.type == BALCONY || _chunck.type == PARCEL) //GARDEN PRESET
					{
						wall.isVisible = false;
					}
					cell.wallList.push_back(wall);
					cell.wallOutList.push_back(wallOut);
				}

				/////////////////////////////////////////////////////// INIT WALL BORDER /////////////////////////////////////////////////////////////////////////////////////////////////////
				if (_chunck.type == LABY || _chunck.type == HOSPITAL || _chunck.type == PARCEL) //LABY BORDER
				{
					Decor wallMissing;
					Decor wallOutMissing;

					if (_mapLine == 0 && line == 0) //BOTTOM WALL
					{
						wallMissing.pos = (glm::vec3(CELL_W * column /*+ BRICK_W / 2*/ + _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W * 2 +*/ _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						wallMissing.isVisible = true;
						wallMissing.name = _text.wallBotWindow[_chunck.type]/*"../Sets/WallWindow.json"*/;
						cell.wallMissingList.push_back(wallMissing);

						wallOutMissing.pos = (glm::vec3(CELL_W * column /*+ BRICK_W / 2*/ + _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line + /*BRICK_W * 2 +*/ _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb) - BRICK_W / 2));
						wallOutMissing.isVisible = true;
						wallOutMissing.name = "../Sets/EXTERIOR/Ext_Brd_2.json";
						cell.wallOutList.push_back(wallOutMissing);
					}
					if (_mapColumn == 0 && column == 0) //LEFT WALL
					{
						wallMissing.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f /*+ BRICK_W / 2*/ - CELL_W + _mapColumn * MAP_W, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line - /*BRICK_W +*/BRICK_W / 2.f + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
						wallMissing.isVisible = true;
						wallMissing.name = _text.wallLeftWindow[_chunck.type]/*"../Sets/WallRotWindow.json"*/;
						cell.wallMissingList.push_back(wallMissing);
						if (_chunck.type != BALCONY)
						{
							wallOutMissing.pos = (glm::vec3(CELL_W * column - BRICK_W / 2.f /*+ BRICK_W / 2*/ - CELL_W + _mapColumn * MAP_W - BRICK_W / 2, /*BRICK_W +*/ CELL_W * _stageNb, CELL_W * line - /*BRICK_W +*/BRICK_W / 2.f + _mapLine * MAP_W + (OFFSET_STAGE * MAP_W * _stageNb)));
							wallOutMissing.isVisible = true;
							wallOutMissing.name = "../Sets/EXTERIOR/Ext_Brd_1.json";
							cell.wallOutList.push_back(wallOutMissing);
						}
					}
				}
				//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

				cell.isVisited = false;


			}
			_chunck.cellList.push_back(cell);
		}
	}
}

void Maze::InitLaby(Chunck& _chunck)
{
	/////////////////////////////////////////////////////// LABY CREATION ///////////////////////////////////////////////////////
	if (_chunck.type == LABY)
	{
		int i = 0;
		int iTemp = i;
		int randTemp = 0;
		int wallEraseCount = 0;
		std::vector<int> temp;
		_chunck.cellList[i].isVisited = true;

		while (wallEraseCount < NB_CELL * NB_CELL - 1)
		{
			if (_chunck.cellList[i].isVisited)
			{
				if (i - NB_CELL >= 0)
				{
					if (!_chunck.cellList[i - NB_CELL].isVisited)
					{
						temp.push_back(UP);
					}
				}

				if (i + NB_CELL < NB_CELL * NB_CELL)
				{
					if (!_chunck.cellList[i + NB_CELL].isVisited)
					{
						temp.push_back(BOTTOM);
					}
				}

				if (i + 1 < NB_CELL * NB_CELL && (i + 1) % NB_CELL != 0)
				{
					if (!_chunck.cellList[i + 1].isVisited)
					{
						temp.push_back(LEFT);
					}
				}

				if (i - 1 >= 0 && (i % NB_CELL) != 0 || i == NB_CELL * NB_CELL)
				{
					if (!_chunck.cellList[i - 1].isVisited)
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
						_chunck.cellList[i + NB_CELL].isVisited = true;
						_chunck.cellList[i].wallList[BOTTOM].isVisible = false;
						i = i + NB_CELL;
						_chunck.cellList[i].lastCase = -NB_CELL;
					}
					if (temp[randTemp] == LEFT)
					{
						_chunck.cellList[i + 1].isVisited = true;
						_chunck.cellList[i].wallList[LEFT].isVisible = false;
						i = i + 1;
						_chunck.cellList[i].lastCase = -1;
					}
					if (temp[randTemp] == RIGHT)
					{
						_chunck.cellList[i - 1].isVisited = true;
						_chunck.cellList[i - 1].wallList[LEFT].isVisible = false;
						i = i - 1;
						_chunck.cellList[i].lastCase = 1;
					}
					if (temp[randTemp] == UP)
					{
						_chunck.cellList[i - NB_CELL].isVisited = true;
						_chunck.cellList[i - NB_CELL].wallList[BOTTOM].isVisible = false;
						i = i - NB_CELL;
						_chunck.cellList[i].lastCase = NB_CELL;
					}
					wallEraseCount++;
				}
				else
				{
					i += _chunck.cellList[i].lastCase;
				}
				temp.clear();
			}
		}
		/////////////////////////////////////////////////////// DENSITY ///////////////////////////////////////////////////////

		for (int id = 0; id < _chunck.cellList.size(); id++)
		{
			for (int shortcut = 0; shortcut < _chunck.cellList[id].wallList.size(); shortcut++)
			{
				int randomLuck = rand() % 100;
				if (randomLuck > SHORTCUT_LUCK)
				{
					//if (id % NB_CELL != 0 || id <= NB_CELL * NB_CELL - NB_CELL - 1)
					//{
					_chunck.cellList[id].wallList[shortcut].isVisible = false;
					//}
				}
				//if (randomLuck < SHORTCUT_LUCK)
				//{
				//	if (id % NB_CELL == 0 || id > NB_CELL*NB_CELL - NB_CELL - 1)
				//	{
				//		chunk.cellList[id].wallList[shortcut].decor->SetVisible(false);
				//	}
				//}
			}
		}
	}
}

void Maze::InitHospital(int _mapNb, int _stageNb, int _mapW, int _nbStage)
{
	if (maze[_mapNb].stageList[_stageNb].chunk.type == HOSPITAL)
	{
		for (int line = 0; line < NB_CELL; line++)
		{
			for (int column = 0; column < NB_CELL; column++)
			{
				if ((_mapNb + 1) % _mapW != 0 && maze[_mapNb].stageList.size() == maze[_mapNb + 1].stageList.size())
				{
					if (maze[_mapNb + 1].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb + 1].stageList[_stageNb].chunk.type == EXIT) // LEFT CORRDIOR ->
					{
						if (line == ((int)(NB_CELL / 2)))
						{
							maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL].wallList[LEFT].isVisible = false;
						}
					}
				}
				if (_mapNb + _mapW < _mapW * _mapW && maze[_mapNb].stageList.size() == maze[_mapNb + _mapW].stageList.size())
				{
					if (maze[_mapNb + _mapW].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb + _mapW].stageList[_stageNb].chunk.type == EXIT) //BOT CORRIDOR ->
					{
						if (column == (int)(NB_CELL / 2))
						{
							maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL].wallList[BOTTOM].isVisible = false;
						}
					}
				}
				if (_mapNb % _mapW != 0 && maze[_mapNb].stageList.size() == maze[_mapNb - 1].stageList.size())
				{
					if (maze[_mapNb - 1].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb - 1].stageList[_stageNb].chunk.type == EXIT) //BOT CORRIDOR <-
					{
						if (line == (int)(NB_CELL / 2))
						{
							maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL].wallList[LEFT].isVisible = false;
						}
					}
				}
				if (_mapNb - _mapW >= 0 && maze[_mapNb].stageList.size() == maze[_mapNb - _mapW].stageList.size())
				{
					if (maze[_mapNb - _mapW].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb - _mapW].stageList[_stageNb].chunk.type == EXIT) // LEFT CORRDIOR <-
					{
						if (column == (int)(NB_CELL / 2))
						{
							maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL].wallList[BOTTOM].isVisible = false;
						}
					}
				}
				for (int wallNb = 0; wallNb < 2; wallNb++)
				{
					if (column + line * NB_CELL == 0 || column == (int)(NB_CELL / 2) + 1 && line == (int)(NB_CELL / 2) + 1 || column == 0 && line == (int)(NB_CELL / 2) + 1 || column == (int)(NB_CELL / 2) + 1 && line == 0)
					{
						maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL].wallList[wallNb].isVisible = false;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL + NB_CELL].wallList[LEFT].isVisible = false;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[column + line * NB_CELL + 1].wallList[BOTTOM].isVisible = false;
					}
				}
			}
		}
	}
}

void Maze::InitSpecialChuncks(int _mapNb, int _stageNb, int _cell, ManagmentText _text, int _mapW, int _nbStage)
{
	///////////////////////////////////////////////////// GARDEN BORDER ///////////////////////////////////////////////////////
	if (maze[_mapNb].stageList[_stageNb].chunk.type == LABY || maze[_mapNb].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb].stageList[_stageNb].chunk.type == PARCEL)
	{
		if (_mapNb + 1 < _mapW * _mapW && maze[_mapNb].stageList.size() == maze[_mapNb + 1].stageList.size())
		{
			if ((_mapNb + 1) % _mapW == 0 || maze[_mapNb + 1].stageList[_stageNb].chunk.type == GARDEN) // RIGHT BORDER
			{
				if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].isVisible = true;
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].name = _text.gardenRot[_stageNb]/*"../Sets/WallRotWindow.json"*/;
				}
			}
		}
		if (_mapNb + _mapW < _mapW * _mapW && maze[_mapNb].stageList.size() == maze[_mapNb + _mapW].stageList.size())
		{
			if (_mapNb > _mapW * _mapW - _mapW - 1 || maze[_mapNb + _mapW].stageList[_stageNb].chunk.type == GARDEN) // TOP BORDER
			{
				if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].isVisible = true;
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].name = _text.garden[_stageNb]/*"../Sets/WallWindow.json"*/;
				}
			}
		}
		if (_mapNb > 0 && maze[_mapNb].stageList.size() == maze[_mapNb - 1].stageList.size())
		{
			if (maze[_mapNb - 1].stageList[_stageNb].chunk.type == GARDEN && _mapNb % _mapW != 0) // RIGHT BORDER
			{
				if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
				{
					maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
					maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].isVisible = true;
					maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].pos.x -= BRICK_W;
					maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].name = _text.gardenRot[_stageNb]/*text.wallLeftWindow[maze[mapNb - 1].stageList[stageNb].chunk.type]*/;
				}
			}
		}
		if (_mapNb - _mapW >= 0 && maze[_mapNb].stageList.size() == maze[_mapNb - _mapW].stageList.size())
		{
			if (maze[_mapNb - _mapW].stageList[_stageNb].chunk.type == GARDEN) // TOP BORDER
			{
				if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
				{
					maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
					maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].isVisible = true;
					maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].pos.z -= BRICK_W;
					maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].name = _text.garden[_stageNb]/*text.wallBotWindow[maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type]*//*"../Sets/WallWindow.json"*/;
				}
			}
		}
		/////////////////////////////////////////////////////// BALCONY BORDER ///////////////////////////////////////////////////////
		if (maze[_mapNb].stageList[_stageNb].chunk.type != BALCONY)
		{
			if (_mapNb + 1 < _mapW * _mapW && maze[_mapNb].stageList.size() == maze[_mapNb + 1].stageList.size())
			{
				if (maze[_mapNb + 1].stageList[_stageNb].chunk.type == BALCONY) // RIGHT BORDER
				{
					if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
					{
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].isVisible = true;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].name = _text.gardenRot[_stageNb]/*"../Sets/WallRotWindow.json"*/;
					}
				}
			}
			if (_mapNb + _mapW < _mapW * _mapW && maze[_mapNb].stageList.size() == maze[_mapNb + _mapW].stageList.size())
			{
				if (maze[_mapNb + _mapW].stageList[_stageNb].chunk.type == BALCONY) // TOP BORDER
				{
					if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
					{
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].isVisible = true;
						maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].name = _text.garden[_stageNb]/*"../Sets/WallWindow.json"*/;
					}
				}
			}
			if (_mapNb > 0 && maze[_mapNb].stageList.size() == maze[_mapNb - 1].stageList.size())
			{
				if (maze[_mapNb - 1].stageList[_stageNb].chunk.type == BALCONY && _mapNb % _mapW != 0) // RIGHT BORDER
				{
					if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
					{
						maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
						maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].isVisible = true;
						maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallOutList[LEFT].pos.x -= BRICK_W;
						maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].name = _text.gardenRot[_stageNb]/*text.wallLeftWindow[maze[mapNb - 1].stageList[stageNb].chunk.type]*//*"../Sets/WallRotWindow.json"*/;
					}
				}
			}
			if (_mapNb - _mapW >= 0 && maze[_mapNb].stageList.size() == maze[_mapNb - _mapW].stageList.size())
			{
				if (maze[_mapNb - _mapW].stageList[_stageNb].chunk.type == BALCONY) // TOP BORDER
				{
					if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
					{
						maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
						maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].isVisible = true;
						maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallOutList[BOTTOM].pos.z -= BRICK_W;
						maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].name = _text.garden[_stageNb]/*text.wallBotWindow[maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type]*//*"../Sets/WallWindow.json"*/;
					}
				}
			}
		}
	}

	/////////////////////////////////////////////////////// LEFT BOT BORDER MAP ///////////////////////////////////////////////////////
	if (maze[_mapNb].stageList[_stageNb].chunk.type == LABY || maze[_mapNb].stageList[_stageNb].chunk.type == HOSPITAL || maze[_mapNb].stageList[_stageNb].chunk.type == GARDEN || maze[_mapNb].stageList[_stageNb].chunk.type == PARCEL)
	{
		if ((_mapNb + 1) % _mapW == 0) // RIGHT BORDER
		{
			if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
			{
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
			}
		}
		if (_mapNb > _mapW * _mapW - _mapW - 1) // TOP BORDER
		{
			if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
			{
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
			}
		}

		if ((_mapNb + 1) % _mapW == 0) // RIGHT BORDER
		{
			if ((_cell + 1) % NB_CELL == 0 || _cell == NB_CELL * NB_CELL - 1)
			{
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = true;
			}
		}
		if (_mapNb > _mapW * _mapW - _mapW - 1) // TOP BORDER
		{
			if (_cell > NB_CELL * NB_CELL - NB_CELL - 1)
			{
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = true;
			}
		}
		/////////////////////////////////////////////////////// EXIT BORDER ///////////////////////////////////////////////////////
		if (_stageNb > 0 && _mapNb + _mapW < _mapW * _mapW)
		{
			if (maze[_mapNb + _mapW].stageList[_stageNb - 1].chunk.type == EXIT) // Stage +1 Border
			{
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = false;
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = false;
				maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].ground[BOTTOM].isVisible = false;
				if (_mapNb - _mapW >= 0 && _cell >= NB_CELL * NB_CELL - NB_CELL && _cell < NB_CELL * NB_CELL)
				{
					maze[_mapNb - _mapW].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = false;
				}
				if (_mapW * _mapW % _mapNb != 0 && (_cell + 1) % NB_CELL == 0)
				{
					maze[_mapNb - 1].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = false;
				}

				if (_cell % NB_CELL == 0 && _mapNb % _mapW == 0)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallMissingList[0].isVisible = false;
				}
				if (_cell < NB_CELL && _mapNb < _mapW)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallMissingList[0].isVisible = false;
				}
			}
		}
		if ((_mapNb + 1) % _mapW != 0 && _mapNb + 1 < _mapW * _mapW) // Right Border
		{
			if (maze[_mapNb + 1].stageList[_stageNb].chunk.type == EXIT)
			{
				if ((_cell + 1) % NB_CELL == 0)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[LEFT].isVisible = false;
				}
			}
		}
		if (_mapNb + _mapW < _mapW * _mapW)
		{
			if (maze[_mapNb + _mapW].stageList[_stageNb].chunk.type == EXIT) // Top Border
			{
				if (_cell >= NB_CELL * NB_CELL - NB_CELL)
				{
					maze[_mapNb].stageList[_stageNb].chunk.cellList[_cell].wallList[BOTTOM].isVisible = false;
				}
			}
		}
	}
}

void Maze::CreateMaze()
{
	for (int mapNb = 0; mapNb < maze.size(); mapNb++)
	{
		for (int stageNb = 0; stageNb < maze[mapNb].stageList.size(); stageNb++)
		{
			//chunk load
			for (int decor = 0; decor < maze[mapNb].stageList[stageNb].chunk.decor.size(); decor++)
			{
				if (maze[mapNb].stageList[stageNb].chunk.decor[decor].isVisible)
				{
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor = Sets::Create();
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor->GenerateRenderingInstance();
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.decor[decor].name)), false);
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor->SetName("Wall");
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor->SetPath("../Sets/");
					maze[mapNb].stageList[stageNb].chunk.decor[decor].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.decor[decor].pos);
				}
			}

			for (int cell = 0; cell < maze[mapNb].stageList[stageNb].chunk.cellList.size(); cell++)
			{
				//ground load
				for (int ground = 0; ground < maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground.size(); ground++)
				{
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor->SetName("Ground");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].pos);
					}
				}

				for (int wall = 0; wall < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList.size(); wall++)
				{
					//wall load
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor->SetName("Wall");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].pos);
					}
				}

				for (int wallMissing = 0; wallMissing < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList.size(); wallMissing++)
				{
					//wallMissing load
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor->SetName("Wall");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].pos);
					}
				}

				for (int wallOut = 0; wallOut < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList.size(); wallOut++)
				{
					//wallOut load
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetName("Wall");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].pos);
					}
				}

				for (int Props = 0; Props < maze[mapNb].stageList[stageNb].chunk.cellList[cell].props.size(); Props++)
				{
					//props load
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->SetName("Props");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->SetRotation(maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].rot, true);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].props[Props].pos);
					}
				}

				for (int bedroom = 0; bedroom < maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom.size(); bedroom++)
				{
					//props load
					if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].isVisible)
					{
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor = Sets::Create();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->GenerateRenderingInstance();
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].name)), false);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetName("Props");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetPath("../Sets/");
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetRotation(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].rot, true);
						maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].pos);
					}
				}
			}
		}
	}
}

void Maze::GenerateMaze(int _mapW, int _nbStage)
{
	ManagmentText text = InitProps();

	for (int mapLine = 0; mapLine < _mapW; mapLine++)
	{
		for (int mapColumn = 0; mapColumn < _mapW; mapColumn++)
		{
			MegaChunck map;
			for (int stageNb = 0; stageNb < _nbStage; stageNb++)
			{
				Stage stage;
				Chunck chunck;

				StageManagment(mapColumn, mapLine, stageNb, chunck, stage, _mapW, _nbStage);
				InitCell(mapColumn, mapLine, stageNb, chunck, text, _mapW, _nbStage);
				InitLaby(chunck);

				stage.chunk = chunck;
				map.stageList.push_back(stage);
			}
			maze.push_back(map);
		}
	}

	std::cout << maze.size() << std::endl;

	for (int mapNb = 0; mapNb < maze.size(); mapNb++)
	{
		for (int stageNb = 0; stageNb < maze[mapNb].stageList.size(); stageNb++)
		{
			InitHospital(mapNb, stageNb, _mapW, _nbStage);
			for (int cell = 0; cell < maze[mapNb].stageList[stageNb].chunk.cellList.size(); cell++)
			{
				InitSpecialChuncks(mapNb, stageNb, cell, text, _mapW, _nbStage);
			}
		}
	}
	CreateMaze();

}