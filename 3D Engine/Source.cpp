#include "Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "Scene.h"
#include "Commands.h"
#include "Network.h"
#include "Audio.h"
#include "Scripting.h"
#include "Tools.h"

float lastFrame = 0.0f;

GameData gameData;

GameData* GetGameData() { return &gameData; }

int main()
{
	bool running = true;
	Network::Events::CreateEvents();
	std::thread eventThread(&Network::Events::Thread, running);
	eventThread.detach();
	std::thread commandsThread(&Commands::Thread, running);
	commandsThread.detach();

	Camera cam = Camera(glm::vec3(0.0f, 0.0f, 1.0f));
	gameData.camera = &cam;
	gameData.resolution[0] = SCREEN_WIDTH;
	gameData.resolution[1] = SCREEN_HEIGHT;
	Settings::Load(&gameData);

	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	gameData.window.Create("3D Rendering Engine", gameData.resolution[0], gameData.resolution[1]);

	Shaders::Shader uiShader("../Shaders/UI.vs", "../Shaders/UI.fs");
	Shaders::Shader uiObject("../Shaders/UIObject.vs", "../Shaders/UIObject.fs");
	Shaders::Shader worldObjectShader("../Shaders/WorldObject.vs", "../Shaders/WorldObject.fs");
	Shaders::Shader geometryShader("../Shaders/Geometry.vs", "../Shaders/Geometry.fs");
	Shaders::Shader renderShader("../Shaders/Render.vs", "../Shaders/Render.fs");
	Shaders::Shader skyboxShader("../Shaders/Skybox.vs", "../Shaders/Skybox.fs");


	gameData.shaders[Shaders::UI] = &uiShader;
	gameData.shaders[Shaders::UI_OBJECT] = &uiObject;
	gameData.shaders[Shaders::SINGLE_DRAW] = &worldObjectShader;
	gameData.shaders[Shaders::GEOMETRY] = &geometryShader;
	gameData.shaders[Shaders::RENDER] = &renderShader;
	gameData.shaders[Shaders::SKYBOX] = &skyboxShader;


	gameData.gameStates[TOOLS] = &Tools::Tick;
	gameData.gameStates[GAME] = &Scritping::Tick;
	gameData.gameState = TOOLS;

#ifdef _DEBUG
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
#endif // _DEBUG

	gameData.window.Focus();
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(gameData.window.GetWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// Initialize variables
	float lastFPSCalculationTime = 0.0f;
	int frameCount = 0;

	while (gameData.window.IsActive())
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		gameData.dt = currentFrame - lastFrame;
		lastFrame = currentFrame;

		frameCount++;

		// Calculate elapsed time since last FPS calculation
		float elapsedTime = currentFrame - lastFPSCalculationTime;

		if (elapsedTime >= 1.0f) // If one second has passed
		{
			// Calculate FPS
			int fps = frameCount / elapsedTime;

			// Reset frame count and timer
			frameCount = 0;
			lastFPSCalculationTime = currentFrame;

			// Update window title with FPS (optional)
			std::string title = "FPS: " + std::to_string(fps);
			gameData.window.SetTitle(title.c_str());
		}

		gameData.gameStates[gameData.gameState](&gameData);
		gameData.window.Events();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	system("pause");
	return 0;
}
