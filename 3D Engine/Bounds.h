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

		glm::vec3 position;     // Center of the box
		glm::vec3 extents;    // Half-size extents of the box
		glm::mat3 rotation; // 3x3 rotation matrix representing the box's orientation
	};
	bool AreColliding(const Box& box1, const Box& box2, glm::vec3& collisionNormal);
	std::vector<glm::vec3> GetCorners(const Box& box);

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