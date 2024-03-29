#include "LoadingScreen.h"
#include "Sprite.h"

static Sprite background;
static Sprite spinner;
static float spinnerSize = 200;
static bool initialized = false;

void LoadingScreen::Render(GameData* _gameData)
{
	if (!initialized)
	{
		background.Load("../Textures/Tableau.jpg", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0f), Colors::Blue, 1);
		spinner.Load("../Textures/LogoStudio.png", glm::vec3(_gameData->resolution[0] - spinnerSize, _gameData->resolution[1] - spinnerSize,0.0), glm::vec3(spinnerSize, spinnerSize, 0.0f), Colors::Blue, 1);
		initialized = true;
	}
	background.Draw();
	spinner.Draw();
}
