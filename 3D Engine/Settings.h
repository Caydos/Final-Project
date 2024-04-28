#ifndef SETTINGS_H
#define SETTINGS_H
struct GameData;

#define SETTINGS_DIRECTORY "../"
#define SETTINGS_FILE_EXTENSION ".json"

namespace Settings
{
	struct Profile
	{
		float sentivity;
		float renderDistance;
	};
	void Load(GameData* _gameData);
	void Save(GameData* _gameData);
}

#endif // !SETTINGS_H