#include "Blocks.h"
#include "Files.h"

static std::vector<Blocks::BlockType*> blocks;

void Blocks::Initialize()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < folders.size(); folderId++)
	{
		Load(folders[folderId]);
	}
}

void Blocks::Load(std::string _name)
{
	Blocks::BlockType* block = new Blocks::BlockType;
	std::vector<std::string> files = Files::GetAllAtPath((std::string(BLOCKS_DIRECTORY) + _name).c_str());
	for (size_t fileId = 0; fileId < files.size(); fileId++)
	{
		size_t lastindex = files[fileId].find_last_of(".");
		std::string extension = files[fileId].substr(lastindex);
		std::string filename_without_extension = files[fileId].substr(0, lastindex);

		if (extension == BLOCKS_FILE_EXTENSION)
		{
			std::string rawName = files[fileId].substr(0, lastindex);
			std::string fileContent = Files::GetFileContent((std::string(BLOCKS_DIRECTORY) + _name + "/" + files[fileId]).c_str());
			json content;
			try
			{
				content = json::parse(fileContent);
				if (content.contains("attributes"))
				{
					if (content["attributes"].contains("scale"))
					{
						glm::vec3 scale(content["attributes"]["scale"]);
						block->SetScale(scale);
					}
					if (content["attributes"].contains("lightDependent"))
					{
						block->SetLightDependency(content["attributes"]["lightDependent"]);
					}
					if (content["attributes"].contains("diffuse"))
					{
						block->SetDiffuse(glm::vec3(content["attributes"]["diffuse"][0],
							content["attributes"]["diffuse"][1],
							content["attributes"]["diffuse"][2]));
					}
					if (content["attributes"].contains("specular"))
					{
						block->SetSpecular(glm::vec3(content["attributes"]["specular"][0],
							content["attributes"]["specular"][1],
							content["attributes"]["specular"][2]));
					}
					if (content["attributes"].contains("shininess"))
					{
						block->SetShininess(content["attributes"]["shininess"]);
					}
				}
			}
			catch (nlohmann::json::parse_error& e)
			{
				std::cerr << "JSON parsing error: " << e.what() << '\n';
				Logger::Write("Failed to parse Json file : ", rawName, "\n");
				continue;
			}
		}
		else if (extension == BLOCKS_TEXTURE_EXTENSION || extension == ".png")
		{
			Texture* texture = new Texture;
			texture->LoadFromFile((std::string(BLOCKS_DIRECTORY) + _name + "/" + files[fileId]).c_str());
			if (filename_without_extension == "tex")
			{
				block->SetTexture(texture);
			}
			else if (filename_without_extension == "map")
			{
				block->SetEffectsTexture(texture);
			}
		}
	}
	block->SetShader(GetGameData()->shaders[Shaders::GEOMETRY]);
	block->SetName(_name);
	blocks.push_back(block);
}

void Blocks::MaterialCheck(Block* _block, const char* _materialName)
{
	if (_materialName == nullptr)
	{
		if (blocks.size() > 0)
		{
			_block->SetType(blocks[0]);
		}
		return;
	}
	for (size_t i = 0; i < blocks.size(); i++)
	{
		if (blocks[i]->GetName() == _materialName)
		{
			_block->SetType(blocks[i]);
			return;
		}
	}
}

void Blocks::Refresh()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < blocks.size(); folderId++)
	{
		for (size_t blockId = 0; blockId < blocks.size(); blockId++)
		{
			if (folders[folderId] == blocks[blockId]->GetName())
			{
				blocks[blockId]->GetTexture()->Refresh();
				continue;
			}
		}
		Load(folders[folderId]);
	}
}

void Blocks::Draw()
{
	for (size_t i = 0; i < blocks.size(); i++)
	{
		blocks[i]->Draw();
	}
}

std::vector<Blocks::BlockType*> Blocks::GetAll()
{
	return blocks;
}
