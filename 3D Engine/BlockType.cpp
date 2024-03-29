#include "Blocks.h"

Blocks::BlockType::BlockType()
{
}

Blocks::BlockType::~BlockType() {}

std::string Blocks::BlockType::GetName()
{
	return this->name;
}
void Blocks::BlockType::SetName(std::string _name)
{
	this->name = _name;
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

Texture* Blocks::BlockType::GetEffectsTexture()
{
	return this->effects;
}

void Blocks::BlockType::SetEffectsTexture(Texture* _effectsTexture)
{
	this->effects = _effectsTexture;
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

Blocks::Instance* Blocks::BlockType::AddInstance(void* _instanceId)
{
	Instance* newInstance = new Instance;
	newInstance->GenerateGraphicsBuffers();
	newInstance->SetVertices();
	newInstance->SetId(_instanceId);
	this->instances.push_back(newInstance);
	return newInstance;
}

void Blocks::BlockType::RemoveInstance(void* _instance)
{
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		if (this->instances[i] == _instance)
		{
			this->instances[i]->RemoveGraphicsBuffers();
			delete this->instances[i];
			this->instances.erase(this->instances.begin() + i);
			return;
		}
	}
}

void Blocks::BlockType::SetInstanceVisibility(void* _instanceId, bool _visible)
{
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		if (this->instances[i]->GetId() == _instanceId)
		{
			this->instances[i]->SetVisibility(_visible);
			return;
		}
	}
}

void Blocks::BlockType::InsertInInstance(void* _instanceId, glm::mat4* _model)
{
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		if (this->instances[i]->GetId() == _instanceId)
		{
			this->instances[i]->InsertModel(_model);
			//this->instances[i]->RegenerateMatrices();
			return;
		}
	}
	std::cout << "Instance not found" << std::endl;
}

void Blocks::BlockType::RemoveFromInstance(void* _instanceId, glm::mat4* _model)
{
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		if (this->instances[i]->GetId() == _instanceId)
		{
			this->instances[i]->RemoveModel(_model);
			this->instances[i]->RegenerateMatrices();
			return;
		}
	}
}

void Blocks::BlockType::AskForRefresh(void* _instanceId)
{
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		if (this->instances[i]->GetId() == _instanceId)
		{
			this->instances[i]->AskForRefresh();
			return;
		}
	}
}

void Blocks::BlockType::Draw()
{
	//this->shader->setBool("instanceUsage", true);

	int mode = 0;
	if (this->texture != nullptr)
	{
		mode++;
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, this->texture->id);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, this->effects->id);

		if (this->color.values[3] > 0.0f)
		{
			mode++;
		}
	}

	shader->setBool("lightDependent", this->lightDependent);
	shader->setInt("mode", mode);
	shader->setFloat("shininess", this->shininess);

	//shader->setVec3("diffuse", this->diffuse);
	//shader->setVec3("material.specular", this->specular);
	//shader->setFloat("material.shininess", this->shininess);
	for (size_t i = 0; i < this->instances.size(); i++)
	{
		this->instances[i]->Draw();
	}
}
