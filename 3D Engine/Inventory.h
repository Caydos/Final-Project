#ifndef INVENTORY_H
#define INVENTORY_H
#include "Common.h"
#include "Blocks.h"

namespace Inventory
{
	void Menu(GameData* _gameData);
	bool IsActive();
	Blocks::BlockType* GetHotBarBlock();
}

#endif // !INVENTORY_H