#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "Material.h"
//#include <glm/ext/matrix_float4x4.hpp>

#define BLOCKS_DIRECTORY "../Blocks/"
#define BLOCKS_FILE_EXTENSION ".json"


namespace Blocks
{
	class BlockType
	{
	public:
		BlockType()
		{
			this->graphicsLoaded = false;
		}
		~BlockType() {}

		void GenerateGraphicsBuffers()
		{
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			this->graphicsLoaded = true;
		}
		void RemoveGraphicsBuffers()
		{
			glDeleteBuffers(1, &this->VBO);
			glDeleteVertexArrays(1, &this->VAO);
			this->graphicsLoaded = false;
		}

		void BindGraphicsBuffers()
		{
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);
		}
		void SetVertices(std::vector<float>* _vertices)
		{
			this->vertices = *_vertices;
			glBindVertexArray(this->VAO);

			glBindBuffer(GL_ARRAY_BUFFER, this->VAO);
			glBufferData(GL_ARRAY_BUFFER, (this->vertices.size() / 8) * sizeof(float), &this->vertices, GL_STATIC_DRAW);

			// Position attribute
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);

			// Texture attribute
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
			glEnableVertexAttribArray(1);

			// Lightning normal attribute
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
			glEnableVertexAttribArray(2);

			for (unsigned int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(i + 4); // 4 is an offset, assuming 0, 1, 2, 3 are used for the cube's vertex data
				glVertexAttribPointer(i + 4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisor(i + 4, 1); // Tell OpenGL this is an instanced vertex attribute.
			}
		}
		void RegenerateMatrices()
		{
			models.clear();
			models.resize(modelsAddresses.size());
			for (size_t i = 0; i < models.size(); i++)
			{
				models[i] = *modelsAddresses[i];
			}
		}

		std::string GetName()
		{
			return this->GetName();
		}
		void SetName(std::string _name)
		{
			this->name = _name;
		}

		void Draw()
		{
			if (this->graphicsLoaded)
			{
				this->shader->setBool("instanceUsage", true);

				int mode = 0;
				if (texture != nullptr)
				{
					mode++;
					glBindTexture(GL_TEXTURE_2D, texture->id);
				}
			}
		}

	private:
		Shaders::Shader* shader;


		bool graphicsLoaded;
		std::string name;

		std::vector<glm::mat4> models;
		std::vector<glm::mat4*> modelsAddresses;

		unsigned int VAO;
		unsigned int VBO;

		Material material;

		Texture* texture;
		Colors::Color color;
		bool lightDependent;
		std::vector<float> vertices;
	};

	void Load();
	void Refresh();
}

#endif // !BLOCKS_H
