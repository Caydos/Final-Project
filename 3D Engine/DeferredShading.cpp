#include "DeferredShading.h"
#include "Set.h"
#include "SSAO.h"
#include "Scene.h"
#include "Skybox.h"

static bool initialized = false;
static unsigned int quadVAO = 0;
static unsigned int quadVBO;
static GLuint gBuffer;
static GLuint gPosition, gNormal, gAlbedoSpec, gEffects, gLightMap;
static GLuint rboDepth;

void DeferredShading::Initialize(GameData* _gameData)
{
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	glGenTextures(1, &gPosition);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	glGenTextures(1, &gAlbedoSpec);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);

	glGenTextures(1, &gEffects);
	glBindTexture(GL_TEXTURE_2D, gEffects);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, gEffects, 0);

	glGenTextures(1, &gLightMap);
	glBindTexture(GL_TEXTURE_2D, gLightMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, _gameData->resolution[0], _gameData->resolution[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, gLightMap, 0);

	unsigned int attachments[5] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4 };
	glDrawBuffers(5, attachments);


	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, _gameData->resolution[0], _gameData->resolution[1]);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);


	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer is not complete!" << std::endl;
	}
	_gameData->shaders[Shaders::GEOMETRY]->use();
	_gameData->shaders[Shaders::GEOMETRY]->setInt("textureX", 0);
	_gameData->shaders[Shaders::GEOMETRY]->setInt("effectsTexture", 1);

	_gameData->shaders[Shaders::RENDER]->use();
	_gameData->shaders[Shaders::RENDER]->setInt("gPosition", 0);
	_gameData->shaders[Shaders::RENDER]->setInt("gNormal", 1);
	_gameData->shaders[Shaders::RENDER]->setInt("gAlbedoSpec", 2);
	_gameData->shaders[Shaders::RENDER]->setInt("gEffects", 3);
	_gameData->shaders[Shaders::RENDER]->setInt("gLighting", 4);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//Skybox::Load(_gameData);

	initialized = true;
}

void DeferredShading::RenderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void DeferredShading::Draw(GameData* _gameData, bool _skyboxUsage)
{
	if (!initialized) { Initialize(_gameData); }
	Colors::Color clearColor = Scene::GetClearColor();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glViewport(0, 0, _gameData->resolution[0], _gameData->resolution[1]);
	glClearColor(clearColor.values[0], clearColor.values[1], clearColor.values[2], clearColor.values[3]);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (_skyboxUsage)
	{
		Skybox::Draw(_gameData);
	}

	glActiveTexture(GL_TEXTURE0);
	_gameData->shaders[Shaders::GEOMETRY]->use();
	Sets::UpdateVisibility();
	Blocks::Draw();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosition);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gEffects);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gLightMap);

	{//Lighting
		Scene::Lights::DrawSpots(_gameData);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, _gameData->resolution[0], _gameData->resolution[1]);
	glClearColor(clearColor.values[0], clearColor.values[1], clearColor.values[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Camera* cam = Scene::World::GetCamera();
	_gameData->shaders[Shaders::RENDER]->use();
	_gameData->shaders[Shaders::RENDER]->setVec3("viewPos", cam->Position);
	_gameData->shaders[Shaders::RENDER]->setVec4("clearColor", clearColor.values[0], clearColor.values[1], clearColor.values[2], clearColor.values[3]);


	RenderQuad();

	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
}
