#ifndef MAP_H
#define MAP_H
#include "Common.h"
#include "Texture.h"
#include "Cube.h"

#define MAP_DIRECTORY "../Maps/"
#define TEXTURES_DIRECTORY "../Textures/Blocks/"

namespace Map
{
	struct Map
	{
		std::string name;
		int id;

		Texture* textures = nullptr;
		unsigned int textureCount;
		void InsertTexture(Texture _texture);

		Cube* objects;
		unsigned int objectCount;
		void InsertObject(Cube _object);

		void SetFromFile(const char* _fileName, Shaders::Shader* _shader);
		void CleanUp(void);
	};

	void GenerateRandomly(Shaders::Shader* _shader);
	void Render();
	Map Get();
}

#endif // !MAP_H