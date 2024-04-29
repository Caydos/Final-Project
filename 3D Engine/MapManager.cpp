#include "MapManager.h"
#include "Props.h"
#include "Maze.h"

std::vector<Map::Stage> map;

Map::ManagmentText Map::InitText(int _size)
{
	ManagmentText text;
	if (_size == NORMAL)
	{
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //LABY
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //GARDEN
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //HOSPITAL
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EXIT
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //ROOM
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //PARCEL
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //BALCONY
		text.ground.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EMPTY
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
		text.wall.push_back("../Sets/LIBRARY/Map/LBR_Wall.json");
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
		text.garden.push_back("../Sets/LIBRARY/Map/LBR_Window.json");
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
}

void Map::GenerateMaze(int _mapW, int _nbStage)
{
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

				//for (int wallOut = 0; wallOut < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList.size(); wallOut++)
				//{
				//	//wallOut load
				//	if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].isVisible)
				//	{
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor = Sets::Create();
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetParent(maze[mapNb].stageList[stageNb].chunk.parentSet, true);
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetRenderingInstance(instanceAddr);
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].name)), false);
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetName("Wall");
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetPath("../Sets/");
				//		maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallOutList[wallOut].pos);
				//	}
				//}

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

				//			for (int bedroom = 0; bedroom < maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom.size(); bedroom++)
				//			{
				//				//props load
				//				if (maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].isVisible)
				//				{
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor = Sets::Create();
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetParent(maze[mapNb].stageList[stageNb].chunk.parentSet, true);
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetRenderingInstance(instanceAddr);
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->LoadFromJson(json::parse(Files::GetFileContent(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].name)), false);
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetName("Props");
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetPath("../Sets/");
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetRotation(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].rot, true);
				//					maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].decor->SetPosition(maze[mapNb].stageList[stageNb].chunk.cellList[cell].bedroom[bedroom].pos);
				//				}
				//			}
				//		}
			}

			map[stageNb].chunckList[mapNb].parentSet->ApplyTransformation();
			map[stageNb].chunckList[mapNb].parentSet->CalculateBoundingBox();
			//	}
			//}
			//for (int objective = 0; objective < objectiveList.size(); objective++)
			//{
			//	if (objectiveList[objective].isVisible)
			//	{
			//		objectiveList[objective].decor = Sets::Create();
			//		objectiveList[objective].decor->GenerateRenderingInstance();
			//		objectiveList[objective].decor->LoadFromJson(json::parse(Files::GetFileContent(objectiveList[objective].name)), false);
			//		objectiveList[objective].decor->SetName("Books");
			//		objectiveList[objective].decor->SetPath("../Sets/");
			//		objectiveList[objective].decor->SetRotation(objectiveList[objective].rot, true);
			//		objectiveList[objective].decor->SetPosition(objectiveList[objective].pos);
			//	}
			//}
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

