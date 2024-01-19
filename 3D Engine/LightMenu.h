#ifndef LIGHT_MENU_H
#define LIGHT_MENU_H
#include "Common.h"
#include "Colors.h"
#include "Editor.h"
#include "Lightning.h"

namespace LightMenu
{
	void Basic(GameData* _gameData);
	void Object(GameData* _gameData, Lightning::Light* _light);
	void CleanUp();
}

#endif // !1
