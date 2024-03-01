#include "Scene.h"
#include "Clock.h"
#include "Editor.h"
#include "Crosshair.h"
#include "Maze.h"
#include "Blocks.h"
#include "Inventory.h"
#include "Set.h"

static bool initialized = false;
static unsigned int FPVCam;
static Scene::Type type = Scene::Type::MODEL_EDITOR;
static Clock inputClock;
static Colors::Color clearColor = Colors::LimedSpruce;
static Lightning::Light* flashLight;


Colors::Color Scene::GetClearColor()
{
	return clearColor;
}

void Scene::SetClearColor(Colors::Color _color)
{
	clearColor = _color;
}
std::vector<int> ints;
std::vector<int> ints2;
std::vector<int> ints3;

void Scene::Initialize(GameData* _gameData)
{
	inputClock.Restart();
	FPVCam = Scene::World::NewCamera(glm::vec3(0.0f, 0.0f, 1.0f));
	Scene::World::FocusCamera(_gameData, FPVCam);

	Lightning::Light flashLight2;
	flashLight2.SetType(Lightning::LightType::SPOT);
	flashLight2.SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
	flashLight2.SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	flashLight2.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	flashLight2.SetConstant(1.0f);
	flashLight2.SetLinear(0.0f);
	flashLight2.SetQuadratic(0.0f);

	flashLight2.SetCutOff(90.5f);
	flashLight2.SetOuterCutOff(90.5f);
	flashLight2.SetName("FlashLight");
	flashLight2.SetActive(true);
	Lights::InsertLight(_gameData, flashLight2);

	Lightning::Light dirLight;
	dirLight.SetType(Lightning::LightType::DIRECTIONAL);
	dirLight.SetDirection(glm::vec3(-0.2f, -1.0f, -0.3f));
	dirLight.SetAmbient(glm::vec3(0.05f, 0.05f, 0.05f));
	dirLight.SetDiffuse(glm::vec3(0.4f, 0.4f, 0.4f));
	dirLight.SetSpecular(glm::vec3(0.5f, 0.5f, 0.5f));
	dirLight.SetName("Directional Light");
	Lights::InsertLight(_gameData, dirLight);
	Scene::Lights::UpdateShader(_gameData);

	Blocks::Initialize();
	//Maze::Generate();

	Blocks::BlockType* blType[3] = { nullptr };
	std::vector<Blocks::BlockType*> types = Blocks::GetAll();
	for (size_t i = 0; i < types.size(); i++)
	{
		if (types[i]->GetName() == "Sol_CarelageMagasin")
		{
			blType[0] = types[i];
		}
		else if (types[i]->GetName() == "Mur_Hopital1")
		{
			blType[1] = types[i];
		}
		else if (types[i]->GetName() == "Mur_Hopital2")
		{
			blType[2] = types[i];
		}
	}


	//Sets::Set* set = Sets::Create();
	//set->GenerateRenderingInstance();
	/*set->LoadFromJson(json::parse(Files::GetFileContent("../Sets/Wall.json")));*/

	const int meters = 50;
	const int dimensions = meters / 0.2f;

	for (int i = 0; i < (int)meters / 4; i++)
	{
		int value = 4 * i / 0.2f;
		ints.push_back(value);
	};

	for (int i = 0; i < (int)meters / 15; i++)
	{
		int value = 15 * i / 0.2f;
		for (size_t j = 0; j < 4 / 0.2f; j++)
		{
			ints2.push_back(value + j);
		}
		if (i != 0 && !ints3.size())
		{
			ints3.push_back(value);
		}
	};

	//for (size_t rowId = 0; rowId < dimensions; rowId++)
	//{
	//	for (size_t columnId = 0; columnId < dimensions; columnId++)
	//	{
	//		{
	//			Blocks::Block block;
	//			block.GenerateModel();
	//			block.SetType(blType[0]);
	//			glm::vec3 scale = /*block.GetType()->GetScale()*/glm::vec3(0.2f);
	//			block.SetScale(scale);
	//			block.SetPosition(glm::vec3(scale.x * rowId, .0f, scale.z * columnId));
	//			set->InsertBlock(block, false);
	//		}

	//		{
	//			Blocks::Block block;
	//			block.GenerateModel();
	//			block.SetType(blType[0]);
	//			glm::vec3 scale = /*block.GetType()->GetScale()*/glm::vec3(0.2f);
	//			block.SetScale(scale);
	//			block.SetPosition(glm::vec3(scale.x * rowId, 20 * 0.2f, scale.z * columnId));
	//			set->InsertBlock(block, false);
	//		}

	//		for (size_t i = 0; i < ints.size(); i++)
	//		{
	//			if (rowId == ints[i])
	//			{
	//				bool found = false;
	//				for (size_t j = 0; j < ints2.size(); j++)
	//				{
	//					if (columnId == ints2[j])
	//					{
	//						found = true;
	//						break;
	//					}
	//				}
	//				if (found)
	//				{
	//					continue;
	//				}
	//				for (size_t heightId = 1; heightId < 20; heightId++)
	//				{
	//					Blocks::Block block;
	//					block.GenerateModel();
	//					block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//					glm::vec3 scale = block.GetType()->GetScale();
	//					block.SetScale(scale);
	//					block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//					set->InsertBlock(block, false);
	//				}
	//			}
	//			else
	//			{
	//				for (size_t k = 0; k < ints3.size(); k++)
	//				{
	//					if (columnId == ints3[0] || columnId == dimensions -1)
	//					{
	//						//bool found = false;
	//						//for (size_t j = 0; j < ints2.size(); j++)
	//						//{
	//						//	if (columnId == ints2[j])
	//						//	{
	//						//		found = true;
	//						//		break;
	//						//	}
	//						//}
	//						//if (found)
	//						//{
	//						//	continue;
	//						//}
	//						for (size_t heightId = 1; heightId < 20; heightId++)
	//						{
	//							Blocks::Block block;
	//							block.GenerateModel();
	//							block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//							glm::vec3 scale = block.GetType()->GetScale();
	//							block.SetScale(scale);
	//							block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//							set->InsertBlock(block, false);
	//						}
	//					}
	//				}
	//			}
	//		}
	//	}
	//}
	//for (size_t rowId = 0; rowId < 50; rowId++)
	//{
	//	for (size_t columnId = 0; columnId < 50; columnId++)
	//	{
	//		Blocks::Block block;
	//		block.GenerateModel();
	//		//Blocks::MaterialCheck(&block, "Sol_CarelageMagasin");
	//		block.SetType(blType[0]);
	//		glm::vec3 scale = block.GetType()->GetScale();
	//		block.SetScale(scale);
	//		block.SetPosition(glm::vec3(scale.x *  rowId, .0f, scale.z * columnId));
	//		set->InsertBlock(block);
	//		if (!rowId || rowId == 49)
	//		{
	//			for (size_t heightId = 1; heightId < 20; heightId++)
	//			{
	//				Blocks::Block block;
	//				block.GenerateModel();
	//				block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//				//Blocks::MaterialCheck(&block, (heightId < 7) ? "Mur_Hopital1" : "Mur_Hopital2");
	//				glm::vec3 scale = block.GetType()->GetScale();
	//				block.SetScale(scale);
	//				block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//				set->InsertBlock(block);
	//			}
	//		}
	//		else if (columnId == 49)
	//		{
	//			for (size_t heightId = 1; heightId < 20; heightId++)
	//			{
	//				Blocks::Block block;
	//				block.GenerateModel();
	//				block.SetType((heightId < 7) ? blType[1] : blType[2]);
	//				//Blocks::MaterialCheck(&block, (heightId < 7) ? "Mur_Hopital1" : "Mur_Hopital2");
	//				glm::vec3 scale = block.GetType()->GetScale();
	//				block.SetScale(scale);
	//				block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
	//				set->InsertBlock(block);
	//			}
	//		}
	//	}
	//}
	//set->SetName("Wall");
	//set->SetPath("../Sets/");

	initialized = true;
}

