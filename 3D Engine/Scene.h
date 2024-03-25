#ifndef SCENE_H
#define SCENE_H

#include "Common.h"
#include <glm/vec3.hpp>
#include "Lightning.h"
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

	Colors::Color GetClearColor();
	void SetClearColor(Colors::Color _color);

	void Initialize(GameData* _gameData);
	void Inputs(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
	void SetInputUsage(bool _usage);

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


		//void QueueLoading(Component* _component, std::string _fileName); // for later sync

		void ConsiderLightning(bool _value = true, GameData* _gameData = nullptr);
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