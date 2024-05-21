#include "MapManager.h"
#include "Props.h"
#include "Maze.h"

std::vector<Map::Stage> map;

Map::ManagmentText Map::InitText(int _size)
{
	ManagmentText text;
	if (_size == NORMAL)
	{
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //LABY
		text.ground.push_back("../Sets/GARDEN/Map/GRD_S1Dirt.json"); //GARDEN
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //HOSPITAL
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //EXIT
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //ROOM
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //PARCEL
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //BALCONY
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground2.json"); //EMPTY
		text.ground.push_back("../Sets/LIBRARY/Map/LBR_S.json"); //EMPTY
		text.ground.push_back("../Sets/LIBRARY/Map/LBR_S.json"); //EMPTY

		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");

		//text.wall.push_back("../Sets/LIBRARY/Map/HSP_Wall1.json");
		//text.wall.push_back("../Sets/LIBRARY/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/LIBRARY/Map/LBR_Wall.json");
		text.wall.push_back("../Sets/GARDEN/Map/GRD_W1b.json");
		text.wall.push_back("../Sets/LIBRARY/Map/LBR_Window.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
		//text.wall.push_back("../Sets/LIBRARY/Map/HSP_Wall1.json");
		//text.wall.push_back("../Sets/LIBRARY/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/LIBRARY/Map/LBR_Wall.json");
		text.wall.push_back("../Sets/LIBRARY/Map/LBR_Wall.json");

		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
		text.door.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");

		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/LIBRARY/Map/LBR_Window.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.wallWindow.push_back("../Sets/LIBRARY/Map/LBR_Window.json");
		text.wallWindow.push_back("../Sets/LIBRARY/Map/LBR_Window.json");

		text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json"); 
		text.garden.push_back("../Sets/GARDEN/Map/GRD_W1b.json");
		text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
		text.garden.push_back("../Sets/HOSPITAL/Map/HSP_Window2.json");
	}

	if (_size == TINY)
	{
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //LABY
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //GARDEN
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //HOSPITAL
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //EXIT
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //ROOM
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //PARCEL
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //BALCONY
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //EMPTY
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //EMPTY
		text.ground.push_back("../Sets/HOSPITAL/Map/TinyGround.json"); //EMPTY

		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");
		text.roof.push_back("../Sets/HOSPITAL/Map/TinyRoof.json");

		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
		text.wall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");

		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
		text.door.push_back("../Sets/HOSPITAL/Map/TinyDoor.json");
	}

	//if ()

	return text;
}

void Map::StageManagment(Stage& _stage, int _stageNb, int _mapW, int _nbStage)
{
	if (_stageNb == 0)//!!!!!!!!!!!!!!!!!!!!!!!
	{
		_stage.type = HOSPITALS;
	}
	else if (_stageNb == 1)
	{
		_stage.type = LABO;
	}
	else
	{
		_stage.type = FIELDS;
	}

	//_stage.type = _stageNb;

	int infoRoomNb = Props::TextChunck(_stage.type).room.size();
	_stage.txtRoom = Props::TextChunck(_stage.type).room;
	//_stage.chunckList[randomRoom].nameRoom = _stage.txtRoom[randomRoomTxt]
	//Map::ManagmentTextProps txtRoom = Props::TextChunck(_stage.type);
	//std::cout << infoRoomNb << std::endl;

	if (_stage.type == HOSPITALS)
	{
		_stage.hightScale = SIZE_HOSPITALS;
	}
	else if (_stage.type == LABO)
	{
		_stage.hightScale = SIZE_LABO;
	}
	else if (_stage.type == LIBRARY)
	{
		_stage.hightScale = SIZE_LIBRARY;
	}
	else if (_stage.type == FIELDS)
	{
		_stage.hightScale = SIZE_FIELDS;
	}

	if (_stageNb != 0)
	{
		_stage.sizeOf = map[_stageNb - 1].hightScale;
		_stage.hightScale += _stage.sizeOf;
	}
	else
	{
		_stage.sizeOf = 0;
		//_stage.hightScale = 0;
	}

	std::cout << "size " << _stage.hightScale << std::endl;

	for (int i = 0; i < _mapW * _mapW; i++)
	{
		Chunck chunck;

		chunck.size = NORMAL * NB_CELL;
		chunck.w = CELL_W / NORMAL;
		chunck.txt = InitText(NORMAL);

		if (_stage.type == HOSPITALS)
		{
			if (rand() % 100 < SHORTCUT_LUCK)
			{
				chunck.type = Map::HOSPITAL;
			}
			else
			{
				chunck.type = Map::PLACE;
			}
		}//!!!!!!!!!!!!!!!!!!!!!!!!!
		else if (_stage.type == LIBRARY) //!!!!!!!!!!!!!!!!!!!!!!!!!
		{//!!!!!!!!!!!!!!!!!!!!!!!!!
			//if (rand() % 100 < SHORTCUT_LUCK + 10)
			//{
			chunck.type = Map::LIBRARIES;
			//}
			//else
			//{
			//	chunck.type = Map::LIBRARYSTAIRS;
			//}
		}
		else if (_stage.type == LABO)
		{
			chunck.type = Map::LABY;
		}
		_stage.chunckList.push_back(chunck);
	}

	int sizeMap = _mapW * _mapW;
	for (int garden = 0; garden < 2; garden++)
	{
		if (_stageNb == 0)
		{
			_stage.chunckList[rand() % sizeMap].type = GARDEN;
		}
	}

	for (int i = 0; i < _mapW * _mapW; i++)
	{
		if (_stageNb > 0 && map[_stageNb - 1].chunckList[i].type == Map::GARDEN)
		{
			_stage.chunckList[i].type = Map::GARDEN;
		}
	}

	for (int i = 0; i < 1; i++)
	{
		int randomExit = 0;
		do
		{
			do
			{
				randomExit = rand() % sizeMap;
			} while (randomExit < _mapW || randomExit >= _mapW * _mapW - _mapW || randomExit % _mapW == 0 || (randomExit + 1) % _mapW == 0);
		} while (_stage.chunckList[randomExit].type == GARDEN || _stage.chunckList[randomExit - 1].type == GARDEN || _stage.chunckList[randomExit + 1].type == GARDEN || _stage.chunckList[randomExit + _mapW].type == GARDEN || _stage.chunckList[randomExit - _mapW].type == GARDEN);

		_stage.chunckList[randomExit].type = EXIT;
		_stage.chunckList[randomExit].nameRoom = Props::TextChunck(_stage.type).exit[_stage.type];
	}

	for (int i = 0; i < _mapW * _mapW; i++)
	{

		if (_stageNb > 0 && map[_stageNb - 1].chunckList[i].type == Map::EXIT)
		{
			_stage.chunckList[i].type = Map::ENTRANCE;
			_stage.chunckList[i].nameRoom = Props::TextChunck(_stage.type).entrance[_stage.type];
		}
	}

	if (_stageNb == 0)
	{
		int randomEntrance = 0;
		do
		{
			do
			{
				do
				{
					randomEntrance = rand() % sizeMap;
				} while (randomEntrance < _mapW || randomEntrance >= _mapW * _mapW - _mapW || randomEntrance % _mapW == 0 || (randomEntrance + 1) % _mapW == 0);
			} while (_stage.chunckList[randomEntrance].type == GARDEN || _stage.chunckList[randomEntrance - 1].type == GARDEN || _stage.chunckList[randomEntrance + 1].type == GARDEN || _stage.chunckList[randomEntrance + _mapW].type == GARDEN || _stage.chunckList[randomEntrance - _mapW].type == GARDEN);
		} while (_stage.chunckList[randomEntrance].type == EXIT);
		_stage.chunckList[randomEntrance].type = ENTRANCE;
		_stage.chunckList[randomEntrance].nameRoom = Props::TextChunck(_stage.type).entrance[_stage.type];
	}

	for (int i = 0; i < infoRoomNb; i++)
	{
		int randomRoom = 0;
		int randomRoomTxt = rand() % _stage.txtRoom.size();
		do
		{
			do
			{
				do
				{
					do
					{
						do
						{
							randomRoom = rand() % sizeMap;
						} while (randomRoom < _mapW || randomRoom >= _mapW * _mapW - _mapW || randomRoom % _mapW == 0 || (randomRoom + 1) % _mapW == 0);
					} while (_stage.chunckList[randomRoom].type == GARDEN || _stage.chunckList[randomRoom - 1].type == GARDEN || _stage.chunckList[randomRoom + 1].type == GARDEN || _stage.chunckList[randomRoom + _mapW].type == GARDEN || _stage.chunckList[randomRoom - _mapW].type == GARDEN);
				} while (_stage.chunckList[randomRoom].type == ENTRANCE);
			} while (_stage.chunckList[randomRoom].type == EXIT);
		} while (_stage.chunckList[randomRoom].type == ROOM);
		_stage.chunckList[randomRoom].type = ROOM;
		_stage.chunckList[randomRoom].nameRoom = _stage.txtRoom[randomRoomTxt];
		_stage.txtRoom.erase(_stage.txtRoom.begin() + randomRoomTxt);
	}

	for (int i = 0; i < _mapW * _mapW; i++)
	{
		if (_stage.chunckList[i].type == Map::GARDEN /*|| _stage.chunckList[i].type == Map::ROOM || _stage.chunckList[i].type == Map::ROOM*/)
		{
			if (i > 0)
			{
				_stage.chunckList[i - 1].size = NORMAL * NB_CELL;
				_stage.chunckList[i - 1].w = CELL_W / NORMAL;
				_stage.chunckList[i - 1].txt = InitText(NORMAL);
			}
			if (i >= _mapW)
			{
				_stage.chunckList[i - _mapW].size = NORMAL * NB_CELL;
				_stage.chunckList[i - _mapW].w = CELL_W / NORMAL;
				_stage.chunckList[i - _mapW].txt = InitText(NORMAL);
			}
		}
	}
}

void Map::GenerateMaze(int _mapW, int _nbStage)
{
	for (int stageNb = 0; stageNb < _nbStage; stageNb++)
	{
		Stage stage;
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		StageManagment(stage, stageNb, _mapW, _nbStage);
		Maze::InitCell(stage, stageNb, _mapW, _nbStage);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		Maze::InitLibrary(stage, _mapW, stageNb);
		Maze::InitLibraryStairs(stage, _mapW, stageNb);
		Maze::InitLaby(stage, _mapW);
		Maze::InitPlace(stage, _mapW, stageNb);
		Maze::InitHospital(stage, _mapW, stageNb);

		Maze::InitGarden(stage, _mapW, stageNb);
		Maze::InitExit(stage, _mapW, stageNb);
		Maze::InitRoom(stage, _mapW, stageNb);
		Maze::InitProps(stage, _mapW, stageNb);
		map.push_back(stage);
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	}
	CreateMaze();
}


void Map::CreateMaze()
{
	for (int stageNb = 0; stageNb < map.size(); stageNb++)
	{
		for (int mapNb = 0; mapNb < map[stageNb].chunckList.size(); mapNb++)
		{
			////chunk load
			if (map[stageNb].chunckList[mapNb].parentSet == nullptr)
			{
				map[stageNb].chunckList[mapNb].parentSet = Sets::Create();
				map[stageNb].chunckList[mapNb].parentSet->Initialize(false);
				map[stageNb].chunckList[mapNb].parentSet->GenerateRenderingInstance();
				map[stageNb].chunckList[mapNb].parentSet->SetName("Chunk");
				//std::cout << "Another chunk" << std::endl;
			}
			std::vector<Blocks::BlockType*>* instanceAddr = map[stageNb].chunckList[mapNb].parentSet->GetRenderingInstance();

			for (int decor = 0; decor < map[stageNb].chunckList[mapNb].decor.size(); decor++)
			{
				if (map[stageNb].chunckList[mapNb].decor[decor].isVisible)
				{
					map[stageNb].chunckList[mapNb].decor[decor].decor = Sets::Create();
					map[stageNb].chunckList[mapNb].decor[decor].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
					map[stageNb].chunckList[mapNb].decor[decor].decor->SetRenderingInstance(instanceAddr);
					//map[stageNb].chunckList[mapNb].decor[decor].decor->GenerateRenderingInstance();
					map[stageNb].chunckList[mapNb].decor[decor].decor->LoadFromJson(json::parse(Files::GetFileContent(map[stageNb].chunckList[mapNb].decor[decor].name)));
					map[stageNb].chunckList[mapNb].decor[decor].decor->SetName("Wall");
					map[stageNb].chunckList[mapNb].decor[decor].decor->SetPath("../Sets/");
					map[stageNb].chunckList[mapNb].decor[decor].decor->SetPosition(map[stageNb].chunckList[mapNb].decor[decor].pos);
				}
			}

			for (int cell = 0; cell < map[stageNb].chunckList[mapNb].cellList.size(); cell++)
			{
				//ground load
				for (int ground = 0; ground < map[stageNb].chunckList[mapNb].cellList[cell].ground.size(); ground++)
				{
					if (map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].isVisible)
					{
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor = Sets::Create();
						//map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetRenderingInstance(instanceAddr);
						//map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->GenerateRenderingInstance();
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->LoadFromJson(json::parse(Files::GetFileContent(map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].name)));
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetName("Ground");
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetPath("../Sets/");
						map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].decor->SetPosition(map[stageNb].chunckList[mapNb].cellList[cell].ground[ground].pos);
					}
				}

				for (int wall = 0; wall < map[stageNb].chunckList[mapNb].cellList[cell].wallList.size(); wall++)
				{
					//wall load
					if (map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].isVisible)
					{
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor = Sets::Create();
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetRenderingInstance(instanceAddr);
						//map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->GenerateRenderingInstance();
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->LoadFromJson(json::parse(Files::GetFileContent(map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].name)));
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetName("Wall");
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetPath("../Sets/");
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetPosition(map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].pos);
						map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].decor->SetRotation(map[stageNb].chunckList[mapNb].cellList[cell].wallList[wall].rot);
					}
				}

				for (int wallMissing = 0; wallMissing < map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList.size(); wallMissing++)
				{
					//wallMissing load
					if (map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].isVisible)
					{
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor = Sets::Create();
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetRenderingInstance(instanceAddr);
						//map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->GenerateRenderingInstance();
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->LoadFromJson(json::parse(Files::GetFileContent(map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].name)), false);
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetName("Wall");
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetPath("../Sets/");
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetPosition(map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].pos);
						map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].decor->SetRotation(map[stageNb].chunckList[mapNb].cellList[cell].wallMissingList[wallMissing].rot);
					}
				}

				for (int Props = 0; Props < map[stageNb].chunckList[mapNb].cellList[cell].props.size(); Props++)
				{
					//props load
					if (map[stageNb].chunckList[mapNb].cellList[cell].props[Props].isVisible)
					{
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor = Sets::Create();
						//map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->GenerateRenderingInstance();
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetParent(map[stageNb].chunckList[mapNb].parentSet, true);
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetRenderingInstance(instanceAddr);
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->LoadFromJson(json::parse(Files::GetFileContent(map[stageNb].chunckList[mapNb].cellList[cell].props[Props].name)), false);
						if (map[stageNb].chunckList[mapNb].cellList[cell].props[Props].name == "../Sets/HOSPITAL/Props/HSP_RoomDoor.json")
						{
							map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetName("Door");
						}
						else
						{
							map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetName("props");
						}
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetPath("../Sets/");
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetRotation(map[stageNb].chunckList[mapNb].cellList[cell].props[Props].rot, true);
						map[stageNb].chunckList[mapNb].cellList[cell].props[Props].decor->SetPosition(map[stageNb].chunckList[mapNb].cellList[cell].props[Props].pos);
					}
				}
			}

			map[stageNb].chunckList[mapNb].parentSet->ApplyTransformation();
			map[stageNb].chunckList[mapNb].parentSet->CalculateBoundingBox();
		}
	}
}

std::vector<Map::Stage> Map::GetMap()
{
	return map;
}

Map::Stage* Map::GetStage(Map::StageEnum _stageEnum)
{
	for (int i = 0; i < map.size(); i++)
	{
		if (map[i].type == _stageEnum)
		{
			return &map[i];
		}
	}
	return nullptr;
}

Sets::Set* Map::SearchForSetInStage(Stage* _stage, const char* _roomName)
{
	for (size_t chunkId = 0; chunkId < _stage->chunckList.size(); chunkId++)
	{
		if (_stage->chunckList[chunkId].nameRoom == nullptr)
		{
			continue;
		}

		if (!strcmp(_roomName, _stage->chunckList[chunkId].nameRoom))
		{
			std::vector<Map::Decor> decors = _stage->chunckList[chunkId].decor;
			for (size_t i = 0; i < decors.size(); i++)
			{
				if (!strcmp(_roomName, decors[i].name))
				{
					return decors[i].decor;
				}
			}
		}
	}
	return nullptr;
}

