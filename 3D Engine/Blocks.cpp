#include "Blocks.h"
#include "Files.h"

static std::vector<Blocks::BlockType> blocks;

void Blocks::Initialize()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < blocks.size(); folderId++)
	{
		Load(folders[folderId]);
	}
}

void Blocks::Load(std::string _name)
{
	Blocks::BlockType block;
	std::vector<std::string> files = Files::GetAllAtPath((std::string(BLOCKS_DIRECTORY) + _name).c_str());
	for (size_t fileId = 0; fileId < files.size(); fileId++)
	{
		size_t lastindex = files[fileId].find_last_of(".");
		std::string extension = files[fileId].substr(lastindex);

		if (extension == BLOCKS_FILE_EXTENSION)
		{
			std::string rawName = files[fileId].substr(0, lastindex);
			std::string fileContent = Files::GetFileContent((std::string(BLOCKS_DIRECTORY) + _name + "/" + files[fileId]).c_str());
			json content;
			try
			{
				content = json::parse(fileContent);
				std::vector<float> vertices;
				if (content.contains("attributes"))
				{
					if (content["attributes"].contains("lightDependent"))
					{
						block.SetLightDependency(content["attributes"]["lightDependent"]);
					}
					if (content["attributes"].contains("diffuse"))
					{
						block.SetDiffuse(glm::vec3(content["attributes"]["diffuse"][0],
							content["attributes"]["diffuse"][1],
							content["attributes"]["diffuse"][2]));
					}
					if (content["attributes"].contains("specular"))
					{
						block.SetSpecular(glm::vec3(content["attributes"]["specular"][0],
							content["attributes"]["specular"][1],
							content["attributes"]["specular"][2]));
					}
					if (content["attributes"].contains("shininess"))
					{
						block.SetShininess(content["attributes"]["shininess"]);
					}
				}
				if (content.contains("vertices"))
				{
					for (const auto& vertex : content["vertices"])
					{
						vertices.push_back(vertex);
					}
				}
				block.GenerateGraphicsBuffers();
				block.SetVertices(&vertices);
				block.BindGraphicsBuffers();
			}
			catch (nlohmann::json::parse_error& e)
			{
				std::cerr << "JSON parsing error: " << e.what() << '\n';
				Logger::Write("Failed to parse Json file : ", rawName, "\n");
				continue;
			}
		}
		else if (extension == BLOCKS_TEXTURE_EXTENSION)
		{
			Texture* texture = new Texture;
			texture->LoadFromFile((std::string(BLOCKS_DIRECTORY) + _name + "/" + _name).c_str());
			block.SetTexture(texture);
		}
	}

	block.SetName(_name);
	blocks.push_back(block);
}

void Blocks::Refresh()
{
	std::vector<std::string> folders = Files::GetAllAtPath(BLOCKS_DIRECTORY);
	for (size_t folderId = 0; folderId < blocks.size(); folderId++)
	{
		for (size_t blockId = 0; blockId < blocks.size(); blockId++)
		{
			if (folders[folderId] == blocks[blockId].GetName())
			{
				blocks[blockId].GetTexture()->Refresh();
				continue;
			}
		}
		Load(folders[folderId]);
	}
}




















#pragma region Definition
Blocks::Block::Block() {}
Blocks::Block::~Block() {}

glm::vec3 Blocks::Block::GetPosition() { return this->position; }
void Blocks::Block::SetPosition(float _x, float _y, float _z)
{
	this->position = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetPosition(glm::vec3 _position)
{
	this->position = _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Move(glm::vec3 _position)
{
	this->position += _position;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Move(float _x, float _y, float _z)
{
	this->position.x += _x;
	this->position.y += _y;
	this->position.z += _z;

	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}

glm::vec3 Blocks::Block::GetRotation() { return this->rotation; }
void Blocks::Block::SetRotation(float _x, float _y, float _z)
{
	this->rotation = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetRotation(glm::vec3 _rotation)
{
	this->rotation = _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Rotate(glm::vec3 _rotation)
{
	this->rotation += _rotation;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Rotate(float _x, float _y, float _z)
{
	this->rotation.x += _x;
	this->rotation.y += _y;
	this->rotation.z += _z;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}

glm::vec3 Blocks::Block::GetScale() { return this->scale; }
void Blocks::Block::SetScale(float _x, float _y, float _z)
{
	this->scale = glm::vec3(_x, _y, _z);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetScale(float _scale)
{
	this->scale = glm::vec3(_scale, _scale, _scale);
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::SetScale(glm::vec3 _scale)
{
	this->scale = _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Scale(glm::vec3 _scale)
{
	this->scale += _scale;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
void Blocks::Block::Scale(float _x, float _y, float _z)
{
	this->scale.x += _x;
	this->scale.y += _y;
	this->scale.z += _z;
	if (this->type != nullptr)
	{
		this->type->AskForRefresh();
	}
}
#pragma endregion


