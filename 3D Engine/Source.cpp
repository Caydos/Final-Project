#include "Common.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <OpenAl/al.h>
#include <OpenAl/alc.h>


#include "Scene.h"
#include "Commands.h"
#include "Network.h"
#include "Audio.h"
#include "Scripting.h"
#include "Tools.h"
#include "Sprite.h"

float lastFrame = 0.0f;

GameData gameData;

GameData* GetGameData() { return &gameData; }
ALCdevice* device;
ALCcontext* context;

int main()
{
	//{//Audio
	//	// Initialize OpenAL device and context
	//	device = alcOpenDevice(NULL);
	//	if (!device)
	//	{
	//		return 0;
	//	}

	//	context = alcCreateContext(device, NULL);
	//	if (!context) {
	//		alcCloseDevice(device);
	//		return 0;
	//	}
	//	alcMakeContextCurrent(context);

	//	// Set Listener properties
	//	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f); // Position of the listener
	//	ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }; // Orientation (facing direction and up vector)
	//	alListenerfv(AL_ORIENTATION, orientation);

	//	// Set Distance Model (optional, depending on your needs)
	//	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
	//}
	std::thread audioThread(Audio::Tick);
	audioThread.detach();


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
	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
		// Get the primary monitor
	//GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	//// Get the video mode for the primary monitor
	//const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);


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
	gameData.gameStates[GAME] = &Scripting::Tick;
	gameData.gameState = GAME;

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
	ImGui_ImplOpenGL3_Init("#version 430 core");

	// Initialize variables
	float lastFPSCalculationTime = 0.0f;
	int frameCount = 0;

	while (gameData.window.IsActive())
	{
		// Set Listener properties
		alListener3f(AL_POSITION, gameData.camera->Position.x, gameData.camera->Position.y, gameData.camera->Position.z); // Position of the listener
		ALfloat orientation[] = { 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f }; // Orientation (facing direction and up vector)
		alListenerfv(AL_ORIENTATION, orientation);

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

		gameData.window.Clear();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		gameData.gameStates[gameData.gameState](&gameData);
		gameData.window.Events();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();

	//alcDestroyContext(context);
	//alcCloseDevice(device);

	system("pause");
	return 0;
}
