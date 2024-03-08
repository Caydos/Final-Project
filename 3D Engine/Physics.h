#ifndef PHYSICS_H
#define PHYSICS_H
#include "Bounds.h"
#include <glm/vec3.hpp>

namespace Physics
{
	float CalculateCrossSectionalArea(const Bounds::Box& box); //A

	float CalculateDragCoefficient(float aerodynamic); // Fdrag

	class Body
	{
	public:
		Body();
		~Body();

		float aerodynamic; // from 0 to 1.0f
		float weight;
		Bounds::Box boundingBox;
		glm::vec3 position;
		glm::vec3 velocity;//speed

		void Update(float _dt);
	};
}



#endif // !PHYSICS_H