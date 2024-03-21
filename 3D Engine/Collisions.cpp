#include "Collisions.h"
#include "Scene.h"
#include "Crosshair.h"
#include "Set.h"

bool Collisions::PointCollidingBox(glm::vec3 _position, Bounds::Box _box)
{
	bool collisionX = _position.x >= _box.min.x && _position.x <= _box.max.x;
	bool collisionY = _position.y >= _box.min.y && _position.y <= _box.max.y;
	bool collisionZ = _position.z >= _box.min.z && _position.z <= _box.max.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

bool Collisions::BoxColliding(Bounds::Box _firstBox, Bounds::Box _secondBox)
{
	bool collisionX = _firstBox.min.x <= _secondBox.max.x && _firstBox.max.x >= _secondBox.min.x;
	bool collisionY = _firstBox.min.y <= _secondBox.max.y && _firstBox.max.y >= _secondBox.min.y;
	bool collisionZ = _firstBox.min.z <= _secondBox.max.z && _firstBox.max.z >= _secondBox.min.z;

	return collisionX && collisionY && collisionZ;
	//return false;
}

bool Collisions::IntersectRayWithBox(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 boxMin, glm::vec3 boxMax, float& t)
{
	glm::vec3 invDir = 1.0f / rayDirection;
	glm::vec3 tMin = (boxMin - rayOrigin) * invDir;
	glm::vec3 tMax = (boxMax - rayOrigin) * invDir;

	glm::vec3 t1 = glm::min(tMin, tMax);
	glm::vec3 t2 = glm::max(tMin, tMax);

	float tNear = glm::max(glm::max(t1.x, t1.y), t1.z);
	float tFar = glm::min(glm::min(t2.x, t2.y), t2.z);

	if (tNear > tFar || tFar < 0.0f) {
		return false;
	}

	t = tNear;  // t will hold the distance at which the intersection occurs
	return true;
}

std::vector<glm::vec3> ComputeVertices(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) {
	// Define the local vertices of a unit cube centered at the origin
	std::vector<glm::vec3> localVertices = {
		glm::vec3(-0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f, -0.5f, -0.5f),
		glm::vec3(0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f,  0.5f, -0.5f),
		glm::vec3(-0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f, -0.5f,  0.5f),
		glm::vec3(0.5f,  0.5f,  0.5f),
		glm::vec3(-0.5f,  0.5f,  0.5f)
	};


	// Create transformation matrix (scale, then rotate, then translate)
	glm::mat4 transform = glm::mat4(1.0f);
	transform = glm::scale(transform, scale);
	transform = glm::rotate(transform, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
	transform = glm::rotate(transform, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	transform = glm::rotate(transform, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	transform = glm::translate(transform, position);


	std::vector<glm::vec3> transformedVertices;
	transformedVertices.reserve(localVertices.size());

	// Apply the transformation to each vertex
	for (const auto& vertex : localVertices) {
		glm::vec4 transformedVertex = transform * glm::vec4(vertex, 1.0f);
		transformedVertices.push_back(glm::vec3(transformedVertex));
	}

	return transformedVertices;
}
bool CheckOverlap(const std::vector<glm::vec3>& axes, const std::vector<glm::vec3>& verticesA, const std::vector<glm::vec3>& verticesB) {
	for (const auto& axis : axes) {
		float minA = std::numeric_limits<float>::max();
		float maxA = std::numeric_limits<float>::lowest();
		float minB = std::numeric_limits<float>::max();
		float maxB = std::numeric_limits<float>::lowest();

		// Project all vertices of the first object onto the axis and find the min/max
		for (const auto& vertex : verticesA) {
			float projection = glm::dot(vertex, axis);
			minA = std::min(minA, projection);
			maxA = std::max(maxA, projection);
		}

		// Project all vertices of the second object onto the axis and find the min/max
		for (const auto& vertex : verticesB) {
			float projection = glm::dot(vertex, axis);
			minB = std::min(minB, projection);
			maxB = std::max(maxB, projection);
		}
		auto epsilon = 1e-8f;
		// Check for interval overlap; if none, no collision on this axis
		if (maxA < minB + epsilon || maxB < minA + epsilon) {
			return false; // Separating axis found, no collision
		}
	}

	return true; // No separating axis found, collision is true
}
bool AreColliding(Blocks::Block first, Blocks::Block second)
{
	std::vector<glm::vec3> verticesFirst = ComputeVertices(first.GetPosition(), first.GetRotation(), first.GetScale());
	std::vector<glm::vec3> verticesSecond = ComputeVertices(second.GetPosition(), second.GetRotation(), second.GetScale());

	// Local face normals in the cube's local space
	glm::vec3 localXNormal = glm::vec3(1, 0, 0);
	glm::vec3 localYNormal = glm::vec3(0, 1, 0);
	glm::vec3 localZNormal = glm::vec3(0, 0, 1);

	std::vector<glm::vec3> axes;
	{
		glm::vec3 rotationRadians = glm::radians(first.GetRotation());
		glm::mat4 rotMat(1.0f);
		rotMat = glm::rotate(rotMat, rotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis rotation
		rotMat = glm::rotate(rotMat, rotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
		rotMat = glm::rotate(rotMat, rotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis rotation
		// Rotate the normals using the rotation matrix
		glm::vec3 rotatedXNormal = rotMat * glm::vec4(localXNormal, 0.0f);
		glm::vec3 rotatedYNormal = rotMat * glm::vec4(localYNormal, 0.0f);
		glm::vec3 rotatedZNormal = rotMat * glm::vec4(localZNormal, 0.0f);
		axes.push_back(glm::normalize(rotatedXNormal));
		axes.push_back(glm::normalize(rotatedYNormal));
		axes.push_back(glm::normalize(rotatedZNormal));
	}
	{
		glm::vec3 rotationRadians = glm::radians(second.GetRotation());
		glm::mat4 rotMat(1.0f);
		rotMat = glm::rotate(rotMat, rotationRadians.z, glm::vec3(0.0f, 0.0f, 1.0f)); // Z-axis rotation
		rotMat = glm::rotate(rotMat, rotationRadians.y, glm::vec3(0.0f, 1.0f, 0.0f)); // Y-axis rotation
		rotMat = glm::rotate(rotMat, rotationRadians.x, glm::vec3(1.0f, 0.0f, 0.0f)); // X-axis rotation
		// Rotate the normals using the rotation matrix
		glm::vec3 rotatedXNormal = rotMat * glm::vec4(localXNormal, 0.0f);
		glm::vec3 rotatedYNormal = rotMat * glm::vec4(localYNormal, 0.0f);
		glm::vec3 rotatedZNormal = rotMat * glm::vec4(localZNormal, 0.0f);
		axes.push_back(glm::normalize(rotatedXNormal));
		axes.push_back(glm::normalize(rotatedYNormal));
		axes.push_back(glm::normalize(rotatedZNormal));
	}
	
	if (!CheckOverlap(axes, verticesFirst, verticesSecond)) {
		return false; // No overlap found on this axis, no collision
	}

	return true; // Overlap found on all axes, collision detected
}


static bool initialized = false;
static Sets::Set* first;
static Sets::Set* second;
void Collisions::Tick()
{
	if (!initialized)
	{
		{
			first = Sets::Create();
			first->Initialize();
			first->GenerateRenderingInstance();
			Blocks::Block block;
			block.GenerateModel();
			block.SetScale(0.2);
			Blocks::MaterialCheck(&block, "Clown_Brain");
			block.SetPosition(glm::vec3(0.0f));
			first->InsertBlock(block);
			first->SetName("First");
		}
		{
			//second = Sets::Create();
			//second->Initialize();
			//second->GenerateRenderingInstance();
			Blocks::Block block;
			block.GenerateModel();
			block.SetScale(0.2f);
			Blocks::MaterialCheck(&block, "Clown_ChildSkin");
			block.SetPosition(glm::vec3(0.0f, 0.5, 0.0f));
			first->InsertBlock(block);
			//second->InsertBlock(block);
			//second->SetName("Second");
		}

		initialized = true;
	}

	first->CalculateBoundingBox();
	//second->CalculateBoundingBox();
	std::vector<Blocks::Block>* firstBlocks = first->GetBlocks();

	//if (Bounds::AreColliding(firstBlocks->at(0).GetBoundingBox(), secondBlocks->at(0).GetBoundingBox(), collisionNormal))
	if (AreColliding(firstBlocks->at(0), firstBlocks->at(1)))
	{
		Crosshairs::Get()->SetColor(Colors::Red);

	}
	else
	{
		Crosshairs::Get()->SetColor(Colors::Green);
	}
}