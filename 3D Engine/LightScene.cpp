#include "Scene.h"

static bool initialized = false;
static std::vector<Lighting::Spot*> spotLights;
static unsigned int spotVAO;
static unsigned int spotVertexVBO;
static unsigned int spotInstanceVBO;
static Shaders::Shader* spotShader;

#define SPOT_MAX_COUNT 1

void Scene::Lights::Initialize(GameData* _gameData)
{
	spotShader = new Shaders::Shader("../Shaders/Lighting/Spot.vs", "../Shaders/Lighting/Spot.fs");
	spotShader->use();
	spotShader->setInt("inPosition", 0);
	spotShader->setInt("inNormal", 1);
	spotShader->setInt("inAlbedoSpec", 2);
	spotShader->setInt("inEffects", 3);
	spotShader->setInt("inLighting", 4);

	glGenVertexArrays(1, &spotVAO);
	glGenBuffers(1, &spotVertexVBO);
	glGenBuffers(1, &spotInstanceVBO); // Corrected to spotInstanceVBO

	glBindVertexArray(spotVAO);

	// Assuming an equilateral triangle with side lengths of 1 unit
	float vertices[] = {
		//front face
		-0.5f, -0.5f, 0.5f,		
		0.5f, -0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.5f,		
		-0.5f, -0.5f, 0.5f,		

		// right face
		0.5f, -0.5f, 0.5f,
		0.5f, -0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, -0.5f, 0.5f,

		//back face
		0.5f, -0.5f, -0.5f,	
		-0.5f, -0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,	
		-0.5f, 0.5f, -0.5f,	
		0.5f, 0.5f, -0.5f,	
		0.5f, -0.5f, -0.5f,	

		// left face
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f, 0.5f,	
		-0.5f, 0.5f, 0.5f,	
		-0.5f, 0.5f, 0.5f,	
		-0.5f, 0.5f, -0.5f,	
		-0.5f, -0.5f, -0.5f,

		// top face
		-0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, -0.5f,
		0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, -0.5f,
		-0.5f, 0.5f, 0.5f,

		//bottom
		-0.5f, -0.5f, -0.5f,
		0.5f, -0.5f, -0.5f,	
		0.5f, -0.5f, 0.5f,	
		0.5f, -0.5f, 0.5f,	
		-0.5f, -0.5f, 0.5f,	
		-0.5f, -0.5f, -0.5f,
	};

	glBindBuffer(GL_ARRAY_BUFFER, spotVertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Vertex positions (for the sphere geometry)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Bind instance VBO for light properties setup
	glBindBuffer(GL_ARRAY_BUFFER, spotInstanceVBO);
	// You will need to buffer your instance data similar to vertices
	// Assuming instanceData is a struct or array you've prepared
	// glBufferData(GL_ARRAY_BUFFER, sizeof(instanceData), instanceData, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(Lighting::Spot) * SPOT_MAX_COUNT, NULL, GL_DYNAMIC_DRAW); // Allocate new size


	glBufferData(GL_ARRAY_BUFFER, sizeof(Lighting::Spot) * SPOT_MAX_COUNT, NULL, GL_DYNAMIC_DRAW); // Allocate new size


	// position
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*) offsetof(Lighting::Spot, position));
	glVertexAttribDivisor(1, 1);

	// direction
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, direction));
	glVertexAttribDivisor(2, 1);

	// cutOff
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, cutOff));
	glVertexAttribDivisor(3, 1);


	// outerCutOff
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, outerCutOff));
	glVertexAttribDivisor(4, 1);

	// ambient
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, ambient));
	glVertexAttribDivisor(5, 1);

	// diffuse
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, diffuse));
	glVertexAttribDivisor(6, 1);

	// specular
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, specular));
	glVertexAttribDivisor(7, 1);

	// constant
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(8, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, constant));
	glVertexAttribDivisor(8, 1);

	// linear
	glEnableVertexAttribArray(9);
	glVertexAttribPointer(9, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, linear));
	glVertexAttribDivisor(9, 1);

	// quadratic
	glEnableVertexAttribArray(10);
	glVertexAttribPointer(10, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, quadratic));
	glVertexAttribDivisor(10, 1);

	// activation (bool to int conversion may be needed)
	glEnableVertexAttribArray(11);
	glVertexAttribPointer(11, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, activation));
	glVertexAttribDivisor(11, 1);

	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 12);
		glVertexAttribPointer(i + 12, 4, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)(offsetof(Lighting::Spot, modelMatrix) + sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 12, 1);
	}



	glBindVertexArray(0);
	initialized = true;
}

Lighting::Spot* Scene::Lights::CreateSpot()
{
	Lighting::Spot* spot = new Lighting::Spot;
	spotLights.push_back(spot);
	return spot;
}

void Scene::Lights::EraseSpot(Lighting::Spot* _spot)
{
	for (size_t spotId = 0; spotId < spotLights.size(); spotId++)
	{
		if (spotLights[spotId] == _spot)
		{
			spotLights[spotId]->activation = false;
			Scene::Lights::UpdateSpot(_spot);
			delete spotLights[spotId];
			spotLights.erase(spotLights.begin() + spotId);
			break;
		}
	}
}

void Scene::Lights::UpdateSpot(Lighting::Spot* _spot)
{
	for (size_t spotId = 0; spotId < spotLights.size(); spotId++)
	{
		if (spotLights[spotId] == _spot)
		{
			glBindVertexArray(spotVAO);
			glBindBuffer(GL_ARRAY_BUFFER, spotInstanceVBO);
			glBufferSubData(GL_ARRAY_BUFFER, spotId * sizeof(Lighting::Spot), sizeof(Lighting::Spot), _spot);
			break;
		}
	}
}


void Scene::Lights::DrawSpots(GameData* _gameData)
{
	if (!initialized) { Initialize(_gameData); }

	spotShader->use();
	spotShader->setMat4("projection", Scene::World::GetProjection());
	spotShader->setMat4("view", Scene::World::GetView());
	spotShader->setVec3("viewPos", _gameData->camera->Position);
	spotShader->setVec2("screenSize", glm::vec2(_gameData->resolution[0], _gameData->resolution[1]));

	Colors::Color clearColor = Scene::GetClearColor();
	spotShader->setVec4("clearColor", clearColor.values[0], clearColor.values[1], clearColor.values[2], clearColor.values[3]);
	glDepthMask(GL_FALSE);  // Disable depth writing

	glBindVertexArray(spotVAO);
	glEnable(GL_CULL_FACE);

	// Double the draw call to ensure only one draw depending if I'm inside or outside the light cone
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, SPOT_MAX_COUNT);

	glCullFace(GL_FRONT);
	glDisable(GL_DEPTH_TEST);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, SPOT_MAX_COUNT);

	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	glDepthMask(GL_TRUE);  // Disable depth writing
	glEnable(GL_DEPTH_TEST);
}

std::vector<Lighting::Spot*>* Scene::Lights::GetSpots()
{
	return &spotLights;
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
