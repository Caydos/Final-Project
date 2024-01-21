#ifndef FRUSTRUM_CULLING_H
#define FRUSTRUM_CULLING_H
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FrustrumCulling
{
	enum FrustumPlane
	{
		FRUSTUM_RIGHT = 0,
		FRUSTUM_LEFT,
		FRUSTUM_BOTTOM,
		FRUSTUM_TOP,
		FRUSTUM_FAR,
		FRUSTUM_NEAR
	};

	bool isBoxInFrustum(glm::mat4 viewProjectionMatrix, glm::vec3 minCorner, glm::vec3 maxCorner)
	{
		// Extract frustum planes from the view projection matrix
		glm::vec4 planes[6];

		for (int i = 0; i < 4; ++i)
		{
			planes[i] = viewProjectionMatrix[i] + viewProjectionMatrix[12];
			planes[i + 4] = viewProjectionMatrix[12] - viewProjectionMatrix[i];
		}
		// Normalize frustum planes
		for (int i = 0; i < 6; ++i)
		{
			float length = glm::length(glm::vec3(planes[i]));
			planes[i] /= length;
		}

		// Check box against each frustum plane
		for (int i = 0; i < 6; ++i)
		{
			glm::vec3 normal = glm::vec3(planes[i]);
			float d = planes[i].w;

			// Calculate the distance from the box center to the frustum plane
			float distance = glm::dot(normal, 0.5f * (minCorner + maxCorner)) + d;

			// If the distance is negative, the box is behind the frustum plane
			if (distance < 0.0f)
			{
				return false;
			}
		}
		// If the box is not behind any frustum plane, it is inside the frustum
		return true;
	}
}
#endif // !FRUSTRUM_CULLING_H
