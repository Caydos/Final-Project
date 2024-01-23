#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "Material.h"
//#include <glm/ext/matrix_float4x4.hpp>

#define BLOCKS_DIRECTORY "../Blocks/"


namespace Blocks
{
	class BlockType
	{
	public:
		BlockType()
		{
			this->graphicsLoaded = false;
		}
		~BlockType() {}

		void GenerateGraphicsBuffers()
		{
			glGenVertexArrays(1, &this->VAO);
			glGenBuffers(1, &this->VBO);
			this->graphicsLoaded = true;
		}

		void Draw()
		{
			if (this->graphicsLoaded)
			{

			}
		}

		void RemoveGraphicsBuffers()
		{
			glDeleteBuffers(1, &this->VBO);
			glDeleteVertexArrays(1, &this->VAO);
			this->graphicsLoaded = false;
		}

	private:
		bool graphicsLoaded;
		std::string name;
		Material material;
		std::vector<glm::mat4> models;
		unsigned int VAO;
		unsigned int VBO;

		std::vector<float> vertices;
	};

	void Load();
	void Refresh();
}

#endif // !BLOCKS_H
