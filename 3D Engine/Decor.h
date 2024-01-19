#ifndef DECOR_H
#define DECOR_H
#include "Scene.h"
#include <glm/fwd.hpp>
#include "Instances.h"
#include "Bounds.h"

#define DECOR_DIRECTORY "../Decors/"
#define DECOR_FILE_EXTENSION ".json"
#define DECOR_SCALE 0.20f

namespace Decors
{
	class Decor : public Scene::World::Component
	{
	public:
		Decor(std::string _name);
		~Decor();

		void Draw() override;
		Cube* Raycast() override;
		void InsertObject(Cube _cube) override;
		void RemoveObject(Cube* _cube) override;

		bool LoadFromFile(const char* _name) override;
		void Save() override;

		void ApplyOffset(glm::vec3 _offset) override;

		bool IsInstanced() override;
		void SetInstanced(bool _instanced) override;
		void LoadInstances() override;


		void CalculateBoundingBox() override;
		Bounds::Box GetBoundingBox() override;

	private:
		std::vector<Cube> objects;
		std::vector<glm::mat4*> models;
		std::vector<Instances::Cube> instances;
		bool instanced;

		Bounds::BoundingBox boundingBox;
	};

}

#endif // !DECOR_H