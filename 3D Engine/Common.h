#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <sstream>
#include <vector>
#include <shared_mutex>
#include <thread>
#include <chrono>
#define _USE_MATH_DEFINES
#include <cmath>

#include <IMGUI/imgui.h>
#include <IMGUI/backends/imgui_impl_opengl3.h>
#include <IMGUI/backends/imgui_impl_glfw.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

enum Gamestate
{
	MAIN_MENU,
	TOOLS,
	GAME,
};

#include "Camera.h"
#include "Window.h"
#include "Shader.h"
#include "Logger.h"
#include "Settings.h"

struct GameData
{
	float dt;
	Camera* camera;

	float resolution[2];
	Window window;

	Shaders::Shader* shaders[6];
	Shaders::Shader* actualShader;

	Gamestate gameState;
	void (*gameStates[5])(GameData* _gameData);

	Settings::Profile settings;
};

GameData* GetGameData();



#define bzero(b,len) (memset((b), '\0', (len)), (void) 0)  


#endif // !COMMON_H