#include "Tools.h"
#include "Lightning.h"
#include "Editor.h"
#include "Set.h"
#include "Inventory.h"
#include "Crosshair.h"
#include "Scene.h"
#include "Clock.h"
static bool initialized = false;

static unsigned int FPVCam;
static Clock inputClock;
static Lightning::Light* flashLight;
static float MovementSpeed = 2.25f;
static Sets::Set* set;

void Tools::Initialize(GameData* _gameData)
{
	Scene::Initialize(_gameData);
	Scene::World::SetSkyboxState(false);

	inputClock.Restart();
	FPVCam = Scene::World::NewCamera(glm::vec3(3.0f, 1.2f, 3.0f));
	Scene::World::FocusCamera(_gameData, FPVCam);

	Lightning::Light flashLight2;
	flashLight2.SetType(Lightning::LightType::SPOT);
	flashLight2.SetAmbient(glm::vec3(0.0f, 0.0f, 0.0f));
	flashLight2.SetDiffuse(glm::vec3(1.0f, 1.0f, 1.0f));
	flashLight2.SetSpecular(glm::vec3(1.0f, 1.0f, 1.0f));
	flashLight2.SetConstant(1.0f);
	flashLight2.SetLinear(0.09f);
	flashLight2.SetQuadratic(0.0032f);

	flashLight2.SetCutOff(90.5f);
	flashLight2.SetOuterCutOff(90.5f);
	flashLight2.SetName("FlashLight");
	flashLight2.SetActive(true);
	Scene::Lights::InsertLight(_gameData, flashLight2);

	Scene::Lights::UpdateShader(_gameData);

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
	/*Sets::Set**/ set = Sets::Create();
	set->GenerateRenderingInstance();

	for (size_t rowId = 0; rowId < 50; rowId++)
	{
		for (size_t columnId = 0; columnId < 50; columnId++)
		{
			Blocks::Block block;
			block.GenerateModel();
			block.SetType(blType[0]);
			glm::vec3 scale = block.GetType()->GetScale();
			block.SetScale(scale);
			block.SetPosition(glm::vec3(scale.x * rowId, .0f, scale.z * columnId));
			set->InsertBlock(block, false);
			if (!rowId || rowId == 49)
			{
				for (size_t heightId = 1; heightId < 20; heightId++)
				{
					Blocks::Block block;
					block.GenerateModel();
					block.SetType((heightId < 7) ? blType[1] : blType[2]);
					glm::vec3 scale = block.GetType()->GetScale();
					block.SetScale(scale);
					block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
					set->InsertBlock(block, false);
				}
			}
			else if (columnId == 49)
			{
				for (size_t heightId = 1; heightId < 20; heightId++)
				{
					Blocks::Block block;
					block.GenerateModel();
					block.SetType((heightId < 7) ? blType[1] : blType[2]);
					glm::vec3 scale = block.GetType()->GetScale();
					block.SetScale(scale);
					block.SetPosition(glm::vec3(scale.x * rowId, scale.y * heightId, scale.z * columnId));
					set->InsertBlock(block, false);
				}
			}
		}
	}
	set->SetName("Wall");
	set->SetPath("../Sets/");
	set->CalculateBoundingBox();

	initialized = true;
}


void Tools::Inputs(GameData* _gameData)
{
	glm::vec3 horizontalFront = glm::normalize(glm::vec3(_gameData->camera->Front.x, 0.0f, _gameData->camera->Front.z));
	float velocity = MovementSpeed * _gameData->dt;

	if ((!Editor::IsDisplayed() && !Inventory::IsActive()) || _gameData->window.IsKeyPressed(Keys::MouseButtons::MOUSE_BUTTON_MIDDLE))
	{
		if (!_gameData->window.IsFocused())
		{
			_gameData->window.Focus(true);
		}
		Scene::World::MouseInputs(_gameData);
		if (_gameData->window.IsKeyPressed(Keys::W))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, FORWARD);
		}
		if (_gameData->window.IsKeyPressed(Keys::S))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, BACKWARD);
		}
		if (_gameData->window.IsKeyPressed(Keys::A))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, LEFT);
		}
		if (_gameData->window.IsKeyPressed(Keys::D))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, RIGHT);
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_SHIFT))
		{

		}
		if (_gameData->window.IsKeyPressed(Keys::SPACE))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, UP);
		}
		if (_gameData->window.IsKeyPressed(Keys::LEFT_CONTROL))
		{
			Scene::World::ProcessCameraInput(_gameData, FPVCam, DOWN);
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

void Tools::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	Inputs(_gameData);

	std::vector<Lightning::Light>* lights = Scene::Lights::GetLights();
	for (size_t i = 0; i < lights->size(); i++)
	{
		if (lights->at(i).GetName() == "FlashLight")
		{
			Camera* cam = Scene::World::GetCamera();
			flashLight = &lights->at(i);
			flashLight->SetPosition(cam->Position);
			flashLight->SetDirection(cam->Front);
			Scene::Lights::UpdateShader(_gameData);
		}
	}

	Scene::Tick(_gameData);
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
				Sets::Edition(_gameData, true);
			}
		}
		ImGui::Render();
	}
	glDisable(GL_DEPTH_TEST);

	Crosshairs::Draw();

	glEnable(GL_DEPTH_TEST);
	//if (!_gameData->window.IsFocused())
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
}
