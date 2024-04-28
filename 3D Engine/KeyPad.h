#ifndef KEYPAD_H
#define KEYPAD_H
#include "Common.h"

namespace KeyPad
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);
	void CleanUp();

	void Interaction();
}

#endif // !KEYPAD_H