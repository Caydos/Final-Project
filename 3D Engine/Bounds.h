#ifndef BOUNDS_H
#define BOUNDS_H
#include <glm/ext/vector_float3.hpp>
#include "Common.h"

namespace Bounds
{
	struct Box
	{
		Box();
		glm::vec3 min;
		glm::vec3 max;

		Box(const glm::vec3& min, const glm::vec3& max) : min(min), max(max) {}

		// Expand box slightly to handle floating-point inaccuracies
		Box getExpanded(const glm::vec3& velocity) const {
			glm::vec3 expandedMin = min, expandedMax = max;
			for (int i = 0; i < 3; ++i) {
				if (velocity[i] < 0.0f) {
					expandedMin[i] += velocity[i];
				}
				else {
					expandedMax[i] += velocity[i];
				}
			}
			return Box(expandedMin, expandedMax);
		}

		// Check for overlap between two boxes
		bool overlaps(const Box& other) const {
			return (min.x <= other.max.x && max.x >= other.min.x) &&
				(min.y <= other.max.y && max.y >= other.min.y) &&
				(min.z <= other.max.z && max.z >= other.min.z);
		}
		// Utility function to check intersection and calculate penetration depth
		bool intersects(const Box& other, glm::vec3& penetration) const {
			glm::vec3 noPenetration(0.0f);

			// Calculate overlap on each axis
			for (int i = 0; i < 3; ++i) {
				float minOverlap = other.max[i] - this->min[i];
				float maxOverlap = this->max[i] - other.min[i];

				// No overlap means no collision on this axis
				if (minOverlap <= 0 || maxOverlap <= 0) {
					penetration = noPenetration;
					return false;
				}

				// Choose the smaller overlap as the axis penetration
				penetration[i] = (minOverlap < maxOverlap) ? minOverlap : -maxOverlap;
			}

			return true;
		}
	};

	class BoundingBox
	{
	public:
		BoundingBox();
		~BoundingBox();

		void Initialize();


		void Draw();

		Box GetBox();
		void SetBox(Box _box);

	private:
		glm::mat4 model;
		Shaders::Shader* shader;
		bool initialized;
		unsigned int VBO;
		unsigned int VAO;
		float lines[24 * 3];
		Box box;
	};
}

#endif // !BOUNDS_H