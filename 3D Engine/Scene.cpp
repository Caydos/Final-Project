#include "Scene.h"
#include "Material.h"
#include "Clock.h"
#include "Editor.h"
#include "Line.h"
#include "RayCasting.h"
#include "TexturePicker.h"
#include "Decor.h"
#include "Crosshair.h"
#include "Maze.h"

static bool initialized = false;
static unsigned int FPVCam;
static std::vector<Material> materials;
static Scene::Type type = Scene::Type::MODEL_EDITOR;
static Clock inputClock;
static Colors::Color clearColor = Colors::LimedSpruce;
static Lightning::Light* flashLight;

const char** Scene::GetMaterialsAsStringArray()
{
	const char** items = new const char* [materials.size()];
	for (size_t i = 0; i < materials.size(); ++i) {
		std::string materialName = materials[i].GetName();
		char* nameCopy = new char[materialName.length() + 1];
		strcpy(nameCopy, materialName.c_str());
		items[i] = nameCopy;
	}
	return items;
}

int Scene::GetMaterialsCount()
{
	return materials.size();
}

std::vector<Material>* Scene::GetMaterials()
{
	return &materials;
}

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

	std::vector<std::string> materialsPaths = Files::GetAllAtPath(MATERIAL_DIRECTORY);
	materials.resize(materialsPaths.size());
	for (size_t i = 0; i < materials.size(); i++)
	{
		materials[i].LoadFromFile(materialsPaths[i].c_str());
	}

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

	TexturePicker::Initialize(_gameData, &materials);
	//Maze::Generate();
	Crosshairs::Get()->Initialize();

	initialized = true;
}

void Scene::Inputs(GameData* _gameData)
{
	if (_gameData->window.IsFocused())
	{
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
	if (_gameData->window.IsKeyPressed(Keys::E) && inputClock.GetElapsedTime() > 125 && !Editor::IsDisplayed())
	{
		if (TexturePicker::IsActive())
		{
			TexturePicker::SetActive(false);
			_gameData->window.Focus(true);
		}
		else
		{
			TexturePicker::SetActive(true);
			_gameData->window.Focus(false);
		}
		inputClock.Restart();
	}
	if (_gameData->window.IsKeyPressed(Keys::ESCAPE) && inputClock.GetElapsedTime() > 125)
	{
		if (TexturePicker::IsActive())
		{
			TexturePicker::SetActive(false);
			_gameData->window.Focus(true);
		}
		else if (Editor::IsDisplayed())
		{
			Editor::SetDisplay(false);
			_gameData->window.Focus(true);
		}
		inputClock.Restart();
	}
	if (_gameData->window.IsKeyPressed(Keys::F1) && inputClock.GetElapsedTime() > 125 && !TexturePicker::IsActive())
	{
		if (Editor::IsDisplayed())
		{
			Editor::SetDisplay(false);
			_gameData->window.Focus(true);
		}
		else
		{
			Editor::SetDisplay(true);
			_gameData->window.Focus(false);
		}
		inputClock.Restart();
	}
}

void Scene::Tick(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }
	Inputs(_gameData);

	// Needs to be called after the inputs that enables it
	if (!_gameData->window.IsFocused())
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (Editor::IsDisplayed())
		{
			Editor::Menu(_gameData);
		}

		ImGui::Render();
	}


	_gameData->window.Clear(clearColor);
	Scene::World::Render(_gameData);
	if (!Editor::IsDisplayed())
	{
		TexturePicker::Inputs(_gameData);
		TexturePicker::Render(_gameData);
	}
	Editor::Tick(_gameData);
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
	if (!_gameData->window.IsFocused())
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}
	_gameData->window.Events();
}

void Scene::CleanUp()
{
}






Scene::World::Component::Component()
{
	this->scale = 1.0f;
	this->visible = true;
	this->raycastHit = nullptr;
	useOffset = false;
}

Scene::World::Component::~Component()
{
}

bool Scene::World::Component::IsVisible()
{
	return this->visible;
}

void Scene::World::Component::SetVisible(bool _state)
{
	this->visible = _state;
}

std::string Scene::World::Component::GetName()
{
	return this->name;
}

void Scene::World::Component::SetName(std::string _name)
{
	this->name = _name;
}

Cube* Scene::World::Component::GetRaycastHit()
{
	return this->raycastHit;
}

void Scene::World::Component::SetRaycastHit(Cube* _hit)
{
	this->raycastHit = _hit;
}

float Scene::World::Component::GetScale()
{
	return this->scale;
}

void Scene::World::Component::SetScale(float _scale)
{
	this->scale = _scale;
}

bool Scene::World::Component::IsUsingOffsets()
{
	return this->useOffset;
}

void Scene::World::Component::SetOffsetUsage(bool _usage)
{
	this->useOffset = _usage;
}

glm::vec3 Scene::World::Component::GetOffset()
{
	return this->offset;
}

void Scene::World::Component::SetOffset(glm::vec3 _offset)
{
	this->offset = _offset;
}


bool Scene::World::Component::AreBoundsVisible()
{
	return this->showBounds;
}
void Scene::World::Component::ShowBounds(bool _displayed)
{
	this->showBounds = _displayed;
}
