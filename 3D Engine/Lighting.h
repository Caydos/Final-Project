#ifndef LIGHTNING_H
#define LIGHTNING_H
#include <glm/vec3.hpp>
#include "Files.h"
#include "FrustumCulling.h"

#define MAX_LIGHTS 300

namespace Lighting
{
	float CalculateLightRange(float constant, float linear, float quadratic, float threshold = 0.05);

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
		Light();
		~Light();

		std::string GetName();
		void SetName(std::string _name);


		bool IsVisible();
		void CheckVisibility();
		void CalculateRange();

		bool IsActive();
		void SetActive(bool _status = true);

		LightType GetType();
		void SetType(LightType _type);

		glm::vec3 GetPosition();
		void SetPosition(glm::vec3 _position);
		glm::vec3 GetDirection();
		void SetDirection(glm::vec3 _direction);

		float GetCutOff();
		void SetCutOff(float _cutOff);
		float GetOuterCutOff();
		void SetOuterCutOff(float _outerCutOff);

		float GetConstant();
		void SetConstant(float _constant);
		float GetLinear();
		void SetLinear(float _linear);
		float GetQuadratic();
		void SetQuadratic(float _quadratic);

		glm::vec3 GetAmbient();
		void SetAmbient(glm::vec3 _ambient);
		glm::vec3 GetDiffuse();
		void SetDiffuse(glm::vec3 _diffuse);
		glm::vec3 GetSpecular();
		void SetSpecular(glm::vec3 _specular);

		json Encode();

		void LoadFromJson(json _content);

	private:

		std::string name;
		bool visible;
		float range;
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