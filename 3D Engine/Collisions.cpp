#include "Collisions.h"
#include "Scene.h"
#include "Crosshair.h"

bool Collisions::PointCollidingBox(glm::vec3 _position, Bounds::Box _box)
{
	bool collisionX = _position.x >= _box.min.x && _position.x <= _box.max.x;
	bool collisionY = _position.y >= _box.min.y && _position.y <= _box.max.y;
	bool collisionZ = _position.z >= _box.min.z && _position.z <= _box.max.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

bool Collisions::BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox)
{
	bool collisionX = _firstBox.min.x <= _secondBox.max.x && _firstBox.max.x >= _secondBox.min.x;
	bool collisionY = _firstBox.min.y <= _secondBox.max.y && _firstBox.max.y >= _secondBox.min.y;
	bool collisionZ = _firstBox.min.z <= _secondBox.max.z && _firstBox.max.z >= _secondBox.min.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

bool Collisions::IntersectRayWithBox(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& t)
{
	glm::vec3 invDir = 1.0f / rayDirection;
	glm::vec3 tMin = (boxMin - rayOrigin) * invDir;
	glm::vec3 tMax = (boxMax - rayOrigin) * invDir;

	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm::max(tMin, tMax);

	float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
	float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

	if (tNear > tFar || tFar < 0.0f) {
		return false;
	}

	t = tNear;  // t will hold the distance at which the intersection occurs
	return true;
}

static bool initialized = false;

void Collisions::Tick()
{
	if (!initialized)
	{

		initialized = true;
	}


	Camera* cam = Scene::World::GetCamera();


}