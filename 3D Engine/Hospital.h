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

	void ClownUpdate(char** _args);
	void TeddyUpdate(char** _args);
}

#endif // !HOSPITAL_H