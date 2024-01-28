#include "Blocks.h"

Blocks::Block::Block()
{
	this->inScene = false;
	this->type = nullptr;
	this->parent = nullptr;
	this->model = nullptr;
}
Blocks::Block::~Block() {}

Blocks::BlockType* Blocks::Block::GetType()
{
	return this->type;
}

void Blocks::Block::SetType(BlockType* _type)
{
	this->type = _type;
}

void Blocks::Block::InsertInScene()
{
	if (!this->inScene && this->type != nullptr)
	{
		this->inScene = true;
		this->type->InsertModel(this->model);
	}
}

void Blocks::Block::RemoveFromScene()
{
	if (this->inScene && this->type != nullptr)
	{
		this->inScene = false;
		this->type->RemoveModel(this->model);
	}
}

void Blocks::Block::GenerateModel()
{
	this->model = new glm::mat4(1.0f);
}

void Blocks::Block::EraseModel()
{
	if (this->model != nullptr)
	{
		delete this->model;
	}
}

void Blocks::Block::ApplyTransformation()
{
	if (this->parent != nullptr)
	{
		*this->model = *parent;
	}
	else
	{
		*this->model = glm::mat4(1.0f);
	}

	*this->model = glm::translate(*this->model, this->position);

	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->model = glm::rotate(*this->model, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	*this->model = glm::scale(*this->model, this->scale);
}

glm::vec3 Blocks::Block::GetPosition() { return this->position; }
void Blocks::Block::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Move(glm::vec3 _position)
{
	this->position += _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;

	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetRotation() { return this->rotation; }
void Blocks::Block::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetScale() { return this->scale; }
void Blocks::Block::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Scale(glm::vec3 _scale)
{
	this->scale += _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}
void Blocks::Block::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
	ApplyTransformation();
}