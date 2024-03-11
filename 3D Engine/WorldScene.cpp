#include "Scene.h"
#include "Camera.h"
#include "Lightning.h"
#include <algorithm>
#include "Files.h"
#include "Blocks.h"
#include "Set.h"
#include "DeferredShading.h"

static std::vector<Camera> cameras;
static unsigned int focusedCamera = 0;

static std::shared_mutex loadingMutex;

static glm::mat4 projection;
static glm::mat4 view;
static std::string sceneName;
static bool considerLightning = true;


// For later sync
//void Scene::World::QueueLoading(Component* _component, std::string _fileName)
//{
//	std::unique_lock<std::shared_mutex> lock(loadingMutex);
//	_component->SetName(_fileName.c_str());
//	loadingQueue.push_back(_component);
//}



void Scene::World::ConsiderLightning(bool _value, GameData* _gameData)
{
	considerLightning = _value;
}

bool Scene::World::IsConsideringLightning()
{
	return considerLightning;
}

unsigned int Scene::World::NewCamera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
{
	cameras.push_back(Camera(position, up, yaw, pitch));
	return cameras.size() - 1;
}

void Scene::World::FocusCamera(GameData* _gameData, unsigned int _cameraId)
{
	focusedCamera = _cameraId;
	Camera* camera = &cameras.at(focusedCamera);

	projection = glm::perspective(glm::radians(camera->Fov), (float)_gameData->resolution[0] / (float)_gameData->resolution[1], 0.1f, 100.0f);

	_gameData->shaders[Shaders::SINGLE_DRAW]->use();
	_gameData->shaders[Shaders::SINGLE_DRAW]->setMat4("projection", projection);
	_gameData->shaders[Shaders::GEOMETRY]->use();
	_gameData->shaders[Shaders::GEOMETRY]->setMat4("projection", projection);


	_gameData->camera = camera;
	camera->MouseSensitivity = _gameData->settings.sentivity;
}

void Scene::World::SetCameraPosition(unsigned int _cameraId, glm::vec3 _position)
{
	Camera* camera = &cameras.at(_cameraId);
	camera->Position = _position;
}

void Scene::World::ProcessCameraInput(GameData* _gameData, unsigned int _cameraId, Camera_Movement _movement)
{
	Camera* camera = &cameras.at(_cameraId);
	camera->ProcessKeyboard(_movement, _gameData->dt);
}

Camera* Scene::World::GetCamera()
{
	return &cameras.at(focusedCamera);
}

float Scene::World::GetCameraFov()
{
	Camera* camera = &cameras.at(focusedCamera);
	return camera->Fov;
}

void Scene::World::SetCameraFov(float _newFov)
{
	Camera* camera = &cameras.at(focusedCamera);
	camera->Fov = _newFov + .0f;
}

glm::mat4 Scene::World::GetProjection()
{
	return projection;
}
glm::mat4 Scene::World::GetView()
{
	return view;
}

void Scene::World::MouseInputs(GameData* _gameData)
{
	try
	{
		Camera* camera = &cameras.at(focusedCamera);
		camera->ProcessMouseMovement(_gameData->window.xoffset, _gameData->window.yoffset, true, _gameData->dt);
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "[Camera] - (MouseInputs) : Exception caught: " << e.what() << std::endl;
	}
}


void Scene::World::Render(GameData* _gameData)
{
    Camera* camera = &cameras.at(focusedCamera);
    projection = glm::perspective(glm::radians(camera->Fov), (float)_gameData->resolution[0] / (float)_gameData->resolution[1], 0.1f, 100.0f);
	view = camera->GetViewMatrix();
    {

		_gameData->shaders[Shaders::SINGLE_DRAW]->use();
        _gameData->shaders[Shaders::SINGLE_DRAW]->setMat4("projection", projection);
        _gameData->shaders[Shaders::SINGLE_DRAW]->setMat4("view", view);
        _gameData->shaders[Shaders::SINGLE_DRAW]->setBool("instanceUsage", false);

		_gameData->shaders[Shaders::GEOMETRY]->use();
        _gameData->shaders[Shaders::GEOMETRY]->setMat4("projection", projection);
        _gameData->shaders[Shaders::GEOMETRY]->setMat4("view", view);
        _gameData->shaders[Shaders::GEOMETRY]->setBool("instanceUsage", true);

    }
	DeferredShading::Draw(_gameData);
}


void Scene::World::CleanUp()
{
}
