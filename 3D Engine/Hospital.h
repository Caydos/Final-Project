#ifndef HOSPITAL_H
#define HOSPITAL_H
#include "Common.h"
#include "Scripting.h"


namespace Hospital
{
	enum SocketNames
	{
		BLUE,
		GREEN,
		RED,
		YELLOW
	};

	void RegisterInteractions();
	void Initialize(GameData* _gameData);
	void Tick(GameData* _gameData);

	bool* GetSocketsBool();
	void UnlockExit();
	bool GetCard();
	void CleanUp(GameData* _gameData);
}

#endif // !HOSPITAL_H