#include "Blocks.h"

Blocks::Block::Block() {}
Blocks::Block::~Block() {}

glm::vec3 Blocks::Block::GetPosition() { return this->position; }
void Blocks::Block::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Move(glm::vec3 _position)
{
	this->position += _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
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
}

glm::vec3 Blocks::Block::GetRotation() { return this->rotation; }
void Blocks::Block::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
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
}

glm::vec3 Blocks::Block::GetScale() { return this->scale; }
void Blocks::Block::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Scale(glm::vec3 _scale)
{
	this->scale += _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
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
}