#include "Blocks.h"
#include "Files.h"

static std::vector<Blocks::BlockType> blocks;

void Blocks::Load()
{
	std::vector<std::string> materialsPaths = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	blocks.resize(materialsPaths.size());
	for (size_t i = 0; i < blocks.size(); i++)
	{
		
	}
}

void Blocks::Refresh()
{
}
