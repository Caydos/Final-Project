#include "Set.h"
#include "Props.h"


Map::ManagmentTextProps Props::TextChunck(int _stageType)
{
	Map::ManagmentTextProps text;

	if (_stageType == Map::HOSPITALS)
	{
		//text.room.push_back("../Sets/HOSPITAL/Map/HSP_PlayRoomDodu.json");
		//text.room.push_back("../Sets/HOSPITAL/Map/HSP_PlayRoomDodu.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_WaitingRoom.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_WeirdRoomChair.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_PlayRoom.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_GiantBedRoom.json");

	}
	if (_stageType == Map::LIBRARY)
	{
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_PlayRoomDodu.json");
	}

	return text;
}


std::vector<Map::Decor> Props::Generate(Map::Cell _cell)
{
	/////////////////////////////////////////////////////// INIT Props //////////////////////////////////////////////////////////////////////////////////////////////////////////////

	std::vector<Map::Decor> propList;
	std::vector<const char*> txtProps{};

	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte01.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte02.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte03.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte04.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte05.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte06.json");

	int randomLuck = 0;
	randomLuck = rand() % 100;
	//std::cout << randomLuck << std::endl;

	//if (_chunckType == Map::HOSPITAL)
	//{
	//	if (_name == Props::CORRIDOR)
	//	{
	//		if (randomLuck > PROPS_LUCK)
	//		{
	//			if (_column == NB_CELL / 2 && _line != NB_CELL / 2)
	//			{
	//				decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ + CELL_W - BRICK_W);
	//				decor.rot = glm::vec3(0, 0, 0);
	//				decor.isVisible = true;
	//				decor.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
	//			}
	//		}
	//		//} 
	//		else if (randomLuck < 100 - PROPS_LUCK)
	//		{
	//			//if (_name == Props::BENCH)
	//			//{
	//			if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
	//			{
	//				//decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z /*+ 2 * (CELL_W / 3)*/ + BRICK_W / 2 + CELL_W);
	//				decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3);
	//				decor.rot = glm::vec3(0, 0, 0);
	//				decor.isVisible = true;
	//				decor.name = "../Sets/HOSPITAL/Props/HSP_Bench.json";
	//			}
	//		}
	//		else if (25 < randomLuck < 35)
	//		{
	//			//if (_name == Props::BENCH)
	//			//{
	//			if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
	//			{
	//				//decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z /*+ 2 * (CELL_W / 3)*/ + BRICK_W / 2 + CELL_W);
	//				decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W + BRICK_W * 3 + BRICK_W/2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + CELL_W/2);
	//				decor.rot = glm::vec3(0, -90, 0);
	//				decor.isVisible = true;
	//				decor.name = "../Sets/HOSPITAL/Props/HSP_VendingMachine.json";
	//			}
	//		}
	//	}


	/////SPAWN DOOR/////
	for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
	{
		if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Door.json" && _cell.wallList[wallId].isVisible)
		{
			Map::Decor door;
			if (wallId == Maze::BOTTOM)
			{
				door.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W * 3, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z);
			}
			else
			{
				door.pos = glm::vec3(_cell.wallList[wallId].pos.x, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z + BRICK_W * 3);
			}
			door.rot = _cell.wallList[wallId].rot;
			door.isVisible = _cell.wallList[wallId].isVisible;
			door.name = "../Sets/HOSPITAL/Props/HSP_RoomDoor.json";
			propList.push_back(door);
		}
	}


	/////SPAWN PANCARTES/////
	///// BEDROOM ET CORRIDOR SONT INVERSER ->> bedroom = corridor / corridor = bedroom /////
	if (_cell.type == Maze::BEDROOM) //PANCARTE DANS LES CORRIDOR MURS LEFT ET BOTTOM
	{
		//std::cout << _cell.type << std::endl;

		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
			{
				if (randomLuck > PROPS_LUCK)
				{
					Map::Decor ad;

					if (wallId == Maze::LEFT)
					{
						ad.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W / 2 + BRICK_W * 5 + (BRICK_W / 2), _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z + 2 * (CELL_W / 3) + BRICK_W / 2 /*+ CELL_W*/ /*+ BRICK_W * 3*/);
						ad.rot = glm::vec3(0, 180, 0);
					}
					else if (wallId == Maze::BOTTOM)
					{
						ad.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W / 2 + BRICK_W * 8, _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z + 2 * (CELL_W / 3) + BRICK_W / 2 /*+ CELL_W*/ - 1.67);
						ad.rot = glm::vec3(0, 90, 0);
					}
					ad.name = txtProps[rand() % txtProps.size()];
					ad.isVisible = true;
					propList.push_back(ad);
				}
			}
		}
	}
	if (_cell.type == Maze::CORRIDOR) //PANCARTES DANS LES BEDROOM MURS LEFT ET BOTTOM
	{
		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
			{
				Map::Decor ad;
				if (randomLuck > PROPS_LUCK)
				{
					if (wallId == Maze::LEFT)
					{
						ad.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W / 2 + BRICK_W * 5 + (BRICK_W / 2), _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z + 2 * (CELL_W / 3) + BRICK_W / 2 /*+ CELL_W*/ /*+ BRICK_W * 3*/);
						ad.rot = glm::vec3(0, 180, 0);
					}
					else if (wallId == Maze::BOTTOM)
					{
						ad.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W / 2 + BRICK_W * 8, _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z + 2 * (CELL_W / 3) + BRICK_W / 2 /*+ CELL_W*/ - 1.67);
						ad.rot = glm::vec3(0, 90, 0);
					}
					ad.name = txtProps[rand() % txtProps.size()];
					ad.isVisible = true;
					propList.push_back(ad);
				}
			}
		}
	}

	////// SPAWN LIGHT ////
	// V1 LIGHT AVEC MURS
	//for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
	//{
	//	if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" || _cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Door.json" || _cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/Wall1.json" && _cell.wallList[wallId].isVisible == true)
	//	{
	//		Map::Decor light;

	//		if (wallId == Maze::LEFT)
	//		{
	//			light.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.03, _cell.wallList[wallId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[wallId].pos.z + 1.15);
	//			light.rot = glm::vec3(0, 0, 0);
	//			light.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
	//			light.isVisible = true;
	//		}
	//		else if (wallId == Maze::BOTTOM)
	//		{
	//			light.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.15, _cell.wallList[wallId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[wallId].pos.z + 1.03);
	//			light.rot = glm::vec3(0, 0, 0);
	//			light.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
	//			light.isVisible = true;
	//		}
	//		propList.push_back(light);
	//	}
	//}

	/////V2 LIGHT AVEC PLAFOND/////
	for (int roofId = 0; roofId < _cell.ground.size(); roofId++)
	{
		if (_cell.ground[roofId].name == "../Sets/HOSPITAL/Map/PLF_HSP.json" && _cell.ground[roofId].isVisible == true)
		{
			Map::Decor light;

			light.pos = glm::vec3(_cell.wallList[roofId].pos.x - 1.47, _cell.wallList[roofId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[roofId].pos.z + 1.15);
			light.rot = glm::vec3(0, 0, 0);
			light.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
			light.isVisible = true;
			propList.push_back(light);
		}
	}



	/////SPAWN REMBARDE AUX MURS/////
	if (_cell.type == Maze::BEDROOM) //REMBARDE DANS LES CORRIDOR MURS LEFT ET BOTTOM
	{
		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
			{
				Map::Decor rail;
				if (wallId == Maze::LEFT)
				{
					rail.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W - BRICK_W + 3.02, _cell.wallList[wallId].pos.y + BRICK_W - 0.042, _cell.wallList[wallId].pos.z + BRICK_W + 2.3);
					rail.rot = glm::vec3(180, 0, 180);
					rail.isVisible = true;
					rail.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
					//propList.push_back(rail);
				}
				else if (wallId == Maze::BOTTOM)
				{
					rail.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W, _cell.wallList[wallId].pos.y + BRICK_W - 0.042, _cell.wallList[wallId].pos.z + BRICK_W * 1.1);
					rail.rot = glm::vec3(0, 90, 0);
					rail.isVisible = true;
					rail.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
				}
				propList.push_back(rail);
			}
		}
	}
	if (_cell.type == Maze::CORRIDOR) //REMBARDE DANS LES BEDROOM MURS LEFT ET BOTTOM
	{
		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
			{
				Map::Decor rail;
				if (wallId == Maze::LEFT)
				{
					rail.pos = glm::vec3(_cell.wallList[wallId].pos.x - CELL_W - BRICK_W + 3.02, _cell.wallList[wallId].pos.y + BRICK_W - 0.042, _cell.wallList[wallId].pos.z + 2.5);
					rail.rot = glm::vec3(180, 0, 180);
					rail.isVisible = true;
					rail.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
				}
				else if (wallId == Maze::BOTTOM)
				{
					rail.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W, _cell.wallList[wallId].pos.y + BRICK_W - 0.042, _cell.wallList[wallId].pos.z + BRICK_W * 1.1);
					rail.rot = glm::vec3(0, 90, 0);
					rail.isVisible = true;
					rail.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
				}
				propList.push_back(rail);
			}
		}
	}

	//if (_name == Props::RAIL)
	//{
	//	if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
	//	{
	//		decor.pos = glm::vec3(_cellPos.x - CELL_W - BRICK_W - 0.02, _cellPos.y + BRICK_W, _cellPos.z + BRICK_W);
	//		decor.rot = glm::vec3(0, 0, 0);
	//		decor.isVisible = true;
	//		decor.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
	//	}
	//}


	//if (_name == Props::PANELEXIT)
	//{
	//	if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line == NB_CELL / 2)
	//	{
	//		decor.pos = glm::vec3(_cellPos.x /*- CELL_W / 2*/ /*- BRICK_W*//* - BRICK_W/2*/, _cellPos.y + CELL_W - BRICK_W, _cellPos.z + CELL_W / 2/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*//* + BRICK_W * 3*/);;
	//		decor.rot = glm::vec3(0, 0, 0);
	//		decor.isVisible = true;
	//		decor.name = "../Sets/HOSPITAL/Props/HSP_Exit2.json";
	//	}
	//}


	//if (_name == Props::BOOK)
	//{
	//	if (_line == 0 && _column == 0 || _line == 0 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == 0)
	//	{
	//		decor.pos = glm::vec3(_cellPos.x - CELL_W/* + BRICK_W + BRICK_W * 3 - BRICK_W * 2*/ + BRICK_W / 2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 /*+ BRICK_W*3*/ + CELL_W - BRICK_W / 2);
	//		decor.rot = glm::vec3(0, 0, 0);
	//		decor.isVisible = false;
	//		decor.name = "../Sets/HOSPITAL/Props/HSP_Shelf.json";
	//	}
	//}
	//}

	return propList;
}


/////SPAWN ROOM/////
std::vector<Map::Decor> Props::GenerateRoom(Map::Chunck _chunk)
{
	////////////////////////////////////////////////// INIT Room ///////////////////////////////////////////////////////

	//scale les ballon x3 pour la playroom//

	std::vector<Map::Decor> roomList;

	Map::Decor room;
	int randomRoom = 0;
	randomRoom = rand() % _chunk.txtRoom.size();

	room.pos = glm::vec3(_chunk.cellList[0].ground[0].pos.x - BRICK_W / 2, _chunk.cellList[0].ground[0].pos.y + BRICK_W + (BRICK_W / 5), _chunk.cellList[0].ground[0].pos.z);
	room.rot = glm::vec3(0, 0, 0);
	room.isVisible = true;
	room.name = _chunk.txtRoom[randomRoom]  /*"../Sets/HOSPITAL/Map/HSP_PlayRoomDodu.json"*/;
	roomList.push_back(room);
	std::cout << _chunk.txtRoom[randomRoom] << std::endl;
	_chunk.txtRoom.erase(_chunk.txtRoom.begin() + randomRoom);

	return roomList;
}
