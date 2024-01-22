#ifndef INSTANCES_H
#define INSTANCES_H
#include "Common.h"
#include "Cube.h"


namespace Instances
{
	struct Cube
	{
		Material* material;
		Shaders::Shader* shader;
		Colors::Color color;
		std::vector<glm::mat4> models;
		unsigned int VBO;//Vertex Buffer Object
		unsigned int shapeVAO;//Shape vertices, does not allow different shapes

		bool lightDependent;

		void Initialize()
		{
			glGenBuffers(1, &this->VBO);
			glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
			glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);

			glBindVertexArray(this->shapeVAO);
			// Assuming Cube's VBO setup is already done elsewhere
			// Setup the instance array
			for (unsigned int i = 0; i < 4; i++)
			{
				glEnableVertexAttribArray(i + 4); // 4 is an offset, assuming 0, 1, 2, 3 are used for the cube's vertex data
				glVertexAttribPointer(i + 4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
				glVertexAttribDivisor(i + 4, 1); // Tell OpenGL this is an instanced vertex attribute.
			}
		}


		void Add(::Cube* _cube)
		{
			this->shapeVAO = _cube->GetVAO();
			models.push_back(_cube->GetModel());
		}

		void Draw()
		{
			GameData* gameData = GetGameData();

			//this->shader->use();

			this->shader->setBool("instanceUsage", true);

			int mode = 0;
			if (material->GetTexture() != nullptr)
			{
				mode++;
				glBindTexture(GL_TEXTURE_2D, material->GetTexture()->id);
			}

			shader->setBool("lightDependent", this->lightDependent);
			shader->setInt("mode", mode);
			shader->setVec4("Color", glm::vec4(this->color.values[0], this->color.values[1], this->color.values[2], this->color.values[3]));
			shader->setFloat("opacity", 1.0f);

			if (this->material != nullptr)
			{
				shader->setVec3("material.diffuse", this->material->GetDiffuse());
				shader->setVec3("material.specular", this->material->GetSpecular());
				shader->setFloat("material.shininess", this->material->GetShininess());
			}

			// create transformations
			glBindVertexArray(this->shapeVAO);

			glDrawArraysInstanced(GL_TRIANGLES, 0, 36, models.size());

			glBindVertexArray(0);
			this->shader->setBool("instanceUsage", false);
		}
	};


	std::vector<Cube> Generate(std::vector<::Cube>* _objects);

}
#endif // !INSTANCES_H