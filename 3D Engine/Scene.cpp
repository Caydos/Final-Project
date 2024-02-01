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
	flashLight2.SetLinear(0.09f);
	flashLight2.SetQuadratic(0.032f);

	flashLight2.SetCutOff(12.5f);
	flashLight2.SetOuterCutOff(15.5f);
	flashLight2.SetName("FlashLight");
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

