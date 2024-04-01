#ifndef LIGHT_MENU_H
#define LIGHT_MENU_H
#include "Common.h"
#include "Colors.h"
#include "Editor.h"
#include "Lighting.h"

namespace LightMenu
{
	void Basic(GameData* _gameData);
	void Object(GameData* _gameData, Lighting::Light* _light);
	void CleanUp();
}

#endif // !1
