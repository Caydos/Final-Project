#include "Scene.h"


static std::vector<Lightning::Light> lights;


Lightning::Light* Scene::Lights::InsertLight(GameData* _gameData, Lightning::Light& _light)
{
	if (lights.size() + 1 >= MAX_LIGHTS)
	{
		std::cout << "Unable to add more lights to the shader, the limit has been reached." << std::endl;
		return nullptr;
	}

	std::string str = "lights[" + std::to_string(lights.size()) + ']';
	_gameData->shaders[Shaders::RENDER]->use();
	switch (_light.GetType())
	{
	case Lightning::DIRECTIONAL:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::DIRECTIONAL);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light.GetDirection());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
		break;
	}
	case Lightning::POINT:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::POINT);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());

		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light.GetPosition());

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light.GetConstant());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light.GetLinear());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light.GetQuadratic());

		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
		break;
	}
	case Lightning::SPOT:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::SPOT);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light.GetDirection());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light.GetPosition());

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".cutOff"), glm::cos(glm::radians(_light.GetCutOff())));
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".outerCutOff"), glm::cos(glm::radians(_light.GetOuterCutOff())));

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light.GetConstant());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light.GetLinear());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light.GetQuadratic());

		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
		break;
	}
	default:
	{
		break;
	}
	}
	lights.push_back(_light);
	return &lights[lights.size() - 1];
}

void Scene::Lights::UpdateLight(GameData* _gameData, unsigned int _id)
{
	std::string str = "lights[" + std::to_string(_id) + ']';
	Lightning::Light* light = &lights.at(_id);
	 
	//_gameData->shaders[Shaders::RENDER]->use();
	switch (light->GetType())
	{
	case Lightning::DIRECTIONAL:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::DIRECTIONAL);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), light->GetDirection());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
		break;
	}
	case Lightning::POINT:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::POINT);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), light->GetPosition());

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), light->GetConstant());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), light->GetLinear());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), light->GetQuadratic());

		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
		break;
	}
	case Lightning::SPOT:
	{
		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lightning::SPOT);
		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), light->GetDirection());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), light->GetPosition());

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".cutOff"), glm::cos(glm::radians(light->GetCutOff())));
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".outerCutOff"), glm::cos(glm::radians(light->GetOuterCutOff())));

		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), light->GetConstant());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), light->GetLinear());
		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), light->GetQuadratic());

		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
		break;
	}
	default:
	{
		break;
	}
	}
}

void Scene::Lights::UpdateShader(GameData* _gameData)
{
	_gameData->shaders[Shaders::RENDER]->use();
	_gameData->shaders[Shaders::RENDER]->setBool("considerLightning", Scene::World::IsConsideringLightning());
	_gameData->shaders[Shaders::RENDER]->setInt("lightCount", lights.size());

	for (size_t i = 0; i < lights.size(); i++)
	{
		UpdateLight(_gameData, i);
	}
}

std::vector<Lightning::Light>* Scene::Lights::GetLights()
{
	return &lights;
}

void Scene::Lights::Remove(Lightning::Light* _lightAddr)
{
	for (auto it = lights.begin(); it != lights.end(); ++it)
	{
		if (_lightAddr == &(*it))
		{
			lights.erase(it);
			return;
		}
	}
}
