#include "Set.h"
#include "Props.h"
//
//void Props::Text()
//{
//	ManagmentText text;
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //LABY
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //GARDEN
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //HOSPITAL
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EXIT
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //ROOM
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //PARCEL
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //BALCONY
//	text.hospital.push_back("../Sets/HOSPITAL/Map/HSP_Ground.json"); //EMPTY
//
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//	text.library.push_back("../Sets/HOSPITAL/Map/PLF_HSP.json");
//
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Door.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//	text.labo.push_back("../Sets/HOSPITAL/Map/HSP_Wall2.json");
//
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//	text.hall.push_back("../Sets/HOSPITAL/Map/HSP_Wall1.json");
//}
//
Maze::Decor Props::Generate(int _chunckType, glm::vec3 _cellPos, int _line, int _column, int _name)
{
	/////////////////////////////////////////////////////// INIT Props //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	Maze::Decor decor{};
	std::vector<const char*> txtProps{};

	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte01.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte02.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte03.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte04.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte05.json");
	txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte06.json");

	int randomLuck = 0;
	randomLuck = rand() % 100;
	std::cout << randomLuck << std::endl;

	if (_chunckType == Maze::HOSPITAL)
	{
		if (_name == Props::CORRIDOR)
		{
			if (randomLuck > PROPS_LUCK)
			{
				if (_column == NB_CELL / 2 && _line != NB_CELL / 2)
				{
					decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ + CELL_W - BRICK_W);
					decor.rot = glm::vec3(0, 0, 0);
					decor.isVisible = true;
					decor.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
				}
			}
			//} 
			else if (randomLuck < 100 - PROPS_LUCK)
			{
				//if (_name == Props::BENCH)
				//{
				if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
				{
					//decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z /*+ 2 * (CELL_W / 3)*/ + BRICK_W / 2 + CELL_W);
					decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3);
					decor.rot = glm::vec3(0, 0, 0);
					decor.isVisible = true;
					decor.name = "../Sets/HOSPITAL/Props/HSP_Bench.json";
				}
			}
			else if (25 < randomLuck < 35)
			{
				//if (_name == Props::BENCH)
				//{
				if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
				{
					//decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z /*+ 2 * (CELL_W / 3)*/ + BRICK_W / 2 + CELL_W);
					decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W + BRICK_W * 3 + BRICK_W/2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + CELL_W/2);
					decor.rot = glm::vec3(0, -90, 0);
					decor.isVisible = true;
					decor.name = "../Sets/HOSPITAL/Props/HSP_VendingMachine.json";
				}
			}
		}
		if (_name == Props::AD)
		{
			if (randomLuck < PROPS_LUCK)
			{
				if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
				{
					decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W * 4 + BRICK_W / 2, _cellPos.y + BRICK_W / 3 + CELL_W / 2, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3);
					decor.rot = glm::vec3(0, 0, 0);
					decor.isVisible = true;
					decor.name = txtProps[rand() % txtProps.size()]/* "../Sets/HOSPITAL/Props/HSP_Pancarte06.json"*/;
				}
			}
		}
		if (_name == Props::DOOR)
		{
			if (_column != NB_CELL / 2 && _line == NB_CELL / 2 || _column != NB_CELL / 2 && _line == NB_CELL / 2 - 1)
			{
				//Maze::Decor door;

				decor.pos = glm::vec3(_cellPos.x - BRICK_W * 3 - BRICK_W / 2 - 0.025, _cellPos.y, _cellPos.z);
				decor.rot = glm::vec3(0, 0, 0);
				decor.isVisible = true;
				decor.name = "../Sets/HOSPITAL/Props/HSP_RoomDoor.json";

				//return door;
			}
		}
		if (_name == Props::LIGHT)
		{
			decor.pos = glm::vec3(_cellPos.x - CELL_W / 2 - BRICK_W, _cellPos.y + CELL_W - BRICK_W, _cellPos.z + CELL_W / 2 - BRICK_W / 2/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*//* + BRICK_W * 3*/);;
			decor.rot = glm::vec3(0, 0, 0);
			decor.isVisible = true;
			decor.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
		}
		if (_name == Props::PANELEXIT)
		{
			if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line == NB_CELL / 2)
			{
				decor.pos = glm::vec3(_cellPos.x /*- CELL_W / 2*/ /*- BRICK_W*//* - BRICK_W/2*/, _cellPos.y + CELL_W - BRICK_W, _cellPos.z + CELL_W / 2/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*//* + BRICK_W * 3*/);;
				decor.rot = glm::vec3(0, 0, 0);
				decor.isVisible = true;
				decor.name = "../Sets/HOSPITAL/Props/HSP_Exit2.json";
			}
		}
		if (_name == Props::RAIL)
		{
			if (_column == NB_CELL / 2 /*&& randomLuck < (100 - PROPS_LUCK)*/ && _line != NB_CELL / 2)
			{
				decor.pos = glm::vec3(_cellPos.x - CELL_W - BRICK_W - 0.02, _cellPos.y + BRICK_W, _cellPos.z + BRICK_W);
				decor.rot = glm::vec3(0, 0, 0);
				decor.isVisible = true;
				decor.name = "../Sets/HOSPITAL/Props/HSP_Rail1.json";
			}
		}
		if (_name == Props::BOOK)
		{
			if (_line == 0 && _column == 0 || _line == 0 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == 0)
			{
				decor.pos = glm::vec3(_cellPos.x - CELL_W/* + BRICK_W + BRICK_W * 3 - BRICK_W * 2*/ + BRICK_W / 2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 /*+ BRICK_W*3*/ + CELL_W - BRICK_W / 2);
				decor.rot = glm::vec3(0, 0, 0);
				decor.isVisible = false;
				decor.name = "../Sets/HOSPITAL/Props/HSP_Shelf.json";
			}
		}
	}
	return decor;
}

