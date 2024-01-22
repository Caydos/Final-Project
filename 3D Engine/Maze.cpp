#include "Maze.h"
#include "Decor.h"

//std::vector<Maze::Chunck> maze;
std::vector<Maze::Stage> maze;

void Maze::Generate()
{
	for (int stageNb = 0; stageNb < NB_STAGE_H; stageNb++)
	{
		Stage stage;
		for (int chunkLine = 0; chunkLine < NB_CHUNK_H; chunkLine++)
		{
			for (int chunkColumn = 0; chunkColumn < NB_CHUNK_H; chunkColumn++)
			{
				/////////////////////////////////////////////////////// CHUNK TYPE CHOICE ////////////////////////////////////////////////////////
				Chunck chunk;
				//if (chunkLine == 1 && chunkColumn == 1)
				//{
					chunk.type = HOSPITAL;
				//}
				/*else if (chunkLine == 0 || chunkColumn == 0 || chunkLine == NB_CHUNK_H - 1 || chunkColumn == NB_CHUNK_H - 1)
				{
					chunk.type = EXTERIOR;
				}  */
				//else if (chunkLine == 2 && chunkColumn == 2)
				//{
				//	chunk.type = GARDEN;
				//}
				//else
				//{
				//	chunk.type = LABY;
				//}

				for (int line = 0; line < NB_CELL; line++)
				{
					for (int column = 0; column < NB_CELL; column++)
					{
						/////////////////////////////////////////////////////// INIT GROUND //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						Cell cell;

						cell.ground = new Decors::Decor("Ground");
						cell.ground->LoadFromFile("Ground");
						Scene::World::InsertComponent(cell.ground);
						//cell.ground->ApplyOffset(glm::vec3(CELL_W * column + BRICK_W + chunkColumn * CHUNK_W, 0 + CELL_W * stageNb, CELL_W * line + BRICK_W + chunkLine * CHUNK_W));
						cell.ground->LoadInstances();
						cell.ground->SetInstanced(true);
						if (chunk.type == GARDEN && stageNb != 0 || chunk.type == EXTERIOR)
						{
							cell.ground->SetVisible(false);
						}

						/////////////////////////////////////////////////////// INIT ROOF //////////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type != GARDEN && chunk.type != EXTERIOR)
						{
							cell.roof = new Decors::Decor("Ground");
							cell.roof->LoadFromFile("Ground");
							Scene::World::InsertComponent(cell.roof);
							//cell.roof->ApplyOffset(glm::vec3(CELL_W * column + BRICK_W + chunkColumn * CHUNK_W, CELL_W / 2 + CELL_W * stageNb, CELL_W * line + BRICK_W + chunkLine * CHUNK_W));
							cell.roof->LoadInstances();
							cell.roof->SetInstanced(true);
						}



						/////////////////////////////////////////////////////// INIT WALL LABY ///////////////////////////////////////////////////////////////////////////////////////////////////////////
						if (chunk.type == LABY || chunk.type == GARDEN || chunk.type == HOSPITAL)
						{
							for (int i = 0; i < 2; i++)
							{
								Wall wall;

								if (i == BOTTOM) //BOTTOM WALL
								{
									wall.decor = new Decors::Decor("Wall");
									wall.decor->LoadFromFile("Wall");
									Scene::World::InsertComponent(wall.decor);
									//wall.decor->ApplyOffset(glm::vec3(CELL_W * column + BRICK_W / 2 + chunkColumn * CHUNK_W, BRICK_W + CELL_W * stageNb, CELL_W * line + BRICK_W + CELL_W + chunkLine * CHUNK_W));
									wall.decor->LoadInstances();
									wall.decor->SetInstanced(true);
								}
								else if (i == LEFT) //LEFT WALL
								{
									wall.decor = new Decors::Decor("WallRot");
									wall.decor->LoadFromFile("WallRot");
									Scene::World::InsertComponent(wall.decor);
									//wall.decor->ApplyOffset(glm::vec3(CELL_W * column + BRICK_W / 2 + chunkColumn * CHUNK_W, BRICK_W + CELL_W * stageNb, CELL_W * line + BRICK_W + chunkLine * CHUNK_W));
									wall.decor->LoadInstances();
									wall.decor->SetInstanced(true);
								}
								/////////////////////////////////////////////////////// INIT WALL PRESET ///////////////////////////////////////////////////////////////////////////////////////////////////////////
								if (chunk.type == LABY) //LABY PREST
								{
									wall.isVisible = true;
								}
								else if (chunk.type == GARDEN) //GARDEN PRESET
								{
									if (column == NB_CELL - 1 && i == LEFT && chunkColumn + 1 < NB_CHUNK_H) // LEFT BORDER
									{
										wall.isVisible = true;
										wall.decor->SetVisible(true);
									}
									else if (line == NB_CELL - 1 && i == BOTTOM && chunkLine + 1 < NB_CHUNK_H) // BOTTOM BORDER
									{
										wall.isVisible = true;
										wall.decor->SetVisible(true);
									}
									else // IN GARDEN
									{
										wall.isVisible = false;
										wall.decor->SetVisible(false);
									}
								}
								cell.wallList.push_back(wall);
							}

							/////////////////////////////////////////////////////// INIT WALL BORDER /////////////////////////////////////////////////////////////////////////////////////////////////////
							if (chunk.type == LABY || chunk.type == HOSPITAL ) //LABY BORDER
							{
								Wall wallMissing;

								if (line == 0 && chunkLine == 0) //BOTTOM WALL
								{
									wallMissing.decor = new Decors::Decor("Wall");
									wallMissing.decor->LoadFromFile("Wall");
									Scene::World::InsertComponent(wallMissing.decor);
									//wallMissing.decor->ApplyOffset(glm::vec3(2.5 * column + BRICK_W / 2 + chunkColumn * CHUNK_W, BRICK_W + CELL_W * stageNb, 2.5 * line + BRICK_W + chunkLine * CHUNK_W));
									wallMissing.decor->LoadInstances();
									wallMissing.decor->SetInstanced(true);
								}
								if (column == 0 && chunkColumn == 0)
								{
									wallMissing.decor = new Decors::Decor("WallRot"); //LEFT WALL
									wallMissing.decor->LoadFromFile("WallRot");
									Scene::World::InsertComponent(wallMissing.decor);
									//wallMissing.decor->ApplyOffset(glm::vec3(2.5 * column + BRICK_W / 2 + chunkColumn * CHUNK_W - 2.5, BRICK_W + CELL_W * stageNb, 2.5 * line + BRICK_W + chunkLine * CHUNK_W));
									wallMissing.decor->LoadInstances();
									wallMissing.decor->SetInstanced(true);
								}
								wallMissing.isVisible = true;

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
									chunk.cellList[i].wallList[BOTTOM].decor->SetVisible(false);
									i = i + NB_CELL;
									chunk.cellList[i].lastCase = -NB_CELL;
								}
								if (temp[randTemp] == LEFT)
								{
									chunk.cellList[i + 1].isVisited = true;
									chunk.cellList[i].wallList[LEFT].isVisible = false;
									chunk.cellList[i].wallList[LEFT].decor->SetVisible(false);
									i = i + 1;
									chunk.cellList[i].lastCase = -1;
								}
								if (temp[randTemp] == RIGHT)
								{
									chunk.cellList[i - 1].isVisited = true;
									chunk.cellList[i - 1].wallList[LEFT].isVisible = false;
									chunk.cellList[i - 1].wallList[LEFT].decor->SetVisible(false);
									i = i - 1;
									chunk.cellList[i].lastCase = 1;
								}
								if (temp[randTemp] == UP)
								{
									chunk.cellList[i - NB_CELL].isVisited = true;
									chunk.cellList[i - NB_CELL].wallList[BOTTOM].isVisible = false;
									chunk.cellList[i - NB_CELL].wallList[BOTTOM].decor->SetVisible(false);
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
							std::cout << randomLuck << std::endl;
							if (randomLuck > SHORTCUT_LUCK)
							{
								chunk.cellList[id].wallList[shortcut].isVisible = false;
								chunk.cellList[id].wallList[shortcut].decor->SetVisible(false);
							}
						}
					}
				}
				stage.chunkList.push_back(chunk);
			}
		}
		maze.push_back(stage);
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/////////////////////////////////////////////////////// GARDEN BORDER ///////////////////////////////////////////////////////

	for (int stageNb = 0; stageNb < NB_STAGE_H; stageNb++)
	{
		for (int chunk = 0; chunk < maze[stageNb].chunkList.size(); chunk++)
		{
			if (maze[stageNb].chunkList[chunk].type == LABY || maze[stageNb].chunkList[chunk].type == HOSPITAL)
			{
				for (int cell = 0; cell < maze[stageNb].chunkList[chunk].cellList.size(); cell++)
				{
					if (maze[stageNb].chunkList[chunk + 1].type == GARDEN && (chunk + 1) % NB_CHUNK_H != 0 && chunk + 1 < NB_CHUNK_H * NB_CHUNK_H || (chunk + 1) % NB_CHUNK_H == 0) // RIGHT BORDER
					{
						if ((cell + 1) % NB_CELL == 0 || cell == NB_CELL * NB_CELL - 1)
						{
							maze[stageNb].chunkList[chunk].cellList[cell].wallList[LEFT].decor->SetVisible(true);
							//maze[stageNb].chunkList[chunk].cellList[cell].wallList[LEFT].decor = new Decors::Decor("Window");
						}
					}
					if (maze[stageNb].chunkList[chunk + NB_CHUNK_H].type == GARDEN || chunk > NB_CHUNK_H*NB_CHUNK_H-NB_CHUNK_H-1) // TOP BORDER
					{
						if (cell > NB_CELL * NB_CELL - NB_CELL - 1)
						{
								maze[stageNb].chunkList[chunk].cellList[cell].wallList[BOTTOM].decor->SetVisible(true);
								//maze[stageNb].chunkList[chunk].cellList[cell].wallList[LEFT].decor->LoadFromFile("WallRot");
								//maze[stageNb].chunkList[chunk].cellList[cell].wallList[LEFT].decor->
						}
					}
				}
			}
		}
	}

	/////////////////////////////////////////////////////// HOSPITAL CREATION ///////////////////////////////////////////////////////

}
