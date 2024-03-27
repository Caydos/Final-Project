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
std::vector<Maze::Decor> Props::Generate(int _chunckType, glm::vec3 _cellPos, int _line, int _column)
{
	/////////////////////////////////////////////////////// INIT Props //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::vector<Maze::Decor> propsList;

	int randomLuck = 0;
	randomLuck = rand() % 100;
	if (_chunckType == Maze::HOSPITAL)
	{
		if (_column == NB_CELL / 2 && randomLuck > PROPS_LUCK && _line != NB_CELL / 2)
		{
			Maze::Decor bed;

			bed.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3, _cellPos.z + 2*(CELL_W / 3) + BRICK_W/2);
			bed.rot = glm::vec3(0, 0, 0);
			bed.isVisible = true;
			bed.name = "../Sets/HOSPITAL/Props/HSP_bed.json";

			propsList.push_back(bed);
		}
		if (_column != NB_CELL / 2 && _line == NB_CELL / 2 || _column != NB_CELL / 2 && _line == NB_CELL / 2  + 1)
		{
			Maze::Decor door;

			door.pos = glm::vec3(_cellPos.x - CELL_W / 2 + BRICK_W, _cellPos.y + BRICK_W / 3 - BRICK_W/2, _cellPos.z + BRICK_W / 2);
			door.rot = glm::vec3(0, 0, 0);
			door.isVisible = true;
			door.name = "../Sets/HOSPITAL/Props/HSP_FireproofDoor.json";

			propsList.push_back(door);
		}
	}

	std::cout << NB_CELL / 2 << std::endl;

	return propsList;
}
