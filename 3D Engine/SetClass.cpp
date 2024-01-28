#include "Set.h"
#include "FrustrumCulling.h"
#include "Scene.h"

Sets::Set::Set()
{
	this->visible = false;
	this->parent = nullptr;
	this->bone = nullptr;
}

Sets::Set::~Set()
{
}

void Sets::Set::Initialize()
{
	if (this->bone == nullptr)
	{
		this->bone = new glm::mat4(1.0f);
	}
}

void Sets::Set::Erase()
{
	if (this->bone != nullptr)
	{
		delete this->bone;
	}
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].EraseModel();
	}
}

std::string Sets::Set::GetName()
{
	return this->name;
}

void Sets::Set::SetName(std::string _name)
{
	this->name = _name;
}

void Sets::Set::CheckVisibility()
{

	if (FrustrumCulling::IsBoxInFrustum(Scene::World::GetProjection(), Scene::World::GetView(), this->boundingBox.min, this->boundingBox.max))
	{
		if (this->visible) { return; }
		this->visible = true;
		this->AppplyVisibility();
	}
	else
	{
		if (!this->visible) { return; }
		this->visible = false;
		this->AppplyVisibility();
	}
}

void Sets::Set::AppplyVisibility()
{
	if (this->visible)
	{
		for (size_t i = 0; i < this->blocks.size(); i++)
		{
			this->blocks[i].InsertInScene();
		}
		return;
	}
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].RemoveFromScene();
	}
}

























void Sets::Set::ApplyTransformation()
{
	if (this->parent != nullptr)
	{
		*this->bone = *parent;
	}
	else
	{
		*this->bone = glm::mat4(1.0f);
	}

	*this->bone = glm::translate(*this->bone, this->position);

	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->bone = glm::rotate(*this->bone, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	*this->bone = glm::scale(*this->bone, this->scale);

	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].ApplyTransformation();
	}
}

glm::vec3 Sets::Set::GetPosition() { return this->position; }
void Sets::Set::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Sets::Set::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	ApplyTransformation();
}
void Sets::Set::Move(glm::vec3 _position)
{
	this->position += _position;
	ApplyTransformation();
}
void Sets::Set::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;

	ApplyTransformation();
}

glm::vec3 Sets::Set::GetRotation() { return this->rotation; }
void Sets::Set::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);

	ApplyTransformation();
}
void Sets::Set::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;

	ApplyTransformation();
}
void Sets::Set::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;

	ApplyTransformation();
}
void Sets::Set::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;

	ApplyTransformation();
}

glm::vec3 Sets::Set::GetScale() { return this->scale; }
void Sets::Set::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);

	ApplyTransformation();
}
void Sets::Set::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);

	ApplyTransformation();
}
void Sets::Set::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;

	ApplyTransformation();
}
void Sets::Set::Scale(glm::vec3 _scale)
{
	this->scale += _scale;

	ApplyTransformation();
}
void Sets::Set::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;

	ApplyTransformation();
}

void Sets::Set::CalculateBoundingBox()
{
	glm::vec3 globalMin = glm::vec3(FLT_MAX);
	glm::vec3 globalMax = glm::vec3(-FLT_MAX);

	for (Blocks::Block& block : blocks)
	{
		glm::vec3 localMin = block.GetPosition() - block.GetScale() / 2.0f;
		glm::vec3 localMax = block.GetPosition() + block.GetScale() / 2.0f;

		globalMin = glm::min(globalMin, localMin);
		globalMax = glm::max(globalMax, localMax);
	}

	this->boundingBox = { globalMin, globalMax };
}

void Sets::Set::PlaceOriginBlock()
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		if (this->blocks[i].GetPosition() == glm::vec3(.0f))
		{
			return;
		}
	}
	Blocks::Block originBlock;
	originBlock.GenerateModel();
	originBlock.SetScale(0.2f);

	Blocks::MaterialCheck(&originBlock, nullptr);
	originBlock.InsertInScene();

	this->blocks.push_back(originBlock);
}

void Sets::Set::MoveOrigin(glm::vec3 _offset)
{
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].Move(_offset);
	}
}

void Sets::Set::MoveOrigin(float _x, float _y, float _z)
{
	glm::vec3 offset(_x,_y,_z);
	for (size_t i = 0; i < this->blocks.size(); i++)
	{
		this->blocks[i].Move(offset);
	}
}
