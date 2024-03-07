#ifndef BLOCKS_H
#define BLOCKS_H
#include "Common.h"
#include "Texture.h"
#include "RayCasting.h"

#define BLOCKS_DIRECTORY "../Blocks/"
#define BLOCKS_FILE_EXTENSION ".json"
#define BLOCKS_TEXTURE_EXTENSION ".jpg"


namespace Blocks
{
	class Instance
	{
	public:
		Instance();
		~Instance();

		void* GetId();
		void SetId(void* _id);

		void GenerateGraphicsBuffers();
		void RemoveGraphicsBuffers();

		void BindGraphicsBuffers();

		void SetVertices();
		void RegenerateMatrices();



		void InsertModel(glm::mat4* _model);

		void RemoveModel(glm::mat4* _model);

		void Draw();

		void AskForRefresh();
		void SetVisibility(bool _visible);
	private:
		void* id;
		bool graphicsLoaded;
		unsigned int VAO;
		unsigned int vertexVBO;
		unsigned int instanceVBO;

		std::vector<glm::mat4> models;
		std::vector<glm::mat4*> modelsAddresses;

		bool refreshInQueue;
		bool visible;
	};
	class BlockType
	{
	public:
		BlockType();
		~BlockType();

		std::string GetName();
		void SetName(std::string _name);


		Shaders::Shader* GetShader();
		void SetShader(Shaders::Shader* _shader);

		Texture* GetTexture();
		void SetTexture(Texture* _texture);

		Texture* GetEffectsTexture();
		void SetEffectsTexture(Texture* _effectsTexture);

		Colors::Color GetColor();
		void SetColor(Colors::Color _color);


		glm::vec3 GetScale();
		void SetScale(glm::vec3 _scale);

		bool GetLightDependency();
		void SetLightDependency(bool _lightDependency);

		glm::vec3 GetDiffuse();
		void SetDiffuse(glm::vec3 _diffuse);

		glm::vec3 GetSpecular();
		void SetSpecular(glm::vec3 _specular);

		float GetShininess();
		void SetShininess(float _shininess);


		Instance* AddInstance(void* _instanceId);
		void RemoveInstance(void* _instance);
		void SetInstanceVisibility(void* _instanceId, bool _visible);
		void InsertInInstance(void* _instanceId, glm::mat4* _model);
		void RemoveFromInstance(void* _instanceId, glm::mat4* _model);
		void AskForRefresh(void* _instanceId);

		void Draw();

	private:
		std::string name;

		Shaders::Shader* shader;
		Texture* texture;
		Texture* effects;
		Colors::Color color;

		glm::vec3 scale;
		bool lightDependent;
		glm::vec3 diffuse;
		glm::vec3 specular;
		float shininess;

		std::vector<Instance*> instances;
	};

	class Block
	{
	public:
		Block();
		~Block();

		BlockType* GetType();
		void SetType(BlockType* _type);

		glm::mat4* GetParent();
		void SetParent(glm::mat4* _parent);

		glm::mat4 GetModel();
		glm::mat4* GetModelAddress();
		void GenerateModel();
		void EraseModel();

		void ApplyTransformation();

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
		bool inScene;

		BlockType* type;
		glm::mat4* parent;
		glm::mat4* model;


		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;
	};

	void Initialize();
	void Load(std::string _name);
	void MaterialCheck(Block* _block, const char* _materialName);
	void Refresh();
	void Draw();
	std::vector<Blocks::BlockType*> GetAll();

	namespace Ghost
	{
		enum Axis
		{
			X,
			Y,
			Z
		};
		void Draw(GameData* _gameData);
		void CalculateTransformations();
		void SetScale(glm::vec3 _scale);
		void SetStartPosition(glm::vec3 _position);
		void SetRay(RayCasting::Ray _ray);
		void RestrictAxis(Axis _axis, int _value);
		std::vector<glm::mat4> GetModels();
	}
}

#endif // !BLOCKS_H
