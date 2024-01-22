#ifndef ENTITY_H
#define ENTITY_H
#include "Scene.h"
#include "Bones.h"

#define ENTITY_DIRECTORY "../Entities/"
#define ENTITY_FILE_EXTENSION ".json"
#define ENTITY_SCALE 0.05f
#include <glm/vec3.hpp>

namespace Entities
{
	class Entity : public Scene::World::Component
	{
	public:
		Entity();
		~Entity();


		void Draw() override;
		Cube* Raycast() override;
		void InsertObject(Cube _cube) override;
		void RemoveObject(Cube* _cube) override;

		bool LoadFromFile(const char* _name) override;
		void Save() override;

		void ApplyOffset(glm::vec3 _offset) override;
		void LoadInstances() override;

		void CalculateBoundingBox() override;

		Skeleton* GetSkeleton()
		{
			return &this->skeleton;
		}
	private:
		Skeleton skeleton;
	};

	void Tick(GameData* _gameData);
}

#endif // !ENTITY_H
