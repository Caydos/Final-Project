#include "Blocks.h"

Blocks::Block::Block()
{
	this->inScene = false;
	this->type = nullptr;
	this->parent = nullptr;
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

glm::mat4* Blocks::Block::GetParent()
{
	return this->parent;
}

void Blocks::Block::SetParent(glm::mat4* _parent)
{
	this->parent = _parent;
	this->ApplyTransformation();
}

void Blocks::Block::GenerateModel()
{
	this->model = new glm::mat4;
	*this->model = glm::mat4(1.0f);
}

glm::mat4 Blocks::Block::GetModel()
{
	return *this->model;
}

glm::mat4* Blocks::Block::GetModelAddress()
{
	return this->model;
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

	this->boundingBox.position = this->position;

	// The extents are half the size of the cube, factoring in the scale.
	// Assuming the cube initially spans from -1 to 1 (so has a length of 2) before scaling.
	this->boundingBox.extents = this->scale * 0.5f;

	glm::vec3 rotationRadians = glm::radians(this->rotation);

	// Create a rotation matrix from the Euler angles
	glm::mat4 rotMat(1.0f);
	rotMat = glm::rotate(rotMat, rotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis rotation
	rotMat = glm::rotate(rotMat, rotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
	rotMat = glm::rotate(rotMat, rotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis rotation

	// Extract the rotational part as a mat3 from the transformation matrix.
	this->boundingBox.rotation = glm::mat3(rotMat);
}

glm::vec3 Blocks::Block::GetPosition() { return this->position; }
void Blocks::Block::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	ApplyTransformation();
}
void Blocks::Block::Move(glm::vec3 _position)
{
	this->position += _position;
	ApplyTransformation();
}
void Blocks::Block::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetRotation() { return this->rotation; }
void Blocks::Block::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	ApplyTransformation();
}
void Blocks::Block::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	ApplyTransformation();
}
void Blocks::Block::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;
	ApplyTransformation();
}

glm::vec3 Blocks::Block::GetScale() { return this->scale; }
void Blocks::Block::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);
	ApplyTransformation();
}
void Blocks::Block::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);
	ApplyTransformation();
}
void Blocks::Block::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
	ApplyTransformation();
}
void Blocks::Block::Scale(glm::vec3 _scale)
{
	this->scale += _scale;
	ApplyTransformation();
}
void Blocks::Block::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;
	ApplyTransformation();
}

Bounds::Box Blocks::Block::GetBoundingBox()
{
	return boundingBox;
}
