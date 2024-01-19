#ifndef LIGHTNING_H
#define LIGHTNING_H
#include <glm/vec3.hpp>
#include "Files.h"

#define MAX_LIGHTS 40

namespace Lightning
{
	enum LightType
	{
		DIRECTIONAL,/*Applied to all, from a direction vector*/
		POINT, /*Spread in every directions*/
		SPOT /*Flashlight kind*/
	};
	static const char* LightTypes[] = { "None", "Directional Light", "Point Light", "Spot Light" };

	class Light
	{
	public:
		Light()
		{
			this->active = false;
		};
		~Light() {};

		std::string GetName() { return this->name; }
		void SetName(std::string _name) { this->name = _name; }

		bool IsActive() { return this->active; }
		void SetActive(bool _status = true)
		{
			this->active = _status;
		}

		LightType GetType() { return this->type; }
		void SetType(LightType _type) { this->type = _type; }

		glm::vec3 GetPosition() { return this->position; }
		void SetPosition(glm::vec3 _position) { this->position = _position; }
		glm::vec3 GetDirection() { return this->direction; }
		void SetDirection(glm::vec3 _direction) { this->direction = _direction; }

		float GetCutOff() { return this->cutOff; }
		void SetCutOff(float _cutOff) { this->cutOff = _cutOff; }
		float GetOuterCutOff() { return this->outerCutOff; }
		void SetOuterCutOff(float _outerCutOff) { this->outerCutOff = _outerCutOff; }

		float GetConstant() { return this->constant; }
		void SetConstant(float _constant) { this->constant = _constant; }
		float GetLinear() { return this->linear; }
		void SetLinear(float _linear) { this->linear = _linear; }
		float GetQuadratic() { return this->quadratic; }
		void SetQuadratic(float _quadratic) { this->quadratic = _quadratic; }

		glm::vec3 GetAmbient() { return this->ambient; }
		void SetAmbient(glm::vec3 _ambient) { this->ambient = _ambient; }
		glm::vec3 GetDiffuse() { return this->diffuse; }
		void SetDiffuse(glm::vec3 _diffuse) { this->diffuse = _diffuse; }
		glm::vec3 GetSpecular() { return this->specular; }
		void SetSpecular(glm::vec3 _specular) { this->specular = _specular; }

		json Encode()
		{
			json object;

			object["name"] = name;
			object["active"] = active;
			object["type"] = static_cast<int>(type);

			object["position"] = { position.x, position.y, position.z };
			object["direction"] = { direction.x, direction.y, direction.z };

			object["cutOff"] = cutOff;
			object["outerCutOff"] = outerCutOff;

			object["constant"] = constant;
			object["linear"] = linear;
			object["quadratic"] = quadratic;

			object["ambient"] = { ambient.x, ambient.y, ambient.z };
			object["diffuse"] = { diffuse.x, diffuse.y, diffuse.z };
			object["specular"] = { specular.x, specular.y, specular.z };

			return object;
		}

		void LoadFromJson(json _content)
		{
			if (_content.contains("name"))
			{
				this->SetName(_content["name"]);
			}
			if (_content.contains("active"))
			{
				this->SetActive(_content["active"]);
			}
			if (_content.contains("type"))
			{
				this->SetType(_content["type"]);
			}

			if (_content.contains("position"))
			{
				this->SetPosition(glm::vec3(_content["position"][0], _content["position"][1], _content["position"][2]));
			}
			if (_content.contains("direction"))
			{
				this->SetDirection(glm::vec3(_content["direction"][0], _content["direction"][1], _content["direction"][2]));
			}

			if (_content.contains("cutOff"))
			{
				this->SetCutOff(_content["cutOff"]);
			}
			if (_content.contains("outerCutOff"))
			{
				this->SetOuterCutOff(_content["outerCutOff"]);
			}

			if (_content.contains("constant"))
			{
				this->SetConstant(_content["constant"]);
			}
			if (_content.contains("linear"))
			{
				this->SetLinear(_content["linear"]);
			}
			if (_content.contains("quadratic"))
			{
				this->SetQuadratic(_content["quadratic"]);
			}

			if (_content.contains("ambient"))
			{
				this->SetAmbient(glm::vec3(_content["ambient"][0], _content["ambient"][1], _content["ambient"][2]));
			}
			if (_content.contains("diffuse"))
			{
				this->SetDiffuse(glm::vec3(_content["diffuse"][0], _content["diffuse"][1], _content["diffuse"][2]));
			}
			if (_content.contains("specular"))
			{
				this->SetSpecular(glm::vec3(_content["specular"][0], _content["specular"][1], _content["specular"][2]));
			}
		}

	private:

		std::string name;
		bool active;

		LightType type;

		glm::vec3 position;
		glm::vec3 direction;

		float cutOff;
		float outerCutOff;


		float constant;
		float linear;
		float quadratic;


		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
	};
}




#endif // !LIGHTNING_H