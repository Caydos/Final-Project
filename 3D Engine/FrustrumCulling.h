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

		void Normalize()
		{
			float mag = glm::length(normal);
			normal /= mag;
			distance /= mag;
		}
	};

	bool IsBoxInFrustum(glm::mat4 _projectionMatrix, glm::mat4 _viewMatrix, glm::vec3 _minCorner, glm::vec3 _maxCorner)
	{
		glm::mat4 mat = _projectionMatrix * _viewMatrix;
		Plane planes[6];

		for (int i = 0; i < 6; i++)
		{
			int sign = (i % 2 == 0) ? 1 : -1;

			planes[i].normal.x = mat[0][3] + sign * mat[0][i / 2];
			planes[i].normal.y = mat[1][3] + sign * mat[1][i / 2];
			planes[i].normal.z = mat[2][3] + sign * mat[2][i / 2];
			planes[i].distance = mat[3][3] + sign * mat[3][i / 2];

			planes[i].Normalize();
		}

		glm::vec3 corners[8] =
		{
			{_minCorner.x, _minCorner.y, _minCorner.z},
			{_maxCorner.x, _minCorner.y, _minCorner.z},
			{_minCorner.x, _maxCorner.y, _minCorner.z},
			{_maxCorner.x, _maxCorner.y, _minCorner.z},
			{_minCorner.x, _minCorner.y, _maxCorner.z},
			{_maxCorner.x, _minCorner.y, _maxCorner.z},
			{_minCorner.x, _maxCorner.y, _maxCorner.z},
			{_maxCorner.x, _maxCorner.y, _maxCorner.z}
		};

		for (const auto& plane : planes)
		{
			int out = 0;
			for (int i = 0; i < 8; i++)
			{
				if (glm::dot(plane.normal, corners[i]) + plane.distance < 0)
				{
					out++;
				}
			}
			if (out == 8)
			{
				return false;
			}
		}


		return true;
	}

}
#endif // !FRUSTRUM_CULLING_H
