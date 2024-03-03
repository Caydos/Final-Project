#ifndef SSAO_H
#define SSAO_H
#include "Common.h"

namespace SSAO
{
	void RenderQuad();
	void Tick(GameData* _gameData, unsigned int _gPosition, unsigned int _gNormal);
	void BindTexture();
}

#endif // !SSAO_H