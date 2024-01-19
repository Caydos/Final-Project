#include "Map.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <nlohmann/json.hpp>
#include "DefaultVertices.h"

Map::Map currentMap;
using json = nlohmann::json;
Shaders::Shader* shader = nullptr;

void Map::Map::InsertTexture(Texture _texture)
{
	if (currentMap.textures != nullptr)
	{
		Texture* tempObjects = new Texture[currentMap.textureCount + 1];
		std::memcpy(tempObjects, currentMap.textures, currentMap.textureCount * sizeof(Texture));
		delete[] currentMap.textures;
		currentMap.textures = tempObjects;
	}
	else
	{
		currentMap.textures = new Texture[1];
	}
	currentMap.textures[currentMap.textureCount] = _texture;
	currentMap.textureCount++;
}

void Map::Map::InsertObject(Cube _object)
{
	if (currentMap.objects != nullptr)
	{
		Cube* tempObjects = new Cube[currentMap.objectCount + 1];
		std::memcpy(tempObjects, currentMap.objects, currentMap.objectCount * sizeof(Cube));
		delete[] currentMap.objects;
		currentMap.objects = tempObjects;
	}
	else
	{
		currentMap.objects = new Cube[1];
	}
	currentMap.objects[currentMap.objectCount] = _object;
	currentMap.objectCount++;
}

void Map::Map::SetFromFile(const char* _fileName, Shaders::Shader* _shader)
{
	std::string path = MAP_DIRECTORY;
	path += _fileName;
	std::ifstream mapFile(path);

	char* content = nullptr;
	while (mapFile.good())
	{
		std::string line;
		std::getline(mapFile, line);
		size_t lineLength = strlen(line.c_str());
		if (content != nullptr)
		{
			size_t oldSize = strlen(content);
			size_t lineSize = strlen(line.c_str());
			char* tempBuffer = new char[oldSize + lineSize + 1];
			std::memcpy(tempBuffer, content, oldSize);
			std::memcpy(tempBuffer + oldSize, line.c_str(), lineSize);
			tempBuffer[oldSize + lineSize] = '\0';
			delete[] content;

			content = tempBuffer;
		}
		else
		{
			content = new char[lineLength + 1];
			std::memcpy(content, line.c_str(), lineLength);
			content[lineLength] = '\0';
		}
	}
	mapFile.close();

	json jsonParsed = json::parse(content);
	for (auto& object : jsonParsed["textures"])
	{
		std::string path = TEXTURES_DIRECTORY;
		path += object;
		Texture texture;
		texture.LoadFromFile(path.c_str());
		currentMap.InsertTexture(texture);
	}
	//for (auto& object : jsonParsed["objects"])
	//{
	//	std::string typeStr = object["type"];
	//	if (strcmp(typeStr.c_str(), "cube") == 0)
	//	{
	//		Cube obj;
	//		obj.position = { object["position"]["x"],object["position"]["y"],object["position"]["z"] };
	//		obj.rotation = { object["rotation"]["x"],object["rotation"]["y"],object["rotation"]["z"] };
	//		obj.scale = { object["size"]["x"],object["size"]["y"],object["size"]["y"] };
	//		obj.texture = &currentMap.textures[object["textureId"]];
	//		obj.durability = object["durability"];
	//		obj.BindShader(_shader);
	//		obj.SetTexture(obj.texture);
	//		currentMap.InsertObject(obj);
	//	}
	//}

	delete[] content;
}

void Map::Map::CleanUp(void)
{
	this->id = -1;
	this->objectCount = 0;
	if (this->objects != nullptr)
	{
		delete[] this->objects;
	}
	if (this->textures != nullptr)
	{
		delete[] this->textures;
	}
}

namespace fs = std::filesystem;
void Map::GenerateRandomly(Shaders::Shader* _shader)
{
	shader = _shader;
	int mapCount = 0;
	std::string path = MAP_DIRECTORY; // Replace with your folder path
	std::vector<std::string> mapsPaths;

	try {
		if (fs::exists(path) && fs::is_directory(path))
		{
			for (const auto& entry : fs::directory_iterator(path))
			{
				mapCount++;
				mapsPaths.push_back(entry.path().filename().generic_string());
			}
		}
	}
	catch (const fs::filesystem_error& err)
	{
		std::cerr << "[Map::GenerateRandomly] - Filesystem error: " << err.what() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "[Map::GenerateRandomly] - Standard exception: " << e.what() << std::endl;
	}

	currentMap.CleanUp();

	int mapId = rand() % mapCount;
	currentMap.name = mapsPaths[mapId];
	currentMap.id = mapId;
	currentMap.SetFromFile(currentMap.name.c_str(), _shader);
}

void Map::Render()
{
	for (unsigned int objId = 0; objId < currentMap.objectCount; objId++)
	{
		Cube& obj = currentMap.objects[objId];
		obj.Draw();
	}
}

Map::Map Map::Get()
{
	return currentMap;
}

