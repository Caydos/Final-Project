#include "LoadingScreen.h"
#include "Sprite.h"


static Sprite background;
static Sprite spinner;
static AnimationM animationSpinner;
static Clock tempClock;
static Texture* textureSpin;
static float spinnerSize = 200;
static bool initialized = false;

void LoadingScreen::Render(GameData* _gameData)
{
    if (!initialized)
    {
        background.Load("../Textures/LoadingScreen/Tableau.jpg", glm::vec3(0.0), glm::vec3(_gameData->resolution[0], _gameData->resolution[1], 0.0f), 1);
        spinner.Load("", glm::vec3(_gameData->resolution[0] - spinnerSize, _gameData->resolution[1] - spinnerSize, 0.0), glm::vec3(spinnerSize, spinnerSize, 0.0f), 1);
        textureSpin = new Texture;
        textureSpin->LoadFromFile("../Textures/LoadingScreen/image0000.png");
        animationSpinner.Load(1, "../Textures/LoadingScreen/image0000.png",
            "../Textures/LoadingScreen/image0001.png", 
            "../Textures/LoadingScreen/image0002.png", 
            "../Textures/LoadingScreen/image0003.png", 
            "../Textures/LoadingScreen/image0004.png", 
            "../Textures/LoadingScreen/image0005.png", 
            "../Textures/LoadingScreen/image0006.png", 
            "../Textures/LoadingScreen/image0007.png", 
            "../Textures/LoadingScreen/image0008.png", 
            "../Textures/LoadingScreen/image0009.png", 
            "../Textures/LoadingScreen/image0010.png", 
            "../Textures/LoadingScreen/image0011.png", 
            "../Textures/LoadingScreen/image0012.png", 
            "../Textures/LoadingScreen/image0013.png", 
            "../Textures/LoadingScreen/image0014.png", 
            "../Textures/LoadingScreen/image0015.png", 
            "../Textures/LoadingScreen/image0016.png", 
            "../Textures/LoadingScreen/image0017.png", 
            "../Textures/LoadingScreen/image0018.png", 
            "../Textures/LoadingScreen/image0019.png", 
            "../Textures/LoadingScreen/image0020.png", 
            "../Textures/LoadingScreen/image0021.png", 
            "../Textures/LoadingScreen/image0022.png");
        spinner.SetTexture(textureSpin);

        tempClock.Restart();


        initialized = true;
    }

    animationSpinner.Animate(&spinner);

    background.Draw();
    spinner.Draw();
}