#include "Blocks.h"


Blocks::BlockType::BlockType()
{
	this->graphicsLoaded = false;
}
Blocks::BlockType::~BlockType() {}

void Blocks::BlockType::GenerateGraphicsBuffers()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	this->graphicsLoaded = true;
}
void Blocks::BlockType::RemoveGraphicsBuffers()
{
	glDeleteBuffers(1, &this->VBO);
	glDeleteVertexArrays(1, &this->VAO);
	this->graphicsLoaded = false;
}

void Blocks::BlockType::BindGraphicsBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, models.size() * sizeof(glm::mat4), &models[0], GL_STATIC_DRAW);
}

std::string Blocks::BlockType::GetName()
{
	return this->GetName();
}
void Blocks::BlockType::SetName(std::string _name)
{
	this->name = _name;
}

void Blocks::BlockType::SetVertices(std::vector<float>* _vertices)
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
void Blocks::BlockType::RegenerateMatrices()
{
	models.clear();
	models.resize(modelsAddresses.size());
	for (size_t i = 0; i < models.size(); i++)
	{
		models[i] = *modelsAddresses[i];
	}
}



void Blocks::BlockType::AddModel(glm::mat4* _model)
{
	modelsAddresses.push_back(_model);
}

void Blocks::BlockType::RemoveModel(glm::mat4* _model)
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





Shaders::Shader* Blocks::BlockType::GetShader()
{
	return this->shader;
}
void Blocks::BlockType::SetShader(Shaders::Shader* _shader)
{
	this->shader = _shader;
}

Texture* Blocks::BlockType::GetTexture()
{
	return this->texture;
}
void Blocks::BlockType::SetTexture(Texture* _texture)
{
	this->texture = _texture;
}

Colors::Color Blocks::BlockType::GetColor()
{
	return this->color;
}
void Blocks::BlockType::SetColor(Colors::Color _color)
{
	this->color = _color;
}

bool Blocks::BlockType::GetLightDependency()
{
	return this->lightDependent;
}
void Blocks::BlockType::SetLightDependency(bool _lightDependency)
{
	this->lightDependent = _lightDependency;
}

glm::vec3 Blocks::BlockType::GetDiffuse() { return this->diffuse; }
void Blocks::BlockType::SetDiffuse(glm::vec3 _diffuse) { this->diffuse = _diffuse; }

glm::vec3 Blocks::BlockType::GetSpecular() { return this->specular; }
void Blocks::BlockType::SetSpecular(glm::vec3 _specular) { this->specular = _specular; }

float Blocks::BlockType::GetShininess() { return this->shininess; }
void Blocks::BlockType::SetShininess(float _shininess) { this->shininess = _shininess; }

void Blocks::BlockType::Draw()
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


void Blocks::BlockType::AskForRefresh()
{
	refreshInQueue = true;
}