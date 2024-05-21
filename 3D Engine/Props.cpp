#include "Set.h"
#include "Props.h"


Map::ManagmentTextProps Props::TextChunck(int _stageType)
{
	Map::ManagmentTextProps text;

	if (_stageType == Map::HOSPITALS)
	{
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_WaitingRoom.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_BreakRoom.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_WeirdRoomChair.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_PlayRoom.json");
		text.room.push_back("../Sets/HOSPITAL/Map/HSP_GiantBedRoom.json");

		text.exit.push_back("../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");
		text.exit.push_back("../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");
		text.exit.push_back("../Sets/HOSPITAL/Map/HSP_HOSPITALEXIT.json");

		text.entrance.push_back("../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
		text.entrance.push_back("../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
		text.entrance.push_back("../Sets/HOSPITAL/Map/HSP_ReceptionDesk.json");
	}
	if (_stageType == Map::LIBRARY)
	{
		text.room.push_back("../Sets/LIBRARY/Map/LBR_LongTableRoom.json");
		text.room.push_back("../Sets/LIBRARY/Map/LBR_MediaRoom.json");
		text.room.push_back("../Sets/LIBRARY/Map/LBR_RugRoom.json");

		text.exit.push_back("../Sets/LIBRARY/Map/LBR_LIBRARYEXIT.json");
		text.exit.push_back("../Sets/LIBRARY/Map/LBR_LIBRARYEXIT.json");
		text.exit.push_back("../Sets/LIBRARY/Map/LBR_LIBRARYEXIT.json");

		text.entrance.push_back("../Sets/LIBRARY/Map/LBR_ReceptionDesk.json");
		text.entrance.push_back("../Sets/LIBRARY/Map/LBR_ReceptionDesk.json");
		text.entrance.push_back("../Sets/LIBRARY/Map/LBR_ReceptionDesk.json");
	}
	if (_stageType == Map::LABO)
	{
		text.room.push_back("../Sets/GARDEN/Map/GRD_Fountain.json");
		text.room.push_back("../Sets/GARDEN/Map/GRD_Place.json");
		text.room.push_back("../Sets/GARDEN/Map/GRD_House.json");

		text.exit.push_back("../Sets/GARDEN/Map/GRD_EXIT.json");
		text.exit.push_back("../Sets/GARDEN/Map/GRD_EXIT.json");
		text.exit.push_back("../Sets/GARDEN/Map/GRD_EXIT.json");

		text.entrance.push_back("../Sets/GARDEN/Map/GRD_ENTER.json");
		text.entrance.push_back("../Sets/GARDEN/Map/GRD_ENTER.json");
		text.entrance.push_back("../Sets/GARDEN/Map/GRD_ENTER.json");
	}

	return text;
}


std::vector<Map::Decor> Props::GenerateHospital(Map::Cell _cell, int _chunckType)
{
	/////////////////////////////////////////////////////// INIT Props ////////////////////////////////////////////////////////////////

	std::vector<Map::Decor> propList;
	std::vector<const char*> txtProps{};

	if (_chunckType == Map::HOSPITAL || _chunckType == Map::PLACE)
	{
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte01.json");
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte02.json");
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte03.json");
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte04.json");
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte05.json");
		txtProps.push_back("../Sets/HOSPITAL/Props/HSP_Pancarte06.json");

		//Chance random de pop chaque objet
		int randomLuck = 0;
		randomLuck = rand() % 100;
		//std::cout << randomLuck << std::endl;

		//Angle random des panneau exit
		int randomRotation = 0;
		randomRotation = rand() % 4;

		glm::vec3 rotation;
		if (randomRotation == 1)
		{
			rotation = glm::vec3(0, 0, 0);
		}
		else if (randomRotation == 2)
		{
			rotation = glm::vec3(0, 90, 0);
		}
		else if (randomRotation == 3)
		{
			rotation = glm::vec3(0, 180, 0);
		}
		else if (randomRotation == 4)
		{
			rotation = glm::vec3(0, 360, 0);
		}


		/////SPAWN DOOR/////
		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Door.json" && _cell.wallList[wallId].isVisible || _cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/TinyDoor.json" && _cell.wallList[wallId].isVisible)
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

				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/TinyDoor.json")
				{
					if (wallId == Maze::BOTTOM)
					{
						door.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W * 3 - 0.75, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z);
					}
					else
					{
						door.pos = glm::vec3(_cell.wallList[wallId].pos.x, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z + BRICK_W * 3 - 0.75);
					}
				}

				door.rot = _cell.wallList[wallId].rot;
				door.isVisible = _cell.wallList[wallId].isVisible;
				door.name = "../Sets/HOSPITAL/Props/HSP_RoomDoor.json";
				propList.push_back(door);
			}
		}

		/////V2 LIGHT AVEC PLAFOND/////
		for (int roofId = 0; roofId < _cell.ground.size(); roofId++)
		{
			if (_cell.ground[roofId].name == "../Sets/HOSPITAL/Map/PLF_HSP.json" /*|| _cell.ground[roofId].name == "../Sets/HOSPITAL/Map/TinyRoof.json"*/ && _cell.ground[roofId].isVisible == true)
			{
				Map::Decor light;

				/*if (_cell.ground[roofId].name == "../Sets/HOSPITAL/Map/TinyRoof.json")
				{
					light.pos = glm::vec3(_cell.ground[roofId].pos.x + 0.5, _cell.ground[roofId].pos.y + 2.1, _cell.ground[roofId].pos.z + 0.25);
				}
				else
				{
					light.pos = glm::vec3(_cell.wallList[roofId].pos.x - 1.47, _cell.wallList[roofId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[roofId].pos.z + 1.15);
				}*/
				light.pos = glm::vec3(_cell.wallList[roofId].pos.x - 1.47, _cell.wallList[roofId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[roofId].pos.z + 1.15);
				light.rot = glm::vec3(0, 0, 0);
				light.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
				light.isVisible = true;
				propList.push_back(light);
			}
		}

		/////V2 LIGHT AVEC PLAFOND/////
		//for (int cellId = 0; cellId < NB_CELL * NB_CELL; cellId++)
		//{
		//	if (/*cellId % 2 == 0 &&*/ _cell.ground[cellId].name == "../Sets/HOSPITAL/Map/TinyRoof.json" && _cell.ground[cellId].isVisible == true)
		//	{
		//		/*if (randomLuck > 80)
		//		{*/
		//		Map::Decor light2;
		//		light2.pos = glm::vec3(_cell.ground[cellId].pos.x + 0.5, _cell.ground[cellId].pos.y + 2.1, _cell.ground[cellId].pos.z + 0.25);
		//		light2.rot = glm::vec3(0, 0, 0);
		//		light2.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
		//		light2.isVisible = true;
		//		propList.push_back(light2);
		//		//}
		//	}
		//}

		/////PANEL EXIT/////
		if (_cell.type == Maze::CORRIDOR)
		{
			for (int roofId = 0; roofId < _cell.ground.size(); roofId++)
			{
				Map::Decor exit;
				if (_cell.ground[roofId].name == "../Sets/HOSPITAL/Map/PLF_HSP.json" || _cell.ground[roofId].name == "../Sets/HOSPITAL/Map/TinyRoof.json" && _cell.ground[roofId].isVisible == true)
				{
					if (randomLuck > 98)
					{
						exit.pos = glm::vec3(_cell.wallList[roofId].pos.x - 1.35, _cell.wallList[roofId].pos.y + CELL_W - BRICK_W + 0.06, _cell.wallList[roofId].pos.z + 0.5);
						exit.name = "../Sets/HOSPITAL/Props/HSP_Exit2.json";
						exit.isVisible = true;
						propList.push_back(exit);
					}
				}
				exit.rot = glm::vec3(0, randomRotation, 0);
			}
		}

		/////SPAWN DEFIBRILATEUR DANS BEDROOM/////
		if (_cell.type == Maze::BEDROOM)
		{
			for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
			{
				Map::Decor defibr;
				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
				{
					if (randomLuck > 90)
					{
						if (wallId == Maze::LEFT)
						{
							defibr.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.13, _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z + 1);
							defibr.rot = glm::vec3(0, 180, 0);
						}
						else if (wallId == Maze::BOTTOM)
						{
							defibr.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1, _cell.wallList[wallId].pos.y + BRICK_W / 3 + CELL_W / 2 /*(laisser ca c'est la hauteur sur le mur)*/, _cell.wallList[wallId].pos.z - 0.13);
							defibr.rot = glm::vec3(0, 90, 0);
						}
						defibr.name = "../Sets/HOSPITAL/Props/HSP_Defibrilator.json";
						defibr.isVisible = true;
						propList.push_back(defibr);
					}
				}
			}
		}

	//PANCARTE DANS LES CORRIDOR MURS LEFT ET BOTTOM
	if (_cell.type == Maze::BEDROOM)
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

		/////REMBARDE A GAUCHE ET DROITE DES PORTES/////
		for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
		{
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Door.json" && _cell.wallList[wallId].isVisible)
			{
				Map::Decor smallRail;
				if (wallId == Maze::LEFT)
				{
					smallRail.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W - 0.42, _cell.wallList[wallId].pos.y + 1.009, _cell.wallList[wallId].pos.z + 0.1);
					smallRail.rot = glm::vec3(180, 0, 180);
					smallRail.isVisible = true;
					smallRail.name = "../Sets/HOSPITAL/Props/HSP_Rail.json";

				}
				else if (wallId == Maze::BOTTOM)
				{
					smallRail.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.6, _cell.wallList[wallId].pos.y + 1.009, _cell.wallList[wallId].pos.z - 0.17);
					smallRail.rot = glm::vec3(0, 90, 0);
					smallRail.isVisible = true;
					smallRail.name = "../Sets/HOSPITAL/Props/HSP_Rail.json";

				}
				propList.push_back(smallRail);
			}
			if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Door.json" && _cell.wallList[wallId].isVisible)
			{
				Map::Decor smallRail2;
				if (wallId == Maze::LEFT)
				{
					smallRail2.pos = glm::vec3(_cell.wallList[wallId].pos.x + BRICK_W - 0.42, _cell.wallList[wallId].pos.y + 1.009, _cell.wallList[wallId].pos.z + 2.2);
					smallRail2.rot = glm::vec3(180, 0, 180);
					smallRail2.isVisible = true;
					smallRail2.name = "../Sets/HOSPITAL/Props/HSP_Rail.json";

				}
				else if (wallId == Maze::BOTTOM)
				{
					smallRail2.pos = glm::vec3(_cell.wallList[wallId].pos.x + 2.62, _cell.wallList[wallId].pos.y + 1.009, _cell.wallList[wallId].pos.z - 0.17);
					smallRail2.rot = glm::vec3(0, 90, 0);
					smallRail2.isVisible = true;
					smallRail2.name = "../Sets/HOSPITAL/Props/HSP_Rail.json";

				}
				propList.push_back(smallRail2);
			}
		}

		/////SPAWN DES CHAISES / LIT / DISTRIB/////
		if (_cell.type == Maze::BEDROOM)
		{
			for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
			{
				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
				{

					//random pour chaque emplacement de mur
					int roll = rand() % 100;


					const int CHANCE_OF_ANY_PROP = 80;  //% de chance qu'un objet soit plac� a un emplacement mur

					if (roll < CHANCE_OF_ANY_PROP)
					{
						int itemRoll = rand() % 100;  //chance total qu'un type d'objet soit plac�

					if (itemRoll > 35 && itemRoll < 100)
					{
						Map::Decor bench;
						if (wallId == Maze::LEFT)
						{
							bench.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.98, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 2);
							bench.rot = glm::vec3(0, 180, 0);
							bench.isVisible = true;
							bench.name = "../Sets/HOSPITAL/Props/HSP_Bench.json";
						}
						else if (wallId == Maze::BOTTOM)
						{
							bench.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.8, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 0.98);
							bench.rot = glm::vec3(0, 90, 0);
							bench.isVisible = true;
							bench.name = "../Sets/HOSPITAL/Props/HSP_Bench.json";
						}
						propList.push_back(bench);
					}
					else if (itemRoll > 15 && itemRoll < 35)
					{
						Map::Decor bed;
						if (wallId == Maze::LEFT)
						{
							bed.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.2, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 2);
							bed.rot = glm::vec3(0, 0, 0);
							bed.isVisible = true;
							bed.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
						}
						else if (wallId == Maze::BOTTOM)
						{
							bed.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.9, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 0.9);
							bed.rot = glm::vec3(0, 90, 0);
							bed.isVisible = true;
							bed.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
						}
						propList.push_back(bed);
					}
					else if (itemRoll > 10 && itemRoll < 15)
					{
						Map::Decor distrib;
						if (wallId == Maze::LEFT)
						{
							distrib.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.2, _cell.wallList[wallId].pos.y + 0.1, _cell.wallList[wallId].pos.z + 1);
							distrib.rot = glm::vec3(0, 90, 0);
							distrib.isVisible = true;
							distrib.name = "../Sets/HOSPITAL/Props/HSP_VendingMachine.json";
						}
						propList.push_back(distrib);
					}
					else if (itemRoll > 0 && itemRoll < 10)
					{
						Map::Decor extinguisher;
						if (wallId == Maze::LEFT)
						{
							extinguisher.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.25, _cell.wallList[wallId].pos.y + 0.4, _cell.wallList[wallId].pos.z + 1.25);
							extinguisher.rot = glm::vec3(0, 0, 0);
							extinguisher.isVisible = true;
							extinguisher.name = "../Sets/HOSPITAL/Props/HSP_Extinguisher.json";
						}
						else if (wallId == Maze::BOTTOM)
						{
							extinguisher.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.25, _cell.wallList[wallId].pos.y + 0.4, _cell.wallList[wallId].pos.z + 0.25);
							extinguisher.rot = glm::vec3(0, -90, 0);
							extinguisher.isVisible = true;
							extinguisher.name = "../Sets/HOSPITAL/Props/HSP_Extinguisher.json";
						}
						propList.push_back(extinguisher);
					}
				}
			}
		}
	}

		/////SPAWN DES LIT DANS BEDROOM/////
		if (_cell.type == Maze::CORRIDOR)
		{
			for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
			{
				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
				{
					//random pour chaque emplacement de mur
					int roll = rand() % 100;

					const int CHANCE_OF_ANY_PROP = 50;  //% de chance qu'un objet soit plac� a un emplacement mur

				if (roll < CHANCE_OF_ANY_PROP)
				{
					Map::Decor bed;
					if (wallId == Maze::LEFT)
					{
						bed.pos = glm::vec3(_cell.wallList[wallId].pos.x + 0.2, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 2);
						bed.rot = glm::vec3(0, 0, 0);
						bed.isVisible = true;
						bed.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
					}
					else if (wallId == Maze::BOTTOM)
					{
						bed.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.9, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 0.98);
						bed.rot = glm::vec3(0, 90, 0);
						bed.isVisible = true;
						bed.name = "../Sets/HOSPITAL/Props/HSP_bed.json";
					}
					propList.push_back(bed);
				}
			}
		}
	}

		/////SPAWN DES TABLE / PERCH DANS BEDROOM/////
		if (_cell.type == Maze::BEDROOM)
		{
			for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
			{
				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
				{

					//random pour chaque emplacement de mur
					int roll = rand() % 100;
					const int CHANCE_OF_ANY_PROP = 20;  //% de chance qu'un objet soit plac� a un emplacement mur

					if (roll < CHANCE_OF_ANY_PROP)
					{
						int itemRoll = rand() % 100;  //chance total qu'un type d'objet soit plac�

						if (itemRoll > 50 && itemRoll < 100)
						{

						Map::Decor perch;
						if (wallId == Maze::LEFT)
						{
							perch.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.7, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z + 1.5);
							perch.rot = glm::vec3(0, 0, 0);
							perch.isVisible = true;
							perch.name = "../Sets/HOSPITAL/Props/HSP_Perch.json";
						}
						else if (wallId == Maze::BOTTOM)
						{
							perch.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1, _cell.wallList[wallId].pos.y + 0.08, _cell.wallList[wallId].pos.z - 0.5);
							perch.rot = glm::vec3(0, 90, 0);
							perch.isVisible = true;
							perch.name = "../Sets/HOSPITAL/Props/HSP_Perch.json";
						}
						propList.push_back(perch);
					}
					else if (itemRoll > 0 && itemRoll < 50)
					{
						Map::Decor table;
						if (wallId == Maze::LEFT)
						{
							table.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.4, _cell.wallList[wallId].pos.y + 0.1, _cell.wallList[wallId].pos.z + 1.5);
							table.rot = glm::vec3(0, 90, 0);
							table.isVisible = true;
							table.name = "../Sets/HOSPITAL/Props/HSP_Table.json";
						}
						else if (wallId == Maze::BOTTOM)
						{
							table.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.5, _cell.wallList[wallId].pos.y + 0.1, _cell.wallList[wallId].pos.z - 0.35);
							table.rot = glm::vec3(0, 0, 0);
							table.isVisible = true;
							table.name = "../Sets/HOSPITAL/Props/HSP_Table.json";
						}
						propList.push_back(table);
					}
				}
			}
		}
	}

		/////SPAWN DES MEDIKIT / CLEANING TROLLEY / WATER2 DANS CORRIDOR/////
		if (_cell.type == Maze::CORRIDOR)
		{
			for (int wallId = 0; wallId < _cell.wallList.size(); wallId++)
			{
				if (_cell.wallList[wallId].name == "../Sets/HOSPITAL/Map/HSP_Wall2.json" && _cell.wallList[wallId].isVisible == true)
				{

					//random pour chaque emplacement de mur
					int roll = rand() % 100;
					const int CHANCE_OF_ANY_PROP = 30;  //% de chance qu'un objet soit plac� a un emplacement mur

					if (roll < CHANCE_OF_ANY_PROP)
					{
						int itemRoll = rand() % 100;  //chance total qu'un type d'objet soit plac�

						if (itemRoll > 70 && itemRoll < 90)
						{
							Map::Decor water;
							if (wallId == Maze::LEFT)
							{
								water.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.5, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z + 2);
								water.rot = glm::vec3(0, 180, 0);
								water.isVisible = true;
								water.name = "../Sets/HOSPITAL/Props/HSP_Water2.json";
							}
							else if (wallId == Maze::BOTTOM)
							{
								water.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.5, _cell.wallList[wallId].pos.y, _cell.wallList[wallId].pos.z - 0.5);
								water.rot = glm::vec3(0, 90, 0);
								water.isVisible = true;
								water.name = "../Sets/HOSPITAL/Props/HSP_Water2.json";
							}
							propList.push_back(water);
						}
						else if (itemRoll > 40 && itemRoll < 60)
						{
							Map::Decor cleaning;
							if (wallId == Maze::LEFT)
							{
								cleaning.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.32, _cell.wallList[wallId].pos.y + 0.07, _cell.wallList[wallId].pos.z + 1.1);
								cleaning.rot = glm::vec3(0, 0, 0);
								cleaning.isVisible = true;
								cleaning.name = "../Sets/HOSPITAL/Props/HSP_CleaningTrolley.json";
							}
							else if (wallId == Maze::BOTTOM)
							{
								cleaning.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.1, _cell.wallList[wallId].pos.y + 0.07, _cell.wallList[wallId].pos.z - 0.7);
								cleaning.rot = glm::vec3(0, 90, 0);
								cleaning.isVisible = true;
								cleaning.name = "../Sets/HOSPITAL/Props/HSP_CleaningTrolley.json";
							}
							propList.push_back(cleaning);
						}
						else if (itemRoll > 0 && itemRoll < 40)
						{
							Map::Decor medikit;
							if (wallId == Maze::LEFT)
							{
								medikit.pos = glm::vec3(_cell.wallList[wallId].pos.x - 0.15, _cell.wallList[wallId].pos.y + 1.3, _cell.wallList[wallId].pos.z + 1.5);
								medikit.rot = glm::vec3(0, 180, 0);
								medikit.isVisible = true;
								medikit.name = "../Sets/HOSPITAL/Props/HSP_MediKit.json";
							}
							else if (wallId == Maze::BOTTOM)
							{
								medikit.pos = glm::vec3(_cell.wallList[wallId].pos.x + 1.5, _cell.wallList[wallId].pos.y + 1.3, _cell.wallList[wallId].pos.z - 0.15);
								medikit.rot = glm::vec3(0, 90, 0);
								medikit.isVisible = true;
								medikit.name = "../Sets/HOSPITAL/Props/HSP_MediKit.json";
							}
							propList.push_back(medikit);
						}
					}
				}
			}
		}
	}
	return propList;
}

