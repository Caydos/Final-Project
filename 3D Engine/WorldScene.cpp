#include "Scene.h"
#include "Camera.h"
#include "Lightning.h"
#include <algorithm>
#include "Files.h"
#include "FrustrumCulling.h"

static std::vector<Camera> cameras;
static unsigned int focusedCamera = 0;
static std::vector <Scene::World::Component*> components;
static std::shared_mutex loadingMutex;
static std::vector<Scene::World::Component*> loadingQueue;

static glm::mat4 projection;
static glm::mat4 view;
static std::string sceneName;
static bool considerLightning = false;

void Scene::World::InsertComponent(Component* _component)
{
	components.push_back(_component);
}

void Scene::World::RemoveComponent(Component* _component)
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i] == _component)
		{
			delete components[i];
			components.erase(components.begin() + i);
			return;
		}
	}
	std::cout << "Unable to remove component" << std::endl;
}

std::vector<Scene::World::Component*>* Scene::World::GetComponents()
{
	return &components;
}

void Scene::World::QueueLoading(Component* _component, std::string _fileName)
{
	std::unique_lock<std::shared_mutex> lock(loadingMutex);
	_component->SetName(_fileName.c_str());
	loadingQueue.push_back(_component);
}



void Scene::World::ConsiderLightning(bool _value)
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
	_gameData->shaders[Shaders::WORLD_OBJECT]->use();
	projection = glm::perspective(glm::radians(camera->Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	_gameData->shaders[Shaders::WORLD_OBJECT]->setMat4("projection", projection);
	_gameData->camera = camera;
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
		camera->ProcessMouseMovement(_gameData->window.xoffset, _gameData->window.yoffset);
	}
	catch (const std::out_of_range& e)
	{
		std::cout << "[Camera] - (MouseInputs) : Exception caught: " << e.what() << std::endl;
	}
}

void Scene::World::Render(GameData* _gameData)
{
	{
		std::unique_lock<std::shared_mutex> lock(loadingMutex);
		while (!loadingQueue.empty())
		{
			auto& item = loadingQueue.front();
			item->LoadFromFile(item->GetName().c_str());
			loadingQueue.erase(loadingQueue.begin());
			InsertComponent(item);
		}
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);


	Camera* camera = &cameras.at(focusedCamera);

	_gameData->shaders[Shaders::WORLD_OBJECT]->use();

	projection = glm::perspective(glm::radians(camera->Fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);
	_gameData->shaders[Shaders::WORLD_OBJECT]->setMat4("projection", projection);

	_gameData->shaders[Shaders::WORLD_OBJECT]->setBool("considerLightning", considerLightning);
	_gameData->shaders[Shaders::WORLD_OBJECT]->setVec3("viewPos", camera->Position);

	view = camera->GetViewMatrix();
	_gameData->shaders[Shaders::WORLD_OBJECT]->setMat4("view", view);

	glm::vec3 position = camera->Position;

	for (size_t objId = 0; objId < components.size(); objId++)
	{
		Bounds::Box boundingBox = components[objId]->GetBoundingBox();
		if (FrustrumCulling::IsBoxInFrustum(projection,view, boundingBox.min, boundingBox.max))
		{
			components[objId]->Draw();
		}
	}
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
}

void Scene::World::CleanUp()
{
}
