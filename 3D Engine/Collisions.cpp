#include "Collisions.h"
#include "Scene.h"
#include "Crosshair.h"
#include "Set.h"
#include "Clock.h"

bool Collisions::PointCollidingBox(glm::vec3 _position, Bounds::Box _box)
{
	bool collisionX = _position.x >= _box.min.x && _position.x <= _box.max.x;
	bool collisionY = _position.y >= _box.min.y && _position.y <= _box.max.y;
	bool collisionZ = _position.z >= _box.min.z && _position.z <= _box.max.z;

	return collisionX && collisionY && collisionZ;
}

Collisions::Result Collisions::BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox)
{
	Result result;

	float penetrationX = std::min(_firstBox.max.x, _secondBox.max.x) - std::max(_firstBox.min.x, _secondBox.min.x);
	float penetrationY = std::min(_firstBox.max.y, _secondBox.max.y) - std::max(_firstBox.min.y, _secondBox.min.y);
	float penetrationZ = std::min(_firstBox.max.z, _secondBox.max.z) - std::max(_firstBox.min.z, _secondBox.min.z);

	bool collisionX = penetrationX > 0;
	bool collisionY = penetrationY > 0;
	bool collisionZ = penetrationZ > 0;

	result.isColliding = collisionX && collisionY && collisionZ;

	if (result.isColliding)
	{
		result.penetration.x = collisionX ? penetrationX : 0.0f;
		result.penetration.y = collisionY ? penetrationY : 0.0f;
		result.penetration.z = collisionZ ? penetrationZ : 0.0f;
	}

	return result;
}

glm::vec3 Collisions::CalculateCollisionResponse(Bounds::Box movingBox, Bounds::Box stationaryBox, glm::vec3 velocity)
{
	//Bounds::Box expandedBox = movingBox.getExpanded(velocity);

	//if (!expandedBox.overlaps(stationaryBox)) {
	//	// If there's no overlap even after expanding, no collision is expected
	//	return velocity;
	//}

	//// Attempt to move on each axis individually to find allowed movement
	//glm::vec3 allowedMovement = velocity;
	//glm::vec3 axes[3] = { glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(0, 0, 1) };

	//for (int i = 0; i < 3; ++i) {
	//	glm::vec3 axisMovement(0);
	//	axisMovement[i] = velocity[i];
	//	Bounds::Box testBox = movingBox.getExpanded(axisMovement);

	//	if (!testBox.overlaps(stationaryBox)) {
	//		// If moving along this axis doesn't cause a collision, retain this component of the movement
	//		continue;
	//	}

	//	// Calculate the depth of overlap along this axis
	//	float depth = std::min(stationaryBox.max[i] - movingBox.min[i], movingBox.max[i] - stationaryBox.min[i]);
	//	if (velocity[i] > 0) {
	//		allowedMovement[i] = std::min(allowedMovement[i], depth);
	//	}
	//	else if (velocity[i] < 0) {
	//		allowedMovement[i] = std::max(allowedMovement[i], -depth);
	//	}
	//	allowedMovement.y = 0;
	//	return allowedMovement;
	//}
	Bounds::Box predictedBox = { movingBox.min + velocity, movingBox.max + velocity };
	glm::vec3 penetration;

	if (!predictedBox.intersects(stationaryBox, penetration)) {
		// No collision; proceed with the movement
		return velocity;
	}
	else {
		// Collision; resolve along the minimal penetration axis
		glm::vec3 minimalPenetration = penetration;

		// Find the axis with the minimal penetration
		if (std::abs(penetration.x) < std::abs(penetration.y) && std::abs(penetration.x) < std::abs(penetration.z)) {
			minimalPenetration.y = minimalPenetration.z = 0.0f;
		}
		else if (std::abs(penetration.y) < std::abs(penetration.z)) {
			minimalPenetration.x = minimalPenetration.z = 0.0f;
		}
		else {
			minimalPenetration.x = minimalPenetration.y = 0.0f;
		}

		// Adjust the velocity based on the penetration depth
		glm::vec3 adjustedVelocity = velocity + minimalPenetration;

		return adjustedVelocity;
	}
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
