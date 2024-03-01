#ifndef SET_H
#define SET_H
#include "Common.h"
#include "Blocks.h"
#include "Bounds.h"
#include "Files.h"

#define SETS_DIRECTORY "../Sets/"
#define SETS_FILE_EXTENSION ".json"

namespace Sets
{
	class Set
	{
	public:
		Set();
		~Set();
		void Initialize();
		void Erase();
		void Save();
		void LoadFromJson(json _content);

		std::string GetName();
		void SetName(std::string _name);
		std::string GetPath();
		void SetPath(std::string _path);

		bool IsVisible();
		void CheckVisibility();
		void AppplyVisibility();

		void ApplyTransformation();

		glm::mat4 GetBone();


		void InsertBlock(Blocks::Block _block, bool _boundsCalculation = true);
		void RemoveBlock(Blocks::Block* _block);
		std::vector<Blocks::Block>* GetBlocks();
		
		void GenerateRenderingInstance();
		void EraseRenderingInstance();
		void SetRenderingInstance(std::vector<Blocks::BlockType*>* _instance);


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

		void CalculateBoundingBox();

		bool HasOrigin();
		void SetOriginBlockScale(float _scale = 0.2f);
		void PlaceOriginBlock(float _scale = 0.2f);

		void MoveOrigin(glm::vec3 _offset);
		void MoveOrigin(float _x, float _y, float _z);


		Bounds::Box GetBoundingBox();
	private:
		std::string name;
		std::string path;

		bool visible;
		Set* parent;
		glm::mat4* bone;
		std::vector<Set*> sets;

		std::vector<Blocks::Block> blocks;
		std::vector<Blocks::BlockType*>* typesInstances;
		bool useParentRendering;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Bounds::Box boundingBox;
	};
	void UpdateVisibility();
	Set* Create();
	void Erase(Set* _set);

	std::vector<Set*>* GetAll();

	void Menu(GameData* _gameData);
	void Edition(GameData* _gameData);
}

#endif // !SET_H