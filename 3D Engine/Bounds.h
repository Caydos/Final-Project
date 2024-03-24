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