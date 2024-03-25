#include "Peds.h"


Peds::Ped::Ped()
{
	this->camera = nullptr;
	this->body.velocity = glm::vec3(.0f);
	this->body.aerodynamic = 0.1f;
	this->body.boundingBox = this->GetBoundingBox();
	this->speed = 1.0f;
	this->runningMultiplier = 2.5f;
	this->running = true;
}

Peds::Ped::~Ped()
{
}


Camera* Peds::Ped::GetCamera()
{
	return this->camera;
}

void Peds::Ped::SetCamera(Camera* _camera)
{
	this->camera = _camera;
}

Physics::Body Peds::Ped::GetBody()
{
	return this->body;
}

void Peds::Ped::PushVelocity(glm::vec3 _velocity, bool _movementVelocity)
{
	this->body.velocity += _velocity;
	if (_movementVelocity)
	{
		this->movementVelocity += _velocity;
	}
}

void Peds::Ped::Update()
{
	if (this->camera != nullptr)
	{
		this->camera->Position = this->GetPosition();
		this->SetRotation(glm::vec3(.0f, -this->camera->Yaw + 90.0, .0f));
	}
	this->Move(this->body.velocity, true);
	this->running = false;

	// Reset states
	this->body.velocity -= this->movementVelocity;
	this->movementVelocity = glm::vec3(.0f);
}

float Peds::Ped::GetSpeed()
{
	return this->speed;
}

void Peds::Ped::SetSpeed(float _speed)
{
	this->speed = _speed;
}

float Peds::Ped::GetRunningMultiplier()
{
	return this->runningMultiplier;
}

void Peds::Ped::SetRunningMultiplier(float _multiplier)
{
	this->runningMultiplier = _multiplier;
}

void Peds::Ped::Run()
{
	this->running = true;
}


std::vector<Peds::Ped*> pedPool;

Peds::Ped* Peds::Create()
{
	Ped* ped = new Ped();
	pedPool.push_back(ped);
	Sets::Insert(ped);
	return ped;
}


void Peds::Erase(Ped* _ped, bool _eraseModel)
{

}

std::vector<Peds::Ped*>* Peds::GetPool()
{
	return &pedPool;
}
