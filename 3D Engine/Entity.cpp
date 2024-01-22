#include "Entity.h"

Entities::Entity::Entity()
{
}

Entities::Entity::~Entity()
{
}

void Entities::Entity::Draw()
{
	this->skeleton.Draw();
}

Cube* Entities::Entity::Raycast()
{
	return nullptr;
}

void Entities::Entity::InsertObject(Cube _cube)
{
}

void Entities::Entity::RemoveObject(Cube* _cube)
{
}

bool Entities::Entity::LoadFromFile(const char* _name)
{
	return false;
}

void Entities::Entity::Save()
{
}

void Entities::Entity::ApplyOffset(glm::vec3 _offset)
{
}


void Entities::Entity::LoadInstances()
{
}


void Entities::Entity::CalculateBoundingBox()
{
}



static bool initialized = false;
static Skeleton* skeleton = nullptr;

void Entities::Tick(GameData* _gameData)
{
	if (!initialized)
	{
		Entity* entity = new Entity;
		
		Scene::World::InsertComponent(entity);
		skeleton = entity->GetSkeleton();
		skeleton->InsertBones();
		// Tes loads ici
		initialized = true;
	}
	//Update

}
