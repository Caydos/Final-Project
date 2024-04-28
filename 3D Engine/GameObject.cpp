#include "GameObject.h"
#include "RayCasting.h"
#include "Scene.h"
#include "Interaction.h"

static std::shared_mutex mutex;
static std::vector<GameObjects::Object> objectArray;

void GameObjects::Register(Sets::Set* _set, float _range, float _cooldown, void (*_hoveredFunction)(), void (*_interactFunction)(Sets::Set* _set))
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	Object obj;
	obj.set = _set;
	obj.range = _range;
	obj.cooldown = _cooldown;
	obj.hoveredFunction = _hoveredFunction;
	obj.interactFunction = _interactFunction;
	objectArray.push_back(obj);
}

void GameObjects::UnRegister(Sets::Set* _set)
{
	std::unique_lock<std::shared_mutex> lock(mutex);
	for (size_t objectId = 0; objectId < objectArray.size(); objectId++)
	{
		if (objectArray[objectId].set == _set)
		{
			objectArray.erase(objectArray.begin() + objectId);
			return;
		}
	}
}

void GameObjects::Tick(GameData* _gameData)
{
	std::shared_lock<std::shared_mutex> lock(mutex);
	// Assuming your crosshair is at the center of the screen
	glm::vec4 ray_clip = glm::vec4(0.0, 0.0, -1.0, 1.0);

	// Convert to Eye Space
	glm::mat4 inv_projection = glm::inverse(Scene::World::GetProjection());
	glm::vec4 ray_eye = inv_projection * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // Homogenize

	// Convert to World Space
	glm::mat4 inv_view = glm::inverse(Scene::World::GetView());
	glm::vec4 ray_wor = inv_view * ray_eye;
	// Don't forget to normalize the ray direction
	glm::vec3 ray_world_direction = glm::normalize(glm::vec3(ray_wor));

	RayCasting::Ray ray;
	glm::vec3 camPosition = Scene::World::GetCamera()->Position;
	ray.origin = camPosition;
	ray.direction = ray_world_direction;

	for (size_t objectId = 0; objectId < objectArray.size(); objectId++)
	{
		float rslt = RayCasting::Intersect(ray, objectArray[objectId].set->GetBoundingBox());

		if (rslt > .0f && rslt < objectArray[objectId].range)
		{
			if (objectArray[objectId].hoveredFunction != nullptr)
			{
				objectArray[objectId].hoveredFunction();
			}
			if (objectArray[objectId].interactFunction != nullptr 
				&& _gameData->window.IsKeyPressed(Keys::E) 
				&& objectArray[objectId].clock.GetElapsedTime() > objectArray[objectId].cooldown)
			{
				//Interactions::Trigger(objectArray[objectId].interactionName.c_str());
				objectArray[objectId].clock.Restart();
				objectArray[objectId].interactFunction(objectArray[objectId].set);
			}
			break;
		}
	}
}
