#include "Peds.h"
#include "Collisions.h"

Peds::Ped::Ped()
{
	this->camera = nullptr;
	this->body.velocity = glm::vec3(.0f);
	this->body.aerodynamic = 0.1f;
	this->body.boundingBox = this->GetBoundingBox();
	this->speed = 2.5f;
	this->runningMultiplier = 2.0f;
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

Physics::Type Peds::Ped::GetBodyType()
{
	return this->body.type;
}

void Peds::Ped::SetBodyType(Physics::Type _type)
{
	this->body.type = _type;
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

	this->body.velocity.y -= 0.05f * std::min(_gameData->dt, 1.0f);
	for (size_t setId = 0; setId < sets.size(); setId++)
	{
		if (sets[setId] == this) { continue; }
		Bounds::Box setBox = this->GetBoundingBox();

		std::vector<Blocks::Block>* blocks = sets[setId]->GetBlocks();
		for (size_t blockId = 0; blockId < blocks->size(); blockId++)
		{
			this->body.velocity = Collisions::CalculateCollisionResponse(setBox, blocks->at(blockId).GetBoundingBox(), this->body.velocity);
		}
	}
	if (this->camera != nullptr)
	{
		this->camera->Position = this->GetPosition();
		this->SetRotation(glm::vec3(.0f, -this->camera->Yaw + 90.0, .0f));
	}
	this->Move(this->body.velocity, true);
	this->running = false;

	// Reset states
	//this->body.velocity -= this->movementVelocity;
	//this->body.velocity = glm::vec3(0);
	this->body.velocity.x = 0;
	//this->body.velocity.y = offset.y;
	this->body.velocity.z = 0;
	this->movementVelocity = glm::vec3(.0f);
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
