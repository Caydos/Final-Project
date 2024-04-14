#include "Scene.h"

static bool initialized = false;
static std::vector<Lighting::Spot*> spotLights;
static unsigned int spotVAO;
static unsigned int spotVertexVBO;
static unsigned int spotInstanceVBO;
static Shaders::Shader* spotShader;


static GLuint gBuffer;
static GLuint gAlbedoSpec;
static GLuint rboDepth;

#define SPOT_MAX_COUNT 1
static glm::mat4 tstMdl;

void Scene::Lights::Initialize(GameData* _gameData)
{
	spotShader = new Shaders::Shader("../Shaders/Lighting/Spot.vs", "../Shaders/Lighting/Spot.fs");
	spotShader->setInt("gAlbedoSpec", 0);

	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gAlbedoSpec, 0);
	unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, attachments);


	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _gameData->resolution[0], _gameData->resolution[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


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

	tstMdl = glm::mat4(1.0f);
	//tstMdl = glm::translate(tstMdl, glm::vec3(1.0, 1.0, 1.0));
	//tstMdl = glm::scale(tstMdl, glm::vec3(2.0, 2.0, 2.0));

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SPOT_MAX_COUNT, &tstMdl, GL_DYNAMIC_DRAW); // Allocate new size


	//// Set up instance attributes
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, position));
	//glEnableVertexAttribArray(1);
	//glVertexAttribDivisor(1, 1); // This attribute only advances once per instance


	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, direction));
	//glEnableVertexAttribArray(2);
	//glVertexAttribDivisor(2, 1);

	//glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, cutOff));
	//glEnableVertexAttribArray(3);
	//glVertexAttribDivisor(3, 1);

	//glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, outerCutOff));
	//glEnableVertexAttribArray(4);
	//glVertexAttribDivisor(4, 1);

	//glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, ambient));
	//glEnableVertexAttribArray(5);
	//glVertexAttribDivisor(5, 1);

	//glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, diffuse));
	//glEnableVertexAttribArray(6);
	//glVertexAttribDivisor(6, 1);

	//glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, specular));
	//glEnableVertexAttribArray(7);
	//glVertexAttribDivisor(7, 1);

	//glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, constant));
	//glEnableVertexAttribArray(8);
	//glVertexAttribDivisor(8, 1);

	//glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, linear));
	//glEnableVertexAttribArray(9);
	//glVertexAttribDivisor(9, 1);

	//glVertexAttribPointer(10, 3, GL_FLOAT, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, quadratic));
	//glEnableVertexAttribArray(10);
	//glVertexAttribDivisor(10, 1);

	//glVertexAttribPointer(11, 1, GL_BOOL, GL_FALSE, sizeof(Lighting::Spot), (void*)offsetof(Lighting::Spot, active));
	//glEnableVertexAttribArray(11);
	//glVertexAttribDivisor(11, 1);

	for (unsigned int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 1);
		glVertexAttribPointer(i + 1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 1, 1);
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
			//glBindVertexArray(spotVAO);
			glBindBuffer(GL_ARRAY_BUFFER, spotInstanceVBO);
			//glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * SPOT_MAX_COUNT, &_spot->modelMatrix, GL_DYNAMIC_DRAW); // Allocate new size
			glBufferSubData(GL_ARRAY_BUFFER, spotId * sizeof(glm::mat4), sizeof(glm::mat4), &_spot->modelMatrix);


			break;
		}
	}
}


unsigned int Scene::Lights::DrawSpots(GameData* _gameData, unsigned int _gPosition, unsigned int _gNormal, unsigned int _gAlbedoSpec, unsigned int _gEffects)
{
	if (!initialized) { Initialize(_gameData); }

	spotShader->use();
	spotShader->setMat4("projection", Scene::World::GetProjection());
	spotShader->setMat4("view", Scene::World::GetView());
	spotShader->setVec3("viewPos", _gameData->camera->Position);
	spotShader->setVec2("screenSize", glm::vec2(_gameData->resolution[0], _gameData->resolution[1]));

	Colors::Color clearColor = Scene::GetClearColor();
	spotShader->setVec4("clearColor", clearColor.values[0], clearColor.values[1], clearColor.values[2], clearColor.values[3]);

	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glViewport(0, 0, _gameData->resolution[0], _gameData->resolution[1]);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // Standard depth function

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _gAlbedoSpec);
	


	glBindVertexArray(spotVAO);
	glEnable(GL_CULL_FACE);
	// Double the draw call to ensure only one draw depending if I'm inside or outside the light cone
	glCullFace(GL_BACK);

	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, SPOT_MAX_COUNT);
	glCullFace(GL_FRONT);
	glDrawArraysInstanced(GL_TRIANGLES, 0, 36, SPOT_MAX_COUNT);

	glBindVertexArray(0);
	glDisable(GL_CULL_FACE);
	return gAlbedoSpec;
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
