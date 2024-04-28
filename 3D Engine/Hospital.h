#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "Common.h"
#include "Scripting.h"


namespace Hospital
{
	void RegisterInteractions();
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	void UnlockExit();
	void CleanUp(GameData* _gameData);
}

#endif // !HOSPITAL_H