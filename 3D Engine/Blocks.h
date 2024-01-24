#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "Material.h"
//#include <glm/ext/matrix_float4x4.hpp>

#define BLOCKS_DIRECTORY "../Blocks/"
#define BLOCKS_FILE_EXTENSION ".json"
#define BLOCKS_TEXTURE_EXTENSION ".jpg"


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

		std::string GetName()
		{
			return this->GetName();
		}
		void SetName(std::string _name)
		{
			this->name = _name;
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



		void AddModel(glm::mat4* _model)
		{
			modelsAddresses.push_back(_model);
		}

		void RemoveModel(glm::mat4* _model)
		{
			for (size_t i = 0; i < modelsAddresses.size(); i++)
			{
				if (modelsAddresses[i] == _model)
				{
					modelsAddresses.erase(modelsAddresses.begin() + i);
					return;
				}
			}
		}





		Shaders::Shader* GetShader()
		{
			return this->shader;
		}
		void SetShader(Shaders::Shader* _shader)
		{
			this->shader = _shader;
		}

		Texture* GetTexture()
		{
			return this->texture;
		}
		void SetTexture(Texture* _texture)
		{
			this->texture = _texture;
		}

		Colors::Color GetColor()
		{
			return this->color;
		}
		void SetColor(Colors::Color _color)
		{
			this->color = _color;
		}

		bool GetLightDependency()
		{
			return this->lightDependent;
		}
		void SetLightDependency(bool _lightDependency)
		{
			this->lightDependent = _lightDependency;
		}

		glm::vec3 GetDiffuse() { return this->diffuse; }
		void SetDiffuse(glm::vec3 _diffuse) { this->diffuse = _diffuse; }

		glm::vec3 GetSpecular() { return this->specular; }
		void SetSpecular(glm::vec3 _specular) { this->specular = _specular; }

		float GetShininess() { return this->shininess; }
		void SetShininess(float _shininess) { this->shininess = _shininess; }

		void Draw()
		{
			if (this->graphicsLoaded)
			{
				if (refreshInQueue)
				{
					RegenerateMatrices();
				}
				this->shader->setBool("instanceUsage", true);

				int mode = 0;
				if (this->texture != nullptr)
				{
					mode++;
					glBindTexture(GL_TEXTURE_2D, this->texture->id);
					if (this->color.values[3] > 0.0f)
					{
						mode++;
					}
				}

				shader->setVec4("color", glm::vec4(this->color.values[0], this->color.values[1], this->color.values[2], this->color.values[3]));
				shader->setBool("lightDependent", this->lightDependent);
				shader->setInt("mode", mode);
				shader->setFloat("opacity", 1.0f);

				shader->setVec3("material.diffuse", this->diffuse);
				shader->setVec3("material.specular", this->specular);
				shader->setFloat("material.shininess", this->shininess);
				// create transformations
				glBindVertexArray(this->VAO);

				glDrawArraysInstanced(GL_TRIANGLES, 0, 36, models.size());

				glBindVertexArray(0);
				this->shader->setBool("instanceUsage", false);
			}
		}


		void AskForRefresh()
		{
			refreshInQueue = true;
		}

	private:

		bool graphicsLoaded;
		unsigned int VAO;
		unsigned int VBO;
		std::string name;

		std::vector<float> vertices;
		std::vector<glm::mat4> models;
		std::vector<glm::mat4*> modelsAddresses;


		Shaders::Shader* shader;
		Texture* texture;
		Colors::Color color;

		bool lightDependent;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;


		bool refreshInQueue;
	};

	class Block
	{
	public:
		Block() {}
		~Block() {}

		glm::vec3 GetPosition();
		void SetPosition(float _x, float _y, float _z);
		void SetPosition(glm::vec3 _position);

		void Move(glm::vec3 _position);
		void Move(float _x, float _y, float _z);

		glm::vec3 GetRotation();
		void SetRotation(float _x, float _y, float _z);
		void SetRotation(glm::vec3 _rotation);

		void Rotate(glm::vec3 _rotation);
		void Rotate(float _x, float _y, float _z);

		glm::vec3 GetScale();
		void SetScale(float _x, float _y, float _z);
		void SetScale(float _scale);
		void SetScale(glm::vec3 _scale);

		void Scale(glm::vec3 _scale);
		void Scale(float _x, float _y, float _z);

	private:
		BlockType* type;
		glm::mat4* parent;
		glm::mat4* model;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	void Initialize();
	void Load(std::string _name);
	void Refresh();
}

#endif // !BLOCKS_H
