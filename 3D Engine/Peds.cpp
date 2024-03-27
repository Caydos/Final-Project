#include "Peds.h"
#include "Collisions.h"


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
void Peds::Ped::Simulate(GameData* _gameData)
{
	std::vector<Sets::Set*> sets = Sets::GetAllParents();
	for (size_t setId = 0; setId < sets.size(); setId++)
	{
		if (sets[setId] == this) { continue; }
		//std::vector<Sets::Set*> childs = sets->at(setId)->GetChilds();
		//bool skip = false;
		//for (size_t childId = 0; childId < childs.size(); childId++)
		//{
		//	if (childs[childId] == sets->at(setId))
		//	{
		//		skip = true;
		//		break;
		//	}
		//}
		//if (skip) { continue; }
		//Bounds::Box parentBox = sets[setId]->GetBoundingBox();
		//Collisions::Result rslt = Collisions::BoxColliding(this->GetBoundingBox(), parentBox);
		//if (rslt.isColliding)
		//{
		//	std::cout << this->GetName() << " - " << sets[setId]->GetName() << std::endl;
		//	for (size_t childId = 0; childId < length; childId++)
		//	{

		//	}
		//}
	}
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

void Peds::Simulate(GameData* _gameData)
{
	for (size_t pedId = 0; pedId < pedPool.size(); pedId++)
	{
		pedPool[pedId]->Simulate(_gameData);
	}
}
