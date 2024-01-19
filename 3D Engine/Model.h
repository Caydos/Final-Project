#ifndef MODEL_H
#define MODEL_H
#include "Scene.h"
#include "Instances.h"

#define MODEL_DIRECTORY "../Models/"
#define MODEL_FILE_EXTENSION ".json"
#define MODEL_SCALE 0.05f

namespace Models
{
	class Model : public Scene::World::Component
	{
	public:
		Model(std::string _name);
		~Model();

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

#endif // !MODEL_H