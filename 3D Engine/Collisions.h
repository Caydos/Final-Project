#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "Bounds.h"

namespace Collisions
{
	bool PointCollidingBox(glm::vec3 _position, Bounds::Box _box);
	bool BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox);
	bool IntersectRayWithBox(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& t);
	void Tick();
}

#endif // !COLLISIONS_H