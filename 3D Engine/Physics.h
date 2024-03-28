#ifndef PHYSICS_H
#define PHYSICS_H
#include "Bounds.h"
#include <glm/vec3.hpp>

namespace Physics
{
	float CalculateCrossSectionalArea(const Bounds::Box& box); //A

	float CalculateDragCoefficient(float aerodynamic); // Fdrag
	enum Type
	{
		GHOST, // Not concerned by anything
		RIGID, // Concerned by physics without deformation
		STATIC, // A rigid not concerned by physics
	};
	class Body
	{
	public:
		Body();
		~Body();

		Type type = STATIC;

		float aerodynamic; // from 0 to 1.0f
		float weight;
		Bounds::Box boundingBox;
		glm::vec3 position;
		glm::vec3 velocity;//speed

		void Update(float _dt);
	};
}



#endif // !PHYSICS_H