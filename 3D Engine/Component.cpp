#include "Scene.h"

Scene::World::Component::Component()
{
	this->scale = 1.0f;
	this->visible = true;
	this->raycastHit = nullptr;
	this->useOffset = false;
	this->matrix = new glm::mat4;
	*this->matrix = glm::mat4(1.0f);
}

Scene::World::Component::~Component()
{
	delete this->matrix;
}

bool Scene::World::Component::IsVisible()
{
	return this->visible;
}

void Scene::World::Component::SetVisible(bool _state)
{
	this->visible = _state;
}

std::string Scene::World::Component::GetName()
{
	return this->name;
}

void Scene::World::Component::SetName(std::string _name)
{
	this->name = _name;
}

void Scene::World::Component::CheckVisibility()
{
	//if (FrustrumCulling::IsBoxInFrustum(projection, view, boundingBox.min, boundingBox.max))
	//{

	//}
}

Cube* Scene::World::Component::GetRaycastHit()
{
	return this->raycastHit;
}

void Scene::World::Component::SetRaycastHit(Cube* _hit)
{
	this->raycastHit = _hit;
}

float Scene::World::Component::GetScale()
{
	return this->scale;
}

void Scene::World::Component::SetScale(float _scale)
{
	this->scale = _scale;
}

glm::mat4* Scene::World::Component::GetMatrix()
{
	std::cout << "Init matrix : " << this->matrix << std::endl;
	return this->matrix;
}

glm::vec3 Scene::World::Component::GetPosition()
{
	return this->position;
}

void Scene::World::Component::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	*this->matrix = glm::mat4(1.0f);
	*this->matrix = glm::translate(*this->matrix, _position);
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


	this->SetUpdated(true);
}

void Scene::World::Component::Move(glm::vec3 _offset)
{
	this->position += _offset;
	*this->matrix = glm::translate(*this->matrix, _offset);

	this->SetUpdated(true);
}

glm::vec3 Scene::World::Component::GetRotation()
{
	return this->rotation;
}

void Scene::World::Component::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	*this->matrix = glm::mat4(1.0f);
	*this->matrix = glm::translate(*this->matrix, this->position);
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(this->rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));


	this->SetUpdated(true);
}

void Scene::World::Component::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	*this->matrix = glm::rotate(*this->matrix, glm::radians(_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	*this->matrix = glm::rotate(*this->matrix, glm::radians(_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	this->SetUpdated(true);
}

bool Scene::World::Component::IsInstanced()
{
	return this->instanced;
}

void Scene::World::Component::SetInstanced(bool _instanced)
{
	this->instanced = _instanced;
}


bool Scene::World::Component::AreBoundsVisible()
{
	return this->showBounds;
}
void Scene::World::Component::ShowBounds(bool _displayed)
{
	this->showBounds = _displayed;
}

Bounds::Box Scene::World::Component::GetBoundingBox()
{
	return this->boundingBox;
}

void Scene::World::Component::SetBoundingBox(Bounds::Box _box)
{
	this->boundingBox = _box;
}

bool Scene::World::Component::IsUpdated()
{
	return this->updated;
}

void Scene::World::Component::SetUpdated(bool _updated)
{
	this->updated = _updated;
	std::cout << "Updated" << std::endl;
}
