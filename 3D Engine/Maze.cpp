#include "Maze.h"
#include "Set.h"

std::vector<Maze::MegaChunck> maze;

void Maze::Generate()
{
	/////////////////////////////////////////////////////// INIT TEXT ////////////////////////////////////////////////////////
	ManagmentText text;
	text.hallRoomList.push_back("Room");
	text.hallRoomList.push_back("Room");
	text.hallRoomList.push_back("Room");
	text.hallRoomList.push_back("Room");

	text.fieldsRoomList.push_back("Room");
	text.fieldsRoomList.push_back("Room");
	text.fieldsRoomList.push_back("Room");
	text.fieldsRoomList.push_back("Room");

	text.hospitalRoomList.push_back("Room");
	text.hospitalRoomList.push_back("Room");
	text.hospitalRoomList.push_back("Room");
	text.hospitalRoomList.push_back("Room");

	text.laboRoomList.push_back("Room");
	text.laboRoomList.push_back("Room");
	text.laboRoomList.push_back("Room");
	text.laboRoomList.push_back("Room");

	text.exitRoomList.push_back("Room");
	text.exitRoomList.push_back("Room");
	text.exitRoomList.push_back("Room");
	text.exitRoomList.push_back("Room");

	text.hallCellList.push_back("Wall");
	text.hallCellList.push_back("Ground");
	text.hallCellList.push_back("WallRot");
	text.hallCellList.push_back("Roof");
	text.hallCellList.push_back("Wall");
	text.hallCellList.push_back("WallRot");

	text.laboCellList.push_back("Wall");
	text.laboCellList.push_back("Ground");
	text.laboCellList.push_back("WallRot");
	text.laboCellList.push_back("Roof");
	text.laboCellList.push_back("Wall");
	text.laboCellList.push_back("WallRot");

	text.hospitalCellList.push_back("Wall");
	text.hospitalCellList.push_back("Ground");
	text.hospitalCellList.push_back("WallRot");
	text.hospitalCellList.push_back("Roof");
	text.hospitalCellList.push_back("Wall");
	text.hospitalCellList.push_back("WallRot");

	text.fieldsCellList.push_back("Wall");
	text.fieldsCellList.push_back("Ground");
	text.fieldsCellList.push_back("WallRot");
	text.fieldsCellList.push_back("Roof");
	text.fieldsCellList.push_back("Wall");
	text.fieldsCellList.push_back("WallRot");

	for (int mapLine = 0; mapLine < NB_MAP_H; mapLine++)
	{
		for (int mapColumn = 0; mapColumn < NB_MAP_H; mapColumn++)
		{
			MegaChunck map;
			for (int stageNb = 0; stageNb < NB_STAGE_H; stageNb++)
			{
				Stage stage;
				/////////////////////////////////////////////////////// STAGE MANAGMENT ////////////////////////////////////////////////////////
				Chunck chunk;

				int randomBalcony = 0;
				randomBalcony = rand() % 100;
				int randomBridge = 0;
				randomBridge = rand() % 100;

				if (stageNb == FACTORY)
				{
					chunk.type = LABY;
					stage.sizeOf = CELL_W;
				}
				else if (stageNb == LABO)
				{
					chunk.type = LABY;
					stage.sizeOf = CELL_W;
				}
				else if (stageNb == FIELDS)
				{
					chunk.type = PARCEL;
					stage.sizeOf = CELL_W;
				}
				else if (stageNb == HOSPITALS)
				{
					chunk.type = HOSPITAL;
					stage.sizeOf = CELL_W;
				}
				//chunk.type = PARCEL;
				if (stageNb != 0 && mapLine == 0 && randomBalcony >= BUILDING_LUCK)
				{
					chunk.type = BALCONY;
				}
				if (NB_STAGE_H - stageNb >= 0 && mapColumn + mapLine * NB_MAP_H - NB_MAP_H >= 0)
				{
					if (maze[mapColumn + mapLine * NB_MAP_H - NB_MAP_H].stageList[stageNb].chunk.type == BALCONY && mapLine - 1 == 0 || mapLine - 1 != 0)
						if (mapLine == 4 - stageNb && mapColumn == 1)
						{
							chunk.type = GARDEN; // !!!!!!!!!!!!!!!!!!!!!
						}
				}
				if (stageNb == 0 && mapLine == 3 && mapColumn == 4)
				{
					chunk.type = EXIT;
				}
				if (stageNb == 1 && mapLine == 4 && mapColumn == 2)
				{
					chunk.type = EXIT;
				}
				if (stageNb == 2 && mapLine == 3 && mapColumn == 3)
				{
					chunk.type = EXIT;
				}

				for (int line = 0; line < NB_CELL; line++)
				{
					for (int column = 0; column < NB_CELL; column++)
					{
						/////////////////////////////////////////////////////// INIT GROUND //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						Cell cell;
						Decor ground;

						ground.decor = Sets::Create();
						ground.decor->GenerateRenderingInstance();
						ground.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
						ground.decor->SetName("Wall");
						ground.decor->SetPath("../Sets/");
						ground.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W + mapColumn * MAP_W, 0 + CELL_W * stageNb, CELL_W * line + BRICK_W + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
						ground.isVisible = true;
						if (chunk.type == GARDEN && stageNb != 0)
						{
							ground.isVisible = false;
							if (randomBridge < 40)
							{
								if (column == NB_CELL - 1)
								{
									ground.isVisible = true;
									if (line == 0)
									{
										Decor balconyBridge;

										balconyBridge.decor = Sets::Create();
										balconyBridge.decor->GenerateRenderingInstance();
										balconyBridge.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
										balconyBridge.decor->SetName("Wall");
										balconyBridge.decor->SetPath("../Sets/");
										balconyBridge.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W - CELL_W, CELL_W + CELL_W * stageNb - CELL_W + BRICK_W, CELL_W * line + BRICK_W * 2 + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
										balconyBridge.isVisible = true;
										chunk.decor.push_back(balconyBridge);
									}
								}
							}
						}
						cell.ground.push_back(ground);

						/////////////////////////////////////////////////////// INIT ROOF //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type != GARDEN && chunk.type != EXIT && chunk.type != BALCONY)
						{
							Decor roof;

							roof.decor = Sets::Create();
							roof.decor->GenerateRenderingInstance();
							roof.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
							roof.decor->SetName("Wall");
							roof.decor->SetPath("../Sets/");
							roof.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W + mapColumn * MAP_W, CELL_W /*/ 2*/ + CELL_W * stageNb, CELL_W * line + BRICK_W + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
							roof.isVisible = true;
							cell.ground.push_back(roof);
						}

						/////////////////////////////////////////////////////// INIT PROPS BUILDING //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type != GARDEN && chunk.type != BALCONY)
						{
							if (mapLine == 0 && stageNb < NB_STAGE_H - 1 || stageNb == NB_STAGE_H - 1)
							{
								if (column == NB_CELL - 1 && line == 0)
								{
									Decor tower;

									tower.decor = Sets::Create();
									tower.decor->GenerateRenderingInstance();
									tower.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
									tower.decor->SetName("Wall");
									tower.decor->SetPath("../Sets/");
									tower.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W + mapColumn * MAP_W, CELL_W /*/ 2*/ + CELL_W * stageNb, CELL_W * line + BRICK_W * 2 + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
									tower.isVisible = true;
									chunk.decor.push_back(tower);
								}
							}
						}

						/////////////////////////////////////////////////////// INIT EXIT //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type == EXIT && column == NB_CELL - 1 && line == 0)
						{
							Decor exit;
							exit.decor = Sets::Create();
							exit.decor->GenerateRenderingInstance();
							exit.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
							exit.decor->SetName("Wall");
							exit.decor->SetPath("../Sets/");
							exit.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W, CELL_W + CELL_W * stageNb - CELL_W, CELL_W * line + BRICK_W * 2 + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
							exit.isVisible = true;
							chunk.decor.push_back(exit);
						}

						/////////////////////////////////////////////////////// INIT BALCONY //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type == BALCONY && column == NB_CELL - 1 && line == 0)
						{
							Decor balcony;

							balcony.decor = Sets::Create();
							balcony.decor->GenerateRenderingInstance();
							balcony.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
							balcony.decor->SetName("Wall");
							balcony.decor->SetPath("../Sets/");
							balcony.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W, CELL_W + CELL_W * stageNb - CELL_W + BRICK_W, CELL_W * line + BRICK_W * 2 + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
							balcony.isVisible = true;
							chunk.decor.push_back(balcony);
						}

						/////////////////////////////////////////////////////// INIT WALL LABY ///////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type == LABY || chunk.type == GARDEN || chunk.type == BALCONY || chunk.type == HOSPITAL || chunk.type == PARCEL)
						{
							for (int i = 0; i < 2; i++)
							{
								Decor wall;

								if (i == BOTTOM) //BOTTOM WALL
								{
									wall.decor = Sets::Create();
									wall.decor->GenerateRenderingInstance();
									wall.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
									wall.decor->SetName("Wall");
									wall.decor->SetPath("../Sets/");
									wall.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W, BRICK_W + CELL_W * stageNb, CELL_W * line + BRICK_W + BRICK_W + CELL_W + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
								}
								else if (i == LEFT) //LEFT WALL
								{
									wall.decor = Sets::Create();
									wall.decor->GenerateRenderingInstance();
									wall.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
									wall.decor->SetName("Wall");
									wall.decor->SetPath("../Sets/");
									wall.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W, BRICK_W + CELL_W * stageNb, CELL_W * line + BRICK_W + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
								}
								wall.isVisible = true;
								/////////////////////////////////////////////////////// INIT WALL PRESET ///////////////////////////////////////////////////////////////////////////////////////////////////////////
								if (chunk.type == GARDEN || chunk.type == BALCONY || chunk.type == PARCEL) //GARDEN PRESET
								{
									wall.isVisible = true;
								}
								cell.wallList.push_back(wall);
							}

							/////////////////////////////////////////////////////// INIT WALL BORDER /////////////////////////////////////////////////////////////////////////////////////////////////////
							if (chunk.type == LABY || chunk.type == HOSPITAL || chunk.type == PARCEL) //LABY BORDER
							{
								Decor wallMissing;

								if (mapLine == 0 && line == 0) //BOTTOM WALL
								{
									wallMissing.decor = Sets::Create();
									wallMissing.decor->GenerateRenderingInstance();
									wallMissing.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
									wallMissing.decor->SetName("Wall");
									wallMissing.decor->SetPath("../Sets/");
									wallMissing.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 + mapColumn * MAP_W, BRICK_W + CELL_W * stageNb, 2.5 * line + BRICK_W * 2 + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
								}
								if (mapColumn == 0 && column == 0) //LEFT WALL
								{
									wallMissing.decor = Sets::Create();
									wallMissing.decor->GenerateRenderingInstance();
									wallMissing.decor->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));
									wallMissing.decor->SetName("Wall");
									wallMissing.decor->SetPath("../Sets/");
									wallMissing.decor->SetPosition(glm::vec3(CELL_W * column + BRICK_W / 2 - CELL_W + mapColumn * MAP_W, BRICK_W + CELL_W * stageNb, 2.5 * line + BRICK_W + mapLine * MAP_W + (OFFSET_STAGE * MAP_W * stageNb)));
								}

								cell.wallMissingList.push_back(wallMissing);
							}
							//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

							cell.isVisited = false;

							chunk.cellList.push_back(cell);
						}
					}
				}

				/////////////////////////////////////////////////////// LABY CREATION ///////////////////////////////////////////////////////
				if (chunk.type == LABY)
				{
					int i = 0;
					int iTemp = i;
					int randTemp = 0;
					int wallEraseCount = 0;
					std::vector<int> temp;
					chunk.cellList[i].isVisited = true;

					while (wallEraseCount < NB_CELL * NB_CELL - 1)
					{
						if (chunk.cellList[i].isVisited)
						{
							if (i - NB_CELL >= 0)
							{
								if (!chunk.cellList[i - NB_CELL].isVisited)
								{
									temp.push_back(UP);
								}
							}

							if (i + NB_CELL < NB_CELL * NB_CELL)
							{
								if (!chunk.cellList[i + NB_CELL].isVisited)
								{
									temp.push_back(BOTTOM);
								}
							}

							if (i + 1 < NB_CELL * NB_CELL && (i + 1) % NB_CELL != 0)
							{
								if (!chunk.cellList[i + 1].isVisited)
								{
									temp.push_back(LEFT);
								}
							}

							if (i - 1 >= 0 && (i % NB_CELL) != 0 || i == NB_CELL * NB_CELL)
							{
								if (!chunk.cellList[i - 1].isVisited)
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
									chunk.cellList[i + NB_CELL].isVisited = true;
									chunk.cellList[i].wallList[BOTTOM].isVisible = false;
									i = i + NB_CELL;
									chunk.cellList[i].lastCase = -NB_CELL;
								}
								if (temp[randTemp] == LEFT)
								{
									chunk.cellList[i + 1].isVisited = true;
									chunk.cellList[i].wallList[LEFT].isVisible = false;
									i = i + 1;
									chunk.cellList[i].lastCase = -1;
								}
								if (temp[randTemp] == RIGHT)
								{
									chunk.cellList[i - 1].isVisited = true;
									chunk.cellList[i - 1].wallList[LEFT].isVisible = false;
									i = i - 1;
									chunk.cellList[i].lastCase = 1;
								}
								if (temp[randTemp] == UP)
								{
									chunk.cellList[i - NB_CELL].isVisited = true;
									chunk.cellList[i - NB_CELL].wallList[BOTTOM].isVisible = false;
									i = i - NB_CELL;
									chunk.cellList[i].lastCase = NB_CELL;
								}
								wallEraseCount++;
							}
							else
							{
								i += chunk.cellList[i].lastCase;
							}
							temp.clear();
						}
					}
					/////////////////////////////////////////////////////// DENSITY ///////////////////////////////////////////////////////

					for (int id = 0; id < chunk.cellList.size(); id++)
					{
						for (int shortcut = 0; shortcut < chunk.cellList[id].wallList.size(); shortcut++)
						{
							int randomLuck = rand() % 100;
							if (randomLuck > SHORTCUT_LUCK)
							{
								//if (id % NB_CELL != 0 || id <= NB_CELL * NB_CELL - NB_CELL - 1)
								//{
								chunk.cellList[id].wallList[shortcut].isVisible = false;
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
				stage.chunk = chunk;
				map.stageList.push_back(stage);
			}
			maze.push_back(map);
		}
	}
	/////////////////////////////////////////////////////// HOSPITAL CREATION ///////////////////////////////////////////////////////
	for (int mapNb = 0; mapNb < maze.size(); mapNb++)
	{
		for (int stageNb = 0; stageNb < maze[mapNb].stageList.size(); stageNb++)
		{
			if (maze[mapNb].stageList[stageNb].chunk.type == HOSPITAL)
			{
				for (int line = 0; line < NB_CELL; line++)
				{
					for (int column = 0; column < NB_CELL; column++)
					{
						if ((mapNb + 1) % NB_MAP_H != 0 && maze[mapNb].stageList.size() == maze[mapNb + 1].stageList.size())
						{
							if (maze[mapNb + 1].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb + 1].stageList[stageNb].chunk.type == EXIT) // LEFT CORRDIOR ->
							{
								if (line == ((int)(NB_CELL / 2)))
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL].wallList[LEFT].isVisible = false;
								}
							}
						}
						if (mapNb + NB_MAP_H < NB_MAP_H * NB_MAP_H && maze[mapNb].stageList.size() == maze[mapNb + NB_MAP_H].stageList.size())
						{
							if (maze[mapNb + NB_MAP_H].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb + NB_MAP_H].stageList[stageNb].chunk.type == EXIT) //BOT CORRIDOR ->
							{
								if (column == (int)(NB_CELL / 2))
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL].wallList[BOTTOM].isVisible = false;
								}
							}
						}
						if (mapNb % NB_MAP_H != 0 && maze[mapNb].stageList.size() == maze[mapNb - 1].stageList.size())
						{
							if (maze[mapNb - 1].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb - 1].stageList[stageNb].chunk.type == EXIT) //BOT CORRIDOR <-
							{
								if (line == (int)(NB_CELL / 2))
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL].wallList[LEFT].isVisible = false;
								}
							}
						}
						if (mapNb - NB_MAP_H >= 0 && maze[mapNb].stageList.size() == maze[mapNb - NB_MAP_H].stageList.size())
						{
							if (maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type == EXIT) // LEFT CORRDIOR <-
							{
								if (column == (int)(NB_CELL / 2))
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL].wallList[BOTTOM].isVisible = false;
								}
							}
						}
						for (int wallNb = 0; wallNb < 2; wallNb++)
						{
							if (column + line * NB_CELL == 0 || column == (int)(NB_CELL / 2) + 1 && line == (int)(NB_CELL / 2) + 1 || column == 0 && line == (int)(NB_CELL / 2) + 1 || column == (int)(NB_CELL / 2) + 1 && line == 0)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL].wallList[wallNb].isVisible = false;
								maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL + NB_CELL].wallList[LEFT].isVisible = false;
								maze[mapNb].stageList[stageNb].chunk.cellList[column + line * NB_CELL + 1].wallList[BOTTOM].isVisible = false;
							}
						}
					}
				}
			}

			/////////////////////////////////////////////////////// GARDEN BORDER ///////////////////////////////////////////////////////
			for (int cell = 0; cell < maze[mapNb].stageList[stageNb].chunk.cellList.size(); cell++)
			{
				if (maze[mapNb].stageList[stageNb].chunk.type == LABY || maze[mapNb].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb].stageList[stageNb].chunk.type == PARCEL)
				{
					if (mapNb + 1 < NB_MAP_H * NB_MAP_H && maze[mapNb].stageList.size() == maze[mapNb + 1].stageList.size())
					{
						if ((mapNb + 1) % NB_MAP_H == 0 || maze[mapNb + 1].stageList[stageNb].chunk.type == GARDEN) // RIGHT BORDER
						{
							if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
							}
						}
					}
					if (mapNb + NB_MAP_H < NB_MAP_H * NB_MAP_H && maze[mapNb].stageList.size() == maze[mapNb + NB_MAP_H].stageList.size())
					{
						if (mapNb > NB_MAP_H * NB_MAP_H - NB_MAP_H - 1 || maze[mapNb + NB_MAP_H].stageList[stageNb].chunk.type == GARDEN) // TOP BORDER
						{
							if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
							}
						}
					}
					if (mapNb > 0 && maze[mapNb].stageList.size() == maze[mapNb - 1].stageList.size())
					{
						if (maze[mapNb - 1].stageList[stageNb].chunk.type == GARDEN && mapNb % NB_MAP_H != 0) // RIGHT BORDER
						{
							if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
							{
								maze[mapNb - 1].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
							}
						}
					}
					if (mapNb - NB_MAP_H >= 0 && maze[mapNb].stageList.size() == maze[mapNb - NB_MAP_H].stageList.size())
					{
						if (maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type == GARDEN) // TOP BORDER
						{
							if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
							{
								maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
							}
						}
					}
					/////////////////////////////////////////////////////// BALCONY BORDER ///////////////////////////////////////////////////////
					if (maze[mapNb].stageList[stageNb].chunk.type != BALCONY)
					{
						if (mapNb + 1 < NB_MAP_H * NB_MAP_H && maze[mapNb].stageList.size() == maze[mapNb + 1].stageList.size())
						{
							if (maze[mapNb + 1].stageList[stageNb].chunk.type == BALCONY) // RIGHT BORDER
							{
								if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
								}
							}
						}
						if (mapNb + NB_MAP_H < NB_MAP_H * NB_MAP_H && maze[mapNb].stageList.size() == maze[mapNb + NB_MAP_H].stageList.size())
						{
							if (maze[mapNb + NB_MAP_H].stageList[stageNb].chunk.type == BALCONY) // TOP BORDER
							{
								if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
								{
									maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
								}
							}
						}
						if (mapNb > 0 && maze[mapNb].stageList.size() == maze[mapNb - 1].stageList.size())
						{
							if (maze[mapNb - 1].stageList[stageNb].chunk.type == BALCONY && mapNb % NB_MAP_H != 0) // RIGHT BORDER
							{
								if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
								{
									maze[mapNb - 1].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
								}
							}
						}
						if (mapNb - NB_MAP_H >= 0 && maze[mapNb].stageList.size() == maze[mapNb - NB_MAP_H].stageList.size())
						{
							if (maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.type == BALCONY) // TOP BORDER
							{
								if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
								{
									maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
								}
							}
						}
					}
				}

				/////////////////////////////////////////////////////// LEFT BOT BORDER MAP ///////////////////////////////////////////////////////
				if (maze[mapNb].stageList[stageNb].chunk.type == LABY || maze[mapNb].stageList[stageNb].chunk.type == HOSPITAL || maze[mapNb].stageList[stageNb].chunk.type == GARDEN || maze[mapNb].stageList[stageNb].chunk.type == PARCEL)
				{
					if ((mapNb + 1) % NB_MAP_H == 0) // RIGHT BORDER
					{
						if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
						{
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
						}
					}
					if (mapNb > NB_MAP_H * NB_MAP_H - NB_MAP_H - 1) // TOP BORDER
					{
						if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
						{
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
						}
					}

					if ((mapNb + 1) % NB_MAP_H == 0) // RIGHT BORDER
					{
						if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
						{
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = true;
						}
					}
					if (mapNb > NB_MAP_H * NB_MAP_H - NB_MAP_H - 1) // TOP BORDER
					{
						if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
						{
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = true;
						}
					}
					/////////////////////////////////////////////////////// EXIT BORDER ///////////////////////////////////////////////////////
					if (stageNb > 0 && mapNb + NB_MAP_H < NB_MAP_H * NB_MAP_H)
					{
						if (maze[mapNb + NB_MAP_H].stageList[stageNb - 1].chunk.type == EXIT) // Stage +1 Border
						{
							//maze[mapNb].stageList[stageNb].chunk.type == ROOM;
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = false;
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = false;
							//maze[mapNb].stageList[stageNb].chunk.cellList[cell].roof->SetVisible(false);
							maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[BOTTOM].isVisible = false;
							if (mapNb - NB_MAP_H >= 0 && cell >= NB_CELL * NB_CELL - NB_CELL && cell < NB_CELL * NB_CELL)
							{
								maze[mapNb - NB_MAP_H].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = false;
							}
							if (NB_MAP_H * NB_MAP_H % mapNb != 0 && (cell + 1) % NB_CELL == 0)
							{
								maze[mapNb - 1].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = false;
							}

							if (cell % NB_CELL == 0 && mapNb % NB_MAP_H == 0)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[0].isVisible = false;
							}
							if (cell < NB_CELL && mapNb < NB_MAP_H)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[0].isVisible = false;
							}
						}
					}
					if ((mapNb + 1) % NB_MAP_H != 0 && mapNb + 1 < NB_MAP_H * NB_MAP_H) // Right Border
					{
						if (maze[mapNb + 1].stageList[stageNb].chunk.type == EXIT)
						{
							if ((cell + 1) % NB_CELL == 0)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[LEFT].isVisible = false;
							}
						}
					}
					if (mapNb + NB_MAP_H < NB_MAP_H * NB_MAP_H)
					{
						if (maze[mapNb + NB_MAP_H].stageList[stageNb].chunk.type == EXIT) // Top Border
						{
							if (cell >= NB_CELL * NB_CELL - NB_CELL)
							{
								maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[BOTTOM].isVisible = false;
							}
						}
					}
				}
			}
		}
	}

	for (int mapNb = 0; mapNb < maze.size(); mapNb++)
	{
		for (int stageNb = 0; stageNb < maze[mapNb].stageList.size(); stageNb++)
		{
			for (int decor = 0; decor < maze[mapNb].stageList[stageNb].chunk.decor.size(); decor++)
			{
				if (!maze[mapNb].stageList[stageNb].chunk.decor[decor].isVisible)
				{
					Sets::Erase(maze[mapNb].stageList[stageNb].chunk.decor[decor].decor);
				}
			}
			for (int cell = 0; cell < maze[mapNb].stageList[stageNb].chunk.cellList.size(); cell++)
			{
				for (int wall = 0; wall < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList.size(); wall++)
				{
					if (!maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].isVisible)
					{
						Sets::Erase(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallList[wall].decor);
					}
				}
				for (int wallMissing = 0; wallMissing < maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList.size(); wallMissing++)
				{
					if (!maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].isVisible)
					{
						Sets::Erase(maze[mapNb].stageList[stageNb].chunk.cellList[cell].wallMissingList[wallMissing].decor);
					}
				}
				for (int ground = 0; ground < maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground.size(); ground++)
				{
					if (!maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].isVisible)
					{
						Sets::Erase(maze[mapNb].stageList[stageNb].chunk.cellList[cell].ground[ground].decor);
					}
				}
			}
		}
	}
}