void Scene::Inputs(GameData* _gameData)
{
	if ((!Editor::IsDisplayed() && !Inventory::IsActive()) || _gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_MIDDLE))
	{
		if (!_gameData->window.IsFocused())
		{
			_gameData->window.Focus(true);
		}
		World::MouseInputs(_gameData);
		if (_gameData->window.IsKeyPressed(Keys::W))
		{
			World::ProcessCameraInput(_gameData, FPVCam, FORWARD);
		}
		if (_gameData->window.IsKeyPressed(Keys::S))
		{
			World::ProcessCameraInput(_gameData, FPVCam, BACKWARD);
		}
		if (_gameData->window.IsKeyPressed(Keys::A))
		{
			World::ProcessCameraInput(_gameData, FPVCam, LEFT);
		}
		if (_gameData->window.IsKeyPressed(Keys::D))
		{
			World::ProcessCameraInput(_gameData, FPVCam, RIGHT);
		}
		if (_gameData->window.IsKeyPressed(Keys::SPACE))
		{
			World::ProcessCameraInput(_gameData, FPVCam, UP);
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
		{
			World::ProcessCameraInput(_gameData, FPVCam, DOWN);
		}
	}
	else
	{
		if ((Editor::IsDisplayed() || Inventory::IsActive()) && _gameData->window.IsFocused())
		{
			_gameData->window.Focus(false);
		}
	}
}

void Scene::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	Inputs(_gameData);

	_gameData->window.Clear(clearColor);
	Scene::World::Render(_gameData);
	// Needs to be called after the inputs that enables it
	//if (!_gameData->window.IsFocused())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Editor::Tick(_gameData);

		if (!Editor::IsDisplayed())
		{
			Inventory::Menu(_gameData);
			if (!Inventory::IsActive())
			{
				Sets::Edition(_gameData);
			}
		}
		ImGui::Render();
	}

	std::vector<Lightning::Light>* lights = Lights::GetLights();
	for (size_t i = 0; i < lights->size(); i++)
	{
		if (lights->at(i).GetName() == "FlashLight")
		{
			Camera* cam = World::GetCamera();
			flashLight = &lights->at(i);
			flashLight->SetPosition(cam->Position);
			flashLight->SetDirection(cam->Front);
			Lights::UpdateShader(_gameData);
		}
	}
	glDisable(GL_DEPTH_TEST);
	Crosshairs::Draw();

	glEnable(GL_DEPTH_TEST);
	//if (!_gameData->window.IsFocused())
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	_gameData->window.Events();
}

void Scene::CleanUp()
{
}

