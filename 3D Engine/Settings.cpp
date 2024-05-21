#include "Settings.h"
#include "Common.h"
#include "Files.h"
#include "Crosshair.h"

void Settings::Load(GameData* _gameData)
{
	std::string savedSettings = Files::GetFileContent((std::string(SETTINGS_DIRECTORY) + "Settings" + std::string(SETTINGS_FILE_EXTENSION)).c_str());

	json savedSettingsObj = json::parse(savedSettings);
	if (savedSettingsObj.contains("sensitivity"))
	{
		_gameData->settings.sentivity = savedSettingsObj["sensitivity"];
		_gameData->camera->MouseSensitivity = _gameData->settings.sentivity;
	}
	if (savedSettingsObj.contains("renderDistance"))
	{
		_gameData->settings.renderDistance = savedSettingsObj["renderDistance"];
	}
	if (savedSettingsObj.contains("crosshair"))
	{
		std::string path = savedSettingsObj["crosshair"];
		Crosshairs::Get()->LoadFromFile(path.c_str());
	}
	if (savedSettingsObj.contains("volume"))
	{
		_gameData->settings.volume = savedSettingsObj["volume"];
	}
}

void Settings::Save(GameData* _gameData)
{
	std::string savedSettings = Files::GetFileContent("../Settings.json");

	json savedSettingsObj = json::parse(savedSettings);
	savedSettingsObj["sensitivity"] = _gameData->settings.sentivity;
	savedSettingsObj["crosshair"] = Crosshairs::Get()->GetName();

	Files::Create(SETTINGS_DIRECTORY, "Settings", SETTINGS_FILE_EXTENSION, savedSettingsObj.dump().c_str());
}
