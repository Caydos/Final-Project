#ifndef SCENE_H
#define SCENE_H

#include "Common.h"
#include <glm/vec3.hpp>
#include "Lightning.h"
#include "Cube.h"
#include "Bounds.h"

#define SCENE_DIRECTORY "../Scenes/"
#define SCENE_FILE_EXTENSION ".json"

namespace Scene
{
	enum Type
	{
		WORLD,
		MODEL_EDITOR,
		LEVEL_EDITOR,
		VFX_EDITOR
	};

	const char** GetMaterialsAsStringArray();
	int GetMaterialsCount();
	std::vector<Material>* GetMaterials();

	Colors::Color GetClearColor();
	void SetClearColor(Colors::Color _color);

	void Initialize(GameData* _gameData);
	void Inputs(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();

	namespace Lights
	{
		Lightning::Light* InsertLight(GameData* _gameData, Lightning::Light& _light);
		void UpdateLight(GameData* _gameData, unsigned int _id);
		void UpdateShader(GameData* _gameData);
		std::vector<Lightning::Light>* GetLights();
		void Remove(Lightning::Light* _lightAddr);
	}

	namespace World
	{
		enum ComponentType
		{
			DECOR,
			MODEL,
			ENTITY
		};

		class Component
		{
		public:
			Component();
			~Component();

			bool IsVisible();
			void SetVisible(bool _state = true);

			std::string GetName();
			void SetName(std::string _name);

			virtual void Draw() = 0;
			virtual void Save() = 0;
			virtual Cube* Raycast() = 0;
			virtual void InsertObject(Cube _cube) = 0;
			virtual void RemoveObject(Cube* _cube) = 0;
			virtual bool LoadFromFile(const char* _name) = 0;

			Cube* GetRaycastHit();
			void SetRaycastHit(Cube* _hit);

			float GetScale();
			void SetScale(float _scale);

			glm::mat4* GetMatrix();

			glm::vec3 GetPosition();
			void SetPosition(glm::vec3 _position);
			void Move(glm::vec3 _offset);
			glm::vec3 GetRotation();
			void SetRotation(glm::vec3 _rotation);
			void Rotate(glm::vec3 _rotation);

			bool IsInstanced();
			void SetInstanced(bool _instanced);
			virtual void LoadInstances() = 0;

			bool AreBoundsVisible();
			void ShowBounds(bool _displayed);
			virtual void CalculateBoundingBox() = 0;
			Bounds::Box GetBoundingBox();
			void SetBoundingBox(Bounds::Box _box);


			bool IsUpdated();
			void SetUpdated(bool _updated);
		private:
			Cube* raycastHit;

			bool visible;
			bool instanced;

			std::string name;
			ComponentType type;
			float scale;

			glm::mat4* matrix;
			bool useOffset;
			glm::vec3 position;
			glm::vec3 rotation;

			bool showBounds;
			Bounds::Box boundingBox;
			bool updated;
		};

		void InsertComponent(Component* _component);
		void RemoveComponent(Component* _component);
		std::vector <Component*>* GetComponents();
		void QueueLoading(Component* _component, std::string _fileName);

		void ConsiderLightning(bool _value = true);
		bool IsConsideringLightning();
		
		unsigned int NewCamera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH);
		void FocusCamera(GameData* _gameData, unsigned int _cameraId);
		void SetCameraPosition(unsigned int _cameraId, glm::vec3 _position);
		void ProcessCameraInput(GameData* _gameData, unsigned int _cameraId, Camera_Movement _movement);
		Camera* GetCamera();
		float GetCameraFov();
		void SetCameraFov(float _newFov);
		glm::mat4 GetProjection();
		glm::mat4 GetView();


		void MouseInputs(GameData* _gameData);
		void Render(GameData* _gameData);
		void CleanUp();
	}
}

#endif // !SCENE_H