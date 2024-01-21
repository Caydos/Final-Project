#ifndef FRUSTRUM_CULLING_H
#define FRUSTRUM_CULLING_H
#include "Common.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace FrustrumCulling
{
	struct Plane
	{
		glm::vec3 normal;
		float distance;
	};

	bool IsBoxInFrustum(glm::mat4 projectionMatrix, glm::mat4 viewMatrix, glm::vec3 minCorner, glm::vec3 maxCorner)
	{
		glm::mat4 mat = projectionMatrix * viewMatrix;
		Plane planes[6];

		for (int i = 0; i < 6; i++)
		{
			int sign = (i % 2 == 0) ? 1 : -1;

			planes[i].normal.x = mat[0][3] + sign * mat[0][i / 2];
			planes[i].normal.y = mat[1][3] + sign * mat[1][i / 2];
			planes[i].normal.z = mat[2][3] + sign * mat[2][i / 2];
			planes[i].distance = mat[3][3] + sign * mat[3][i / 2];

			if (i < 4)
			{
				planes[i].distance += 0.1f; // Margin
			}
		}

		for (const auto& plane : planes)
		{
			if (glm::dot(plane.normal, minCorner) + plane.distance < 0
				&& glm::dot(plane.normal, maxCorner) + plane.distance < 0)
			{
				return false;
			}
		}

		return true;
	}

}
#endif // !FRUSTRUM_CULLING_H
