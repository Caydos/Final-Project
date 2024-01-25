#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "Material.h"
//#include <glm/ext/matrix_float4x4.hpp>

#define BLOCKS_DIRECTORY "../Blocks/"
#define BLOCKS_FILE_EXTENSION ".json"
#define BLOCKS_TEXTURE_EXTENSION ".jpg"


namespace Blocks
{
	class BlockType
	{
	public:
		BlockType();
		~BlockType();

		void GenerateGraphicsBuffers();
		void RemoveGraphicsBuffers();

		void BindGraphicsBuffers();

		std::string GetName();
		void SetName(std::string _name);

		void SetVertices(std::vector<float>* _vertices);
		void RegenerateMatrices();



		void AddModel(glm::mat4* _model);

		void RemoveModel(glm::mat4* _model);





		Shaders::Shader* GetShader();
		void SetShader(Shaders::Shader* _shader);

		Texture* GetTexture();
		void SetTexture(Texture* _texture);

		Colors::Color GetColor();
		void SetColor(Colors::Color _color);

		bool GetLightDependency();
		void SetLightDependency(bool _lightDependency);

		glm::vec3 GetDiffuse();
		void SetDiffuse(glm::vec3 _diffuse);

		glm::vec3 GetSpecular();
		void SetSpecular(glm::vec3 _specular);

		float GetShininess();
		void SetShininess(float _shininess);
		void Draw();


		void AskForRefresh();

	private:

		bool graphicsLoaded;
		unsigned int VAO;
		unsigned int VBO;
		std::string name;

		std::vector<float> vertices;
		std::vector<glm::mat4> models;
		std::vector<glm::mat4*> modelsAddresses;


		Shaders::Shader* shader;
		Texture* texture;
		Colors::Color color;

		bool lightDependent;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;


		bool refreshInQueue;
	};

	class Block
	{
	public:
		Block();
		~Block();

		glm::vec3 GetPosition();
		void SetPosition(float _x, float _y, float _z);
		void SetPosition(glm::vec3 _position);

		void Move(glm::vec3 _position);
		void Move(float _x, float _y, float _z);

		glm::vec3 GetRotation();
		void SetRotation(float _x, float _y, float _z);
		void SetRotation(glm::vec3 _rotation);

		void Rotate(glm::vec3 _rotation);
		void Rotate(float _x, float _y, float _z);

		glm::vec3 GetScale();
		void SetScale(float _x, float _y, float _z);
		void SetScale(float _scale);
		void SetScale(glm::vec3 _scale);

		void Scale(glm::vec3 _scale);
		void Scale(float _x, float _y, float _z);

	private:
		BlockType* type;
		glm::mat4* parent;
		glm::mat4* model;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	void Initialize();
	void Load(std::string _name);
	void Refresh();
	void Draw();
}

#endif // !BLOCKS_H