////COLONNE PLACE ROOM////
//std::vector<Map::Decor> Props::GenerateColumn(Map::Cell _cell)
//{
//	std::vector<Map::Decor> propList;
//
//	for (int cellId = 0; cellId < _cell.ground.size(); cellId++)
//	{
//		if (_cell.ground[cellId].name == "../Sets/HOSPITAL/Map/PLF_HSP.json" && _cell.ground[cellId].isVisible == true)
//		{
//			Map::Decor column;
//
//			column.pos = glm::vec3(_cell.ground[cellId].pos.x + 2, _cell.ground[cellId].pos.y, _cell.ground[cellId].pos.z + 2);
//			column.rot = glm::vec3(0, 0, 0);
//			column.name = "../Sets/HOSPITAL/Props/Column.json";
//			column.isVisible = true;
//			propList.push_back(column);
//		}
//	}
//
//	return propList;
//}
//
//////LIGHT PLACE ROOM////
//std::vector<Map::Decor> Props::GenerateLight(Map::Cell _cell)
//{
//	std::vector<Map::Decor> propList;
//
//	////LIGHT////
//	for (int roofId = 0; roofId < _cell.ground.size(); roofId++)
//	{
//		if (_cell.ground[roofId].name == "../Sets/HOSPITAL/Map/PLF_HSP.json" && _cell.ground[roofId].isVisible == true)
//		{
//			Map::Decor light;
//			light.pos = glm::vec3(_cell.wallList[roofId].pos.x - 2.2, _cell.wallList[roofId].pos.y + CELL_W - BRICK_W + 0.04, _cell.wallList[roofId].pos.z + 0.4);
//			light.rot = glm::vec3(0, 0, 0);
//			light.name = "../Sets/HOSPITAL/Props/HSP_Light.json";
//			light.isVisible = true;
//			propList.push_back(light);
//
//		}
//	}
//
//	return propList;
//}

