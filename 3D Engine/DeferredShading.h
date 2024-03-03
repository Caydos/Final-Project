#ifndef DEFERRED_SHADING_H
#define DEFERRED_SHADING_H
#include "Common.h"

namespace DeferredShading
{
	void Initialize(GameData* _gameData);
	void RenderQuad();
	void Draw(GameData* _gameData);
}
#endif // !DEFERRED_SHADING_H