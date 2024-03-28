#ifndef COLLISIONS_H
#define COLLISIONS_H
#include "Bounds.h"

namespace Collisions
{
	struct Result
	{
		bool isColliding;
		glm::vec3 penetration; // Negative values indicate no penetration

		Result() : isColliding(false), penetration(glm::vec3(0.0f)) {}
	};
	bool PointCollidingBox(glm::vec3 _position, Bounds::Box _box);
	Result BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox);

	// Function to detect collision and return an offset vector for sliding
	glm::vec3 CalculateCollisionResponse(Bounds::Box movingBox, Bounds::Box stationaryBox, glm::vec3 velocity);
	bool IntersectRayWithBox(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& t);
}

#endif // !COLLISIONS_H