std::vector<Maze::Decor> Props::GenerateBedRoom(int _chunckType, glm::vec3 _cellPos, int _line, int _column)
{

	std::vector<Maze::Decor> room;

	if (_chunckType == Maze::HOSPITAL)
	{
		if (_line == 0 && _column == 0 || _line == 0 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == NB_CELL / 2 + 1 || _line == NB_CELL / 2 + 1 && _column == 0)
		{
			Maze::Decor bed1;
			Maze::Decor bed2;
			Maze::Decor name;
			Maze::Decor book;
			Maze::Decor chair;
			Maze::Decor table;
			Maze::Decor perch;

			bed1.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W * 2, _cellPos.y + BRICK_W / 3, _cellPos.z + BRICK_W * 3 + CELL_W - BRICK_W/* + CELL_W/2*/);
			bed1.rot = glm::vec3(0, 90, 0);
			bed1.isVisible = true;
			bed1.name = "../Sets/HOSPITAL/Props/HSP_Bed.json";

			bed2.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W * 2 + CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z + BRICK_W * 3 + CELL_W - BRICK_W * 3 - BRICK_W / 3/* + CELL_W/2*/);
			bed2.rot = glm::vec3(0, -90, 0);
			bed2.isVisible = true;
			bed2.name = "../Sets/HOSPITAL/Props/HSP_Bed.json";

			chair.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W + BRICK_W * 3 - BRICK_W * 2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 + BRICK_W);
			chair.rot = glm::vec3(0, rand() % 90, 0);
			chair.isVisible = true;
			chair.name = "../Sets/HOSPITAL/Props/HSP_Table.json";

			book.pos = glm::vec3(_cellPos.x - CELL_W/* + BRICK_W + BRICK_W * 3 - BRICK_W * 2*/ + BRICK_W/2, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 /*+ BRICK_W*3*/ + CELL_W - BRICK_W/2);
			book.rot = glm::vec3(0, 0, 0);
			book.isVisible = true;
			book.name = "../Sets/HOSPITAL/Props/HSP_Shelf.json";

			table.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W + BRICK_W * 2 + BRICK_W * 3, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 + BRICK_W * 2 - BRICK_W * 3);
			table.rot = glm::vec3(0, 90, 0);
			table.isVisible = true;
			table.name = "../Sets/HOSPITAL/Props/HSP_Armchair.json";

			perch.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W + BRICK_W * 2 + BRICK_W * 3, _cellPos.y + BRICK_W / 3, _cellPos.z/* + 2 * (CELL_W / 3) + BRICK_W / 2*/ /*+ CELL_W*/ + BRICK_W * 3 + BRICK_W * 2 - BRICK_W * 3);
			perch.rot = glm::vec3(0, 0, 0);
			perch.isVisible = true;
			perch.name = "../Sets/HOSPITAL/Props/HSP_Perch.json";

			room.push_back(bed1);
			room.push_back(bed2);
			room.push_back(chair);
			//room.push_back(book);
			room.push_back(table);
			room.push_back(perch);

		}
	}
	return room;
}
