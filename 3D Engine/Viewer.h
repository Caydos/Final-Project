#ifndef VIEWER_H
#define VIEWER_H
#include "Common.h"

namespace Viewer
{
	void Initialize(GameData* _gameData);
	void Inputs(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();
}

#endif // !VIEWER_H