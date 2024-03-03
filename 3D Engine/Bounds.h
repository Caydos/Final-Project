#ifndef BOUNDS_H
#define BOUNDS_H
#include <glm/ext/vector_float3.hpp>
#include "Common.h"

namespace Bounds
{
	struct Box
	{
		glm::vec3 min;
		glm::vec3 max;
	};

	class BoundingBox
	{
	public:
		BoundingBox()
		{
			this->initialized = false;
		}
		~BoundingBox() {}

		void Initialize()
		{
			// Create VBO and VAO
			glGenBuffers(1, &this->VBO);
			glGenVertexArrays(1, &this->VAO);

			// Bind VAO and VBO, upload vertex data
			glBindVertexArray(this->VAO);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(this->lines), this->lines, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Unbind
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
			this->shader = GetGameData()->shaders[Shaders::GEOMETRY];
			this->initialized = true;
		}


		void Draw()
		{
			if (!initialized)
			{
				this->Initialize();
			}
			glm::mat4 test(1.0f);
			this->shader->setMat4("model", test);
			this->shader->setVec4("color", glm::vec4(1.0f, 1.0f,1.0f,1.0f));
			this->shader->setFloat("opacity", 1.0f);
			glBindVertexArray(this->VAO);

			// Draw the lines
			glDrawArrays(GL_LINES, 0, 24);

			// Unbind the VAO
			glBindVertexArray(0);
		}

		Box GetBox()
		{
			return this->box;
		}
		void SetBox(Box _box)
		{
			if (!initialized)
			{
				this->Initialize();
			}
			this->box = _box;
			glm::vec3 corners[8] = {
				glm::vec3(box.min.x, box.min.y, box.min.z),
				glm::vec3(box.min.x, box.min.y, box.max.z),
				glm::vec3(box.min.x, box.max.y, box.min.z),
				glm::vec3(box.min.x, box.max.y, box.max.z),
				glm::vec3(box.max.x, box.min.y, box.min.z),
				glm::vec3(box.max.x, box.min.y, box.max.z),
				glm::vec3(box.max.x, box.max.y, box.min.z),
				glm::vec3(box.max.x, box.max.y, box.max.z)
			};

			int lineIndices[24] = {
					0, 1, 1, 3, 3, 2, 2, 0, // Front face
					4, 5, 5, 7, 7, 6, 6, 4, // Back face
					0, 4, 1, 5, 2, 6, 3, 7  // Connectors
			};

			for (int i = 0; i < 24; ++i)
			{
				glm::vec3 point = corners[lineIndices[i]] * 0.8f;
				lines[i * 3] = point.x;
				lines[i * 3 + 1] = point.y;
				lines[i * 3 + 2] = point.z;
			}

			if (initialized)
			{
				glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(this->lines), this->lines, GL_DYNAMIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
		}

	private:
		glm::vec3 scale;
		Shaders::Shader* shader;
		bool initialized;
		unsigned int VBO;
		unsigned int VAO;
		float lines[24 * 3];
		Box box;
	};
}

#endif // !BOUNDS_H