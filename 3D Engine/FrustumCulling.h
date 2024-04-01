#ifndef FRUSTRUM_CULLING_H
#define FRUSTRUM_CULLING_H
#include "Common.h"


namespace FrustumCulling
{
	struct Plane
	{
		glm::vec3 normal;
		float distance;

		void Normalize();
	};

	bool IsBoxInFrustum(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, glm::vec3 _minCorner, glm::vec3 _maxCorner);

}
#endif // !FRUSTRUM_CULLING_H
