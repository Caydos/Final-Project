#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "Common.h"
#include "Scripting.h"


namespace Hospital
{
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	void CleanUp(GameData* _gameData);


	void KeyPad(GameData* _gameData);
}

#endif // !HOSPITAL_H