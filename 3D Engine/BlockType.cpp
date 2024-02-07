#include "Blocks.h"


Blocks::BlockType::BlockType()
{
	this->graphicsLoaded = false;
	std::memcpy(this->vertices, cubeVertices, 288 * sizeof(float));
	this->graphicsBufferSize = 0;
}
Blocks::BlockType::~BlockType() {}

void Blocks::BlockType::GenerateGraphicsBuffers()
{
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->vertexVBO);
	glGenBuffers(1, &this->instanceVBO);

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);

	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 3);
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 3, 1);
	}

	this->graphicsLoaded = true;
}
void Blocks::BlockType::RemoveGraphicsBuffers()
{
	glDeleteBuffers(1, &this->vertexVBO);
	glDeleteBuffers(1, &this->instanceVBO);
	glDeleteVertexArrays(1, &this->VAO);
	this->graphicsLoaded = false;
}

void Blocks::BlockType::BindGraphicsBuffers()
{
	int oldSize = this->models.size();
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	if (this->models.size() > this->graphicsBufferSize)
	{
		glBufferData(GL_ARRAY_BUFFER, this->models.size() * sizeof(glm::mat4), &this->models[0], GL_STATIC_DRAW);
		return;
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, this->models.size() * sizeof(glm::mat4), &this->models[0]);
}

std::string Blocks::BlockType::GetName()
{
	return this->name;
}
void Blocks::BlockType::SetName(std::string _name)
{
	this->name = _name;
}

void Blocks::BlockType::SetVertices()
{
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(float), &this->vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Texture attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Lightning normal attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

}
void Blocks::BlockType::RegenerateMatrices()
{
	// models -> static resize
	// modelsAddresses -> constant resize

	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	if (this->modelsAddresses.size() > this->graphicsBufferSize)
	{
		this->graphicsBufferSize = this->models.size();

		this->models.resize(this->graphicsBufferSize);
		//std::memcpy(this->models.data(), this->modelsAddresses.data(), this->graphicsBufferSize * sizeof(glm::mat4)); // not working cuz we're using pointers
		glBufferData(GL_ARRAY_BUFFER, this->models.size() * sizeof(glm::mat4), &this->models[0], GL_STATIC_DRAW);
		return;
	}
	std::memcpy(this->models.data(), this->modelsAddresses.data(), this->graphicsBufferSize * sizeof(glm::mat4)); // not working cuz we're using pointers
	//size_t numBytes = (vec.size() - startIndex) * sizeof(vec[0]);
	// Use std::memset to set the specified range to zero
	//std::memset(vec.data() + startIndex, 0, numBytes);

	glBufferSubData(GL_ARRAY_BUFFER, 0, this->models.size() * sizeof(glm::mat4), &this->models[0]);
	//models.clear();
	//models.resize(modelsAddresses.size());
	//for (size_t i = 0; i < models.size(); i++)
	//{
	//	models[i] = *modelsAddresses[i];
	//}
	//BindGraphicsBuffers();
}



void Blocks::BlockType::InsertModel(glm::mat4* _model)
{
	modelsAddresses.push_back(_model);
	this->AskForRefresh();
}

void Blocks::BlockType::RemoveModel(glm::mat4* _model)
{
	for (size_t i = 0; i < modelsAddresses.size(); i++)
	{
		if (modelsAddresses[i] == _model)
		{
			modelsAddresses.erase(modelsAddresses.begin() + i);
			this->AskForRefresh();
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

glm::vec3 Blocks::BlockType::GetScale()
{
	return this->scale;
}

void Blocks::BlockType::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
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
		if (this->refreshInQueue)
		{
			RegenerateMatrices();
			this->refreshInQueue = false;
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

		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, this->models.size());

		glBindVertexArray(0);
		this->shader->setBool("instanceUsage", false);
	}
}


void Blocks::BlockType::AskForRefresh()
{
	refreshInQueue = true;
}