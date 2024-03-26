#include "Scene.h"
#include "Clock.h"
#include "Editor.h"
#include "Crosshair.h"
#include "Maze.h"
#include "Blocks.h"
#include "Inventory.h"
#include "Set.h"

static bool initialized = false;
static Colors::Color clearColor = Colors::LimedSpruce;

Colors::Color Scene::GetClearColor()
{
	return clearColor;
}

void Scene::SetClearColor(Colors::Color _color)
{
	clearColor = _color;
}

static Sets::Set* set = nullptr;
void Scene::Initialize(GameData* _gameData)
{
	Blocks::Initialize();

	Blocks::BlockType* blType[3] = { nullptr };
	std::vector<Blocks::BlockType*> types = Blocks::GetAll();
	for (size_t i = 0; i < types.size(); i++)
	{
		if (types[i]->GetName() == "HSP_S1")
		{
			blType[0] = types[i];
		}
		else if (types[i]->GetName() == "HSP_W1")
		{
			blType[1] = types[i];
		}
		else if (types[i]->GetName() == "HSP_W2")
		{
			blType[2] = types[i];
		}
	}

	//Maze::Generate();
	///*Sets::Set**/ set = Sets::Create();
	//set->GenerateRenderingInstance();

	//for (size_t rowId = 0; rowId < 50; rowId++)
	//{
	//	for (size_t columnId = 0; columnId < 50; columnId++)
	//	{
	//		Blocks::Block block;
	//		block.GenerateModel();
	//		block.SetType(blType[0]);
	//		glm::vec3 scale = block.GetType()->GetScale();
	//		block.SetScale(scale);
	//		block.SetPosition(glm::vec3(scale.x * rowId, .0f, scale.z * columnId));
	//		set->InsertBlock(block, false);
	//		if (!rowId || rowId == 49)
	//		{
	//			for (size_t heightId = 1; heightId < 20; heightId++)
	//			{
	//				Blocks::Block block;
	//				block.GenerateModel();
	//				block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//				glm::vec3 scale = block.GetType()->GetScale();
	//				block.SetScale(scale);
	//				block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//				set->InsertBlock(block, false);
	//			}
	//		}
	//		else if (columnId == 49)
	//		{
	//			for (size_t heightId = 1; heightId < 20; heightId++)
	//			{
	//				Blocks::Block block;
	//				block.GenerateModel();
	//				block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//				glm::vec3 scale = block.GetType()->GetScale();
	//				block.SetScale(scale);
	//				block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//				set->InsertBlock(block, false);
	//			}
	//		}
	//	}
	//}
	//set->SetName("Wall");
	//set->SetPath("../Sets/");
	//set->CalculateBoundingBox();

	initialized = true;
}

void Scene::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	

	_gameData->window.Clear(clearColor);

	Scene::World::Render(_gameData);
}

void Scene::CleanUp()
{
}