////GENERATE LGHT AND COLONNE////
std::vector<Map::Decor> Props::GenerateProps(Map::Chunck& _chunck)
{
	for (int cell = 0; cell < _chunck.cellList.size(); cell++)
	{
		std::vector<Map::Decor> props;

		props = GenerateHospital(_chunck.cellList[cell], _chunck.type);
		// Toujours g�n�rer et ajouter des lights
		//std::vector<Map::Decor> lights = GenerateLight(cell);
		//props.insert(props.end(), lights.begin(), lights.end());

		// D�cider une fois si des colonnes doivent �tre ajout�es � cette cellule
		//bool shouldAddColumns = (rand() % 100 < 50);  // 50% de chance de g�n�rer des colonnes

	//std::cout << "Colonne apparaisse si 1 sinon apparait pas si 0 : " << shouldAddColumns << std::endl;

		//if (shouldAddColumns == true)  // Si vrai, g�n�rer et ajouter toutes les colonnes
		//{
		//if (_random >= 50)
		//{
		//std::vector<Map::Decor> columns = GenerateColumn(cell);
		//props.insert(props.end(), columns.begin(), columns.end());
		//}
		//}

		_chunck.cellList[cell].props = props;
	}
}


/////SPAWN ROOM/////
std::vector<Map::Decor> Props::GenerateRoom(Map::Chunck& _chunk)
{
	////////////////////////////////////////////////// INIT Room ///////////////////////////////////////////////////////

	//scale les ballon x3 pour la playroom//

	std::vector<Map::Decor> roomList;

	Map::Decor room;

	room.pos = glm::vec3(_chunk.cellList[0].ground[0].pos.x - BRICK_W / 2, _chunk.cellList[0].ground[0].pos.y + BRICK_W + (BRICK_W / 5) - BRICK_W / 5, _chunk.cellList[0].ground[0].pos.z + BRICK_W/2);
	room.rot = glm::vec3(0, 0, 0);
	room.isVisible = true;
	room.name = _chunk.nameRoom  /*"../Sets/HOSPITAL/Map/HSP_PlayRoomDodu.json"*/;
	roomList.push_back(room);
	//std::cout << _chunk.txtRoom[randomRoom] << std::endl;
	//_chunk.txtRoom.erase(_chunk.txtRoom.begin() + randomRoom);

	return roomList;
}
