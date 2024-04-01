#include "Scene.h"


static std::vector<Lighting::Light*> lights;
struct ShaderLight
{
	Lighting::Light* light = nullptr;
	bool state = false;
};
static ShaderLight shaderLights[MAX_LIGHTS];
static std::vector<int> refreshId;

void InsertInShader(Lighting::Light* _light)
{
	for (size_t inShaderId = 0; inShaderId < MAX_LIGHTS; inShaderId++)
	{
		if (!shaderLights[inShaderId].state)
		{//Free cell
			shaderLights[inShaderId].light = _light;
			shaderLights[inShaderId].state = true;
			//std::cout << "Inserting at id : " << inShaderId << std::endl;
			refreshId.push_back(inShaderId);
			break;
		}
	}
}

void RemoveFromShader(Lighting::Light* _light)
{
	for (size_t inShaderId = 0; inShaderId < MAX_LIGHTS; inShaderId++)
	{
		if (shaderLights[inShaderId].light == _light)
		{
			shaderLights[inShaderId].state = false;
			shaderLights[inShaderId].light = nullptr;
			//std::cout << "Removing at id : " << inShaderId << std::endl;
			refreshId.push_back(inShaderId);
			break;
		}
	}
}

Lighting::Light* Scene::Lights::Create()
{
	Lighting::Light* light = new Lighting::Light;
	lights.push_back(light);
	return light;
}

void Scene::Lights::Erase(Lighting::Light* _light)
{
	for (size_t lightId = 0; lightId < lights.size(); lightId++)
	{
		if (lights[lightId] == _light)
		{
			if (_light->IsVisible())
			{
				//Remove from shader
				RemoveFromShader(_light);
			}
			delete _light;
			lights.erase(lights.begin() + lightId);
			break;
		}
	}
}

void Scene::Lights::UpdateVisibility()
{
	for (size_t lightId = 0; lightId < lights.size(); lightId++)
	{
		Lighting::Light* currentLight = lights[lightId];
		bool previousVisibility = currentLight->IsVisible();
		currentLight->CheckVisibility();
		if (currentLight->IsVisible() && !previousVisibility)
		{
			//Insert in shader
			InsertInShader(currentLight);
		}
		else if (!currentLight->IsVisible() && previousVisibility)
		{
			//Remove from shader
			RemoveFromShader(currentLight);
		}
	}

	for (size_t id = 0; id < refreshId.size(); id++)
	{
		std::string str = "lights[" + std::to_string(refreshId[id]) + ']';
		GameData* gameData = GetGameData();
		gameData->shaders[Shaders::RENDER]->use();
		Lighting::Light* _light = shaderLights[refreshId[id]].light;
		if (_light != nullptr && shaderLights[refreshId[id]].state)
		{
			switch (_light->GetType())
			{
			case Lighting::DIRECTIONAL:
			{
				gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::DIRECTIONAL);
				gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light->IsActive());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light->GetDirection());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light->GetAmbient());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light->GetDiffuse());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light->GetSpecular());
				break;
			}
			case Lighting::POINT:
			{
				gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::POINT);
				gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light->IsActive());

				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light->GetPosition());

				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light->GetConstant());
				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light->GetLinear());
				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light->GetQuadratic());

				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light->GetAmbient());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light->GetDiffuse());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light->GetSpecular());
				break;
			}
			case Lighting::SPOT:
			{
				//std::cout << "Updating " << str << std::endl;
				gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::SPOT);
				gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light->IsActive());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light->GetDirection());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light->GetPosition());

				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".cutOff"), glm::cos(glm::radians(_light->GetCutOff())));
				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".outerCutOff"), glm::cos(glm::radians(_light->GetOuterCutOff())));

				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light->GetConstant());
				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light->GetLinear());
				gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light->GetQuadratic());

				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light->GetAmbient());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light->GetDiffuse());
				gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light->GetSpecular());
				//std::cout << _light->GetConstant() << " " << _light->GetLinear() << " " << _light->GetQuadratic() << std::endl << std::endl;
				break;
			}
			default:
			{
				break;
			}
			}
		}
		else
		{
			gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), false);
		}
	}
	refreshId.clear();
}

