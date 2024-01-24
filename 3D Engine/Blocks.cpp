#include "Blocks.h"
#include "Files.h"

static std::vector<Blocks::BlockType> blocks;

void Blocks::Load()
{
	std::vector<std::string> materialsPaths = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	blocks.resize(materialsPaths.size());
	for (size_t materialId = 0; materialId < blocks.size(); materialId++)
	{
		Blocks::BlockType block;
		std::vector<std::string> files = Files::GetAllAtPath((std::string(BLOCKS_DIRECTORY) + materialsPaths[materialId]).c_str());
		size_t lastindex = materialsPaths[materialId].find_last_of(".");

		std::string extension = materialsPaths[materialId].substr(lastindex);
		if (extension == BLOCKS_FILE_EXTENSION)
		{
			std::string rawName = materialsPaths[materialId].substr(0, lastindex);
			std::string fileContent = Files::GetFileContent((std::string(BLOCKS_DIRECTORY) + materialsPaths[materialId]).c_str());
			json content;
			try
			{
				content = json::parse(fileContent);



			}
			catch (nlohmann::json::parse_error& e)
			{
				std::cerr << "JSON parsing error: " << e.what() << '\n';
				Logger::Write("Failed to parse Json file : ", rawName, "\n");
				continue;
			}

			block.SetName(rawName);
			blocks.push_back(block);
		}
	}
}

void Blocks::Refresh()
{
}
