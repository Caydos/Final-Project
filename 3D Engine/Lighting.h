#ifndef LIGHTNING_H
#define LIGHTNING_H
#include <glm/vec3.hpp>
#include "Files.h"
#include "FrustumCulling.h"

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

	struct Directional
	{
		glm::vec3 direction;

		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;

		bool active;
	};

	struct Spot
	{
		glm::vec3 position;
		glm::vec3 direction;
		float cutOff;
		float outerCutOff;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float constant;
		float linear;
		float quadratic;

		bool active;
	};
}




#endif // !LIGHTNING_H