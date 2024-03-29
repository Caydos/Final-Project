#include "Blocks.h"
#include "DefaultVertices.h"

Blocks::Instance::Instance()
{
	this->graphicsLoaded = false;
	this->visible = false;
}
Blocks::Instance::~Instance() {}

void* Blocks::Instance::GetId()
{
	return this->id;
}

void Blocks::Instance::SetId(void* _id)
{
	this->id = _id;
}


void Blocks::Instance::GenerateGraphicsBuffers()
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
void Blocks::Instance::RemoveGraphicsBuffers()
{
	if (!this->graphicsLoaded) { return; }
	glDeleteBuffers(1, &this->vertexVBO);
	glDeleteBuffers(1, &this->instanceVBO);
	glDeleteVertexArrays(1, &this->VAO);
	this->graphicsLoaded = false;
}

void Blocks::Instance::BindGraphicsBuffers()
{
	if (!this->graphicsLoaded) { return; }
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, this->models.size() * sizeof(glm::mat4), &this->models[0], GL_STATIC_DRAW);
}

void Blocks::Instance::SetVertices()
{
	if (!this->graphicsLoaded) { return; }
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->vertexVBO);
	glBufferData(GL_ARRAY_BUFFER, 288 * sizeof(float), &cubeVertices, GL_STATIC_DRAW);

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

void Blocks::Instance::RegenerateMatrices()
{
	this->models.clear();
	this->models.resize(this->modelsAddresses.size());
	for (size_t i = 0; i < this->models.size(); i++)
	{
		this->models[i] = *modelsAddresses[i];
	}
	BindGraphicsBuffers();
}



void Blocks::Instance::InsertModel(glm::mat4* _model)
{
	this->modelsAddresses.push_back(_model);
	this->AskForRefresh();
}

void Blocks::Instance::RemoveModel(glm::mat4* _model)
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




void Blocks::Instance::Draw()
{
	if (!this->visible) { return; }
	if (this->graphicsLoaded)
	{
		if (this->refreshInQueue)
		{
			RegenerateMatrices();
			this->refreshInQueue = false;
		}
		// create transformations
		glBindVertexArray(this->VAO);

		glDrawArraysInstanced(GL_TRIANGLES, 0, 36, this->models.size());

		glBindVertexArray(0);
	}
	else
	{
		this->GenerateGraphicsBuffers();
		this->SetVertices();
		this->BindGraphicsBuffers();
	}
}


void Blocks::Instance::AskForRefresh()
{
	refreshInQueue = true;
}

void Blocks::Instance::SetVisibility(bool _visible)
{
	this->visible = _visible;
}