void Scene::Lights::RefreshLight(Lighting::Light* _light)
{
	for (size_t lightId = 0; lightId < MAX_LIGHTS; lightId++)
	{
		Lighting::Light* currentLight = shaderLights[lightId].light;
		if (currentLight == _light)
		{
			refreshId.push_back(lightId);
			break;
		}
	}

}































//Lighting::Light* Scene::Lights::InsertLight(GameData* _gameData, Lighting::Light& _light)
//{
//	if (lights.size() + 1 >= MAX_LIGHTS)
//	{
//		std::cout << "Unable to add more lights to the shader, the limit has been reached." << std::endl;
//		return nullptr;
//	}
//
//	std::string str = "lights[" + std::to_string(lights.size()) + ']';
//	_gameData->shaders[Shaders::RENDER]->use();
//	switch (_light.GetType())
//	{
//	case Lighting::DIRECTIONAL:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::DIRECTIONAL);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light.GetDirection());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
//		break;
//	}
//	case Lighting::POINT:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::POINT);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());
//
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light.GetPosition());
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light.GetConstant());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light.GetLinear());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light.GetQuadratic());
//
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
//		break;
//	}
//	case Lighting::SPOT:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::SPOT);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), _light.IsActive());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), _light.GetDirection());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), _light.GetPosition());
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".cutOff"), glm::cos(glm::radians(_light.GetCutOff())));
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".outerCutOff"), glm::cos(glm::radians(_light.GetOuterCutOff())));
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), _light.GetConstant());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), _light.GetLinear());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), _light.GetQuadratic());
//
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), _light.GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), _light.GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), _light.GetSpecular());
//		break;
//	}
//	default:
//	{
//		break;
//	}
//	}
//	lights.push_back(_light);
//	return &lights[lights.size() - 1];
//}
//
//void Scene::Lights::UpdateLight(GameData* _gameData, unsigned int _id)
//{
//	std::string str = "lights[" + std::to_string(_id) + ']';
//	Lighting::Light* light = &lights.at(_id);
//	 
//	//_gameData->shaders[Shaders::RENDER]->use();
//	switch (light->GetType())
//	{
//	case Lighting::DIRECTIONAL:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::DIRECTIONAL);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), light->GetDirection());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
//		break;
//	}
//	case Lighting::POINT:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::POINT);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), light->GetPosition());
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), light->GetConstant());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), light->GetLinear());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), light->GetQuadratic());
//
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
//		break;
//	}
//	case Lighting::SPOT:
//	{
//		_gameData->shaders[Shaders::RENDER]->setInt(std::string(str + ".type"), Lighting::SPOT);
//		_gameData->shaders[Shaders::RENDER]->setBool(std::string(str + ".activated"), light->IsActive());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".direction"), light->GetDirection());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".position"), light->GetPosition());
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".cutOff"), glm::cos(glm::radians(light->GetCutOff())));
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".outerCutOff"), glm::cos(glm::radians(light->GetOuterCutOff())));
//
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".constant"), light->GetConstant());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".linear"), light->GetLinear());
//		_gameData->shaders[Shaders::RENDER]->setFloat(std::string(str + ".quadratic"), light->GetQuadratic());
//
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".ambient"), light->GetAmbient());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".diffuse"), light->GetDiffuse());
//		_gameData->shaders[Shaders::RENDER]->setVec3(std::string(str + ".specular"), light->GetSpecular());
//		break;
//	}
//	default:
//	{
//		break;
//	}
//	}
//}
//
//void Scene::Lights::UpdateShader(GameData* _gameData)
//{
//	_gameData->shaders[Shaders::RENDER]->use();
//	_gameData->shaders[Shaders::RENDER]->setBool("considerLightning", Scene::World::IsConsideringLightning());
//	_gameData->shaders[Shaders::RENDER]->setInt("lightCount", lights.size());
//
//	for (size_t i = 0; i < lights.size(); i++)
//	{
//		UpdateLight(_gameData, i);
//	}
//}
//
//std::vector<Lighting::Light>* Scene::Lights::GetLights()
//{
//	return &lights;
//}
//
//void Scene::Lights::Remove(Lighting::Light* _lightAddr)
//{
//	for (auto it = lights.begin(); it != lights.end(); ++it)
//	{
//		if (_lightAddr == &(*it))
//		{
//			lights.erase(it);
//			return;
//		}
//	}
//}
