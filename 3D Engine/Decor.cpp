#include "Decor.h"
#include "Files.h"
#include "RayCasting.h"
static Cube ghostCube;

Decors::Decor::Decor(std::string _name)
{
	this->SetName(_name);
	this->SetScale(DECOR_SCALE);
}

Decors::Decor::~Decor()
{
}


void Decors::Decor::InsertObject(Cube _object)
{
	float scale = this->GetScale();
	_object.SetScale(scale, scale, scale);
	_object.SetId(this->objects.size());
	_object.SetAttachment(this->GetMatrix());
	this->objects.push_back(_object);
}

void Decors::Decor::RemoveObject(Cube* _cube)
{
	for (size_t i = 0; i < this->objects.size(); i++)
	{
		if (&this->objects[i] == _cube)
		{
			if (!this->objects[i].GetId()) { return; }
			this->objects.erase(this->objects.begin() + i);
		}
	}
}


void Decors::Decor::Draw()
{
	if (!this->IsVisible())
	{
		return;
	}
	if (this->AreBoundsVisible())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	if (this->IsUpdated())
	{
		for (size_t i = 0; i < this->objects.size(); i++)
		{
			this->objects[i].Update();
		}
		this->CalculateBoundingBox();
		this->LoadInstances();
	}
	if (this->IsInstanced())
	{
		for (size_t i = 0; i < instances.size(); i++)
		{
			instances[i].Draw();
		}
	}
	else
	{
		for (size_t i = 0; i < this->objects.size(); i++)
		{
			this->objects[i].Draw();
		}
	}
	if (this->AreBoundsVisible())
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

Cube* Decors::Decor::Raycast()
{
	if (this->IsInstanced()) { return nullptr; }
	if (!ghostCube.IsInitialized())
	{
		GameData* gameData = GetGameData();
		ghostCube.Initialize();
		ghostCube.BindShader(gameData->shaders[Shaders::WORLD_OBJECT]);
		float scale = this->GetScale();
		ghostCube.SetScale(scale, scale, scale);
		ghostCube.SetColor(Colors::White);
		ghostCube.SetLightDependency(false);
		ghostCube.SetOpacity(0.5f);
	}
	this->SetRaycastHit(nullptr);

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

	std::sort(objects.begin(), objects.end(), [position](Drawable& a, Drawable& b) {
		float distA = glm::length(a.GetPosition() - position);
		float distB = glm::length(b.GetPosition() - position);
		return distA < distB;
		});

	RayCasting::Face hittedFace;
	float closest = 100.0f;
	for (size_t i = 0; i < objects.size(); i++)
	{
		auto it = &objects.at(i);
		glm::vec3 boxPosition = it->GetPosition();
		float halfSize = this->GetScale() / 2.0f; // Since scale is 1.0, half-size on each axis is 0.5

		Bounds::Box boxAABB;
		boxAABB.min = boxPosition - glm::vec3(halfSize, halfSize, halfSize);
		boxAABB.max = boxPosition + glm::vec3(halfSize, halfSize, halfSize);
		float rslt = RayCasting::Intersect(ray, boxAABB);
		if (rslt > .0f && rslt < closest)
		{
			RayCasting::Cube cube = { it->GetPosition(), this->GetScale()};
			hittedFace = IntersectedFace(ray, cube);
			closest = rslt;
			glm::vec3 position = it->GetPosition();
			switch (hittedFace)
			{
			case RayCasting::FRONT: {position.z += this->GetScale(); break; }
			case RayCasting::BACK: {position.z -= this->GetScale(); break; }
			case RayCasting::LEFT: {position.x -= this->GetScale(); break; }
			case RayCasting::RIGHT: {position.x += this->GetScale(); break; }
			case RayCasting::TOP: {position.y += this->GetScale(); break; }
			case RayCasting::BOTTOM: {position.y -= this->GetScale(); break; }
			default: break;
			}
			ghostCube.SetPosition(position.x, position.y, position.z);
			this->SetRaycastHit(it);
			continue;
		}
	}
	if (this->GetRaycastHit() != nullptr)
	{
		ghostCube.GetShader()->use();
		ghostCube.Draw();
		return &ghostCube;
	}
	return nullptr;
}

bool Decors::Decor::LoadFromFile(const char* _name)
{
	this->SetName(_name);
	std::string path = DECOR_DIRECTORY;
	path += this->GetName();
	path += DECOR_FILE_EXTENSION;

	//Make sure the previous scene is cleared out
	this->objects.clear();

	std::vector<Material>* materials = Scene::GetMaterials();
	GameData* gameData = GetGameData();

	//Load new scene
	std::string sceneFileContent = Files::GetFileContent(path.c_str());
	if (sceneFileContent.size() < 3) { return false; }

	json jsonParsed = json::parse(sceneFileContent);
	for (auto& object : jsonParsed["objects"])
	{
		std::string typeStr = object["type"];
		if (strcmp(typeStr.c_str(), "Cube") == 0)
		{
			Cube obj;
			obj.Initialize();

			if (object.contains("position") && !object["position"].is_null())
			{
				obj.SetPosition(object["position"][0], object["position"][1], object["position"][2]);
			}
			if (object.contains("rotation") && !object["rotation"].is_null())
			{
				obj.SetRotation(object["rotation"][0], object["rotation"][1], object["rotation"][2]);
			}

			obj.SetScale(DECOR_SCALE, DECOR_SCALE, DECOR_SCALE);

			if (object.contains("material") && !object["material"].is_null())
			{
				std::string materialStr = object["material"].get<std::string>();
				if (materialStr.size() > 3)
				{
					for (size_t i = 0; i < materials->size(); i++)
					{
						if (strcmp(materialStr.c_str(), materials->at(i).GetName().c_str()) == 0)
						{
							obj.SetMaterial(&materials->at(i), true);
							break;
						}
					}
				}
			}
			if (object.contains("colors") && !object["colors"].is_null())
			{
				obj.SetColor(Colors::Color(object["colors"][0], object["colors"][1], object["colors"][2], object["colors"][3]));
			}

			if (object.contains("light") && !object["light"].is_null())
			{
				Lightning::Light lightT;
				lightT.LoadFromJson(object["light"]);
				GameData* gameData = GetGameData();
				Lightning::Light* light = Scene::Lights::InsertLight(gameData, lightT);
				obj.SetLight(light);
			}
			obj.BindShader(gameData->shaders[Shaders::WORLD_OBJECT]);
			this->objects.push_back(obj);
		}
	}
	this->CalculateBoundingBox();
	return true;
}

void Decors::Decor::Save()
{
	json jsonObject;
	jsonObject["objects"] = json::array();

	for (auto object : this->objects)
	{
		jsonObject["objects"].push_back(object.Encode());
	}

	Files::Create(DECOR_DIRECTORY, this->GetName().c_str(), DECOR_FILE_EXTENSION, jsonObject.dump().c_str());
}

void Decors::Decor::LoadInstances()
{
	this->instances = Instances::Generate(&this->objects);
}


void Decors::Decor::CalculateBoundingBox()
{
	glm::vec3 globalMin = glm::vec3(FLT_MAX);
	glm::vec3 globalMax = glm::vec3(-FLT_MAX);

	for (Cube& cube : objects)
	{
		glm::vec3 localMin = cube.GetPosition() - cube.GetScale() / 2.0f;
		glm::vec3 localMax = cube.GetPosition() + cube.GetScale() / 2.0f;

		globalMin = glm::min(globalMin, localMin);
		globalMax = glm::max(globalMax, localMax);
	}

	this->SetBoundingBox({ globalMin, globalMax });
}
