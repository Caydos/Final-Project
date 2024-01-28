#ifndef EDITOR_H
#define EDITOR_H
#include "Common.h"

#define SCENE_DIRECTORY "../Scenes/"
#define SCENE_FILE_EXTENSION ".json"

namespace Editor
{
	enum OutputFileType
	{
		SCENE,
		DECOR,
		MODEL,
		ENTITY,
		SET
	};
	void Initialize(GameData* _gameData);

	bool IsDisplayed();
	void SetDisplay(bool _status = true);

	void Menu(GameData* _gameData);
	void CleanUp();

	void Tick(GameData* _gameData);

	void Close();
}

#endif // !EDITOR_H