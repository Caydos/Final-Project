#include "Set.h"
#include "RayCasting.h"
#include "Scene.h"

static std::vector<Sets::Set*> sets;

void Sets::UpdateVisibility()
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		sets[i]->CheckVisibility();
	}
}

Sets::Set* Sets::Create()
{
	Set* newSet = new Set;
	newSet->Initialize();
	sets.push_back(newSet);
	return newSet;
}

void Sets::Erase(Set* _set)
{
	for (size_t i = 0; i < sets.size(); i++)
	{
		if (sets[i] == _set)
		{
			sets[i]->Erase();
			delete sets[i];
			sets.erase(sets.begin() + i);
			return;
		}
	}
}

std::vector<Sets::Set*>* Sets::GetAll()
{
	return &sets;
}


void Sets::Edition()
{
	std::vector<Blocks::Block> blockList;
	for (size_t setId = 0; setId < sets.size(); setId++)
	{
		if (sets[setId]->IsVisible())
		{
			std::vector<Blocks::Block> setBlocks = sets[setId]->GetBlocks();
			for (size_t i = 0; i < setBlocks.size(); i++)
			{
				blockList.push_back(setBlocks[i]);
			}
		}
	}
	// Assuming your crosshair is at the center of the screen
	glm::vec4 ray_clip = glm::vec4(0.0, 0.0, -1.0, 1.0);

	// Convert to Eye Space
	glm::mat4 inv_projection = glm::inverse(Scene::World::GetProjection());
	glm::vec4 ray_eye = inv_projection * ray_clip;
	ray_eye = glm::vec4(ray_eye.x, ray_eye.y, -1.0, 0.0); // Homogenize

	// Convert to World Space
	glm::mat4 inv_view = glm::inverse(Scene::World::GetView());
	glm::vec4 ray_wor = inv_view * ray_eye;
	// Don't forget to normalize the ray direction
	glm::vec3 ray_world_direction = glm::normalize(glm::vec3(ray_wor));

	RayCasting::Ray ray;
	glm::vec3 position = Scene::World::GetCamera()->Position;
	ray.origin = position;
	ray.direction = ray_world_direction;

	RayCasting::Face hittedFace;
	float closest = 100.0f;
	Blocks::Block* hittedOne = nullptr;
	for (size_t i = 0; i < blockList.size(); i++)
	{
		auto it = &blockList.at(i);
		glm::vec3 boxPosition = it->GetPosition();
		float halfSize = it->GetScale().x / 2.0f; // Since scale is 1.0, half-size on each axis is 0.5

		Bounds::Box boxAABB;
		boxAABB.min = boxPosition - glm::vec3(halfSize, halfSize, halfSize);
		boxAABB.max = boxPosition + glm::vec3(halfSize, halfSize, halfSize);
		float rslt = RayCasting::Intersect(ray, boxAABB);
		if (rslt > .0f && rslt < closest)
		{
			hittedOne = it;
			RayCasting::Cube cube = { it->GetPosition(), it->GetScale().x };
			hittedFace = IntersectedFace(ray, cube);
			closest = rslt;
			glm::vec3 position = it->GetPosition();
			switch (hittedFace)
			{
			case RayCasting::FRONT: {position.z += it->GetScale().z; break; }
			case RayCasting::BACK: {position.z -= it->GetScale().z; break; }
			case RayCasting::LEFT: {position.x -= it->GetScale().x; break; }
			case RayCasting::RIGHT: {position.x += it->GetScale().x; break; }
			case RayCasting::TOP: {position.y += it->GetScale().y; break; }
			case RayCasting::BOTTOM: {position.y -= it->GetScale().y; break; }
			default: break;
			}

			//ghostCube.SetPosition(position.x, position.y, position.z);
		}
	}
	if (hittedOne != nullptr)
	{
		//Block placement
	}
}