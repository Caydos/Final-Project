#ifndef TEXTURE_PICKER_H
#define TEXTURE_PICKER_H
#include "Common.h"
#include "Cube.h"
#include "Material.h"

namespace TexturePicker
{
	void Initialize(GameData* _gameData, std::vector<Material>* _materials);
	void Render(GameData* _gameData);
	void Inputs(GameData* _gameData);
	bool IsActive(void);
	void SetActive(bool _status = true);

	Cube* GetHotBarItem();
}

#endif // !TEXTURE_PICKER_H