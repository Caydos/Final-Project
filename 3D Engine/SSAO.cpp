#include "Common.h"
#include "SSAO.h"
#include <glad/glad.h>
#include <random>
#include "Scene.h"

static bool initialized = false;

void SSAO::Tick(GameData* _gameData, unsigned int _gPosition, unsigned int _gNormal)
{
    if (!initialized)
    {

        initialized = true;
    }

}

void SSAO::BindTexture()
{

}
