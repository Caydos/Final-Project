#include "Physics.h"

const float g = 9.81f / 8; // gravitational acceleration
const float airDensity = 1.225f; // density of air in kg/m^3

float Physics::CalculateCrossSectionalArea(const Bounds::Box& box) //A
{
	// Simplified calculation: use the width and depth of the bounding box as the cross-sectional area
	float width = box.max.x - box.min.x;
	float depth = box.max.z - box.min.z;
	return width * depth;
}

float Physics::CalculateDragCoefficient(float aerodynamic) // Fdrag
{
	const float minCd = 0.2f; // Minimum drag coefficient for highly aerodynamic shapes
	const float maxCd = 1.3f; // Maximum drag coefficient for non-aerodynamic shapes

	// Linearly interpolate between minCd and maxCd based on the aerodynamic value
	float Cd = minCd + (maxCd - minCd) * (1.0f - aerodynamic);

	return Cd;
}


Physics::Body::Body()
{
}

Physics::Body::~Body()
{
}

void Physics::Body::Update(float _dt)
{
	if (_dt <= 0.0f)
	{
		return;
	}
	this->velocity.y += -g * _dt;

	if (this->weight <= 0.0f)
	{
		return;
	}

	float Cd = CalculateDragCoefficient(this->aerodynamic);

	// Calculate cross-sectional area
	float area = CalculateCrossSectionalArea(this->boundingBox);

	// Calculate the drag force
	float speed = glm::length(this->velocity);
	if (speed == 0.0f)
	{
		std::cout << "Body is stationary. No drag force applied." << std::endl;
	}
	else
	{
		float dragForceMagnitude = 0.5f * Cd * airDensity * area * speed * speed;
		glm::vec3 dragForce = -dragForceMagnitude * glm::normalize(this->velocity);

		// Apply drag force to velocity
		glm::vec3 dragAcceleration = dragForce / this->weight; // Assuming body has a 'weight' attribute
		this->velocity += dragAcceleration * _dt;
		//std::cout << "Speed : " << speed << std::endl;
		//std::cout << "Velocity : " << this->velocity.y << std::endl;
	}
}