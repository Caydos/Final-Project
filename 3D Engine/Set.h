#ifndef SET_H
#define SET_H
#include "Common.h"
#include "Blocks.h"
#include "Bounds.h"
#include "Files.h"
#include "Animation.h"
#include "Collisions.h"

#define SETS_DIRECTORY "../Sets/"
#define SETS_FILE_EXTENSION ".json"

namespace Sets
{
	class Set
	{
	public:
		Set();
		~Set();
		void Initialize(bool _computeTransformation = true);
		void Erase();
		void Save();
		void LoadFromJson(json _content, bool _computeTransformation = true);

		std::string GetName();
		void SetName(std::string _name);
		std::string GetPath();
		void SetPath(std::string _path);

		bool IsVisible();
		void CheckVisibility();
		void AppplyVisibility();

		void ApplyTransformation();

		Set* GetParent();
		void SetParent(Set* _parent, bool _boundsCalculation = true);

		std::vector<Set*> GetChilds();
		void AddChild(Set* _child, bool _boundsCalculation = true);
		void RemoveChild(Set* _child, bool _boundsCalculation = true);
		glm::mat4 GetBone();


		void InsertBlock(Blocks::Block _block, bool _boundsCalculation = true);
		void RemoveBlock(Blocks::Block* _block);
		std::vector<Blocks::Block>* GetBlocks();
		
		void GenerateRenderingInstance();
		void EraseRenderingInstance();
		std::vector<Blocks::BlockType*>* GetRenderingInstance();
		void SetRenderingInstance(std::vector<Blocks::BlockType*>* _instance);


		glm::vec3 GetPosition();
		void SetPosition(float _x, float _y, float _z, bool _computeTransformation = true);
		void SetPosition(glm::vec3 _position, bool _computeTransformation = true);

		void Move(glm::vec3 _position, bool _computeTransformation = true);
		void Move(float _x, float _y, float _z, bool _computeTransformation = true);

		glm::vec3 GetRotation();
		void SetRotation(float _x, float _y, float _z, bool _computeTransformation = true);
		void SetRotation(glm::vec3 _rotation, bool _computeTransformation = true);

		void Rotate(glm::vec3 _rotation, bool _computeTransformation = true);
		void Rotate(float _x, float _y, float _z, bool _computeTransformation = true);

		glm::vec3 GetScale();
		void SetScale(float _x, float _y, float _z, bool _computeTransformation = true);
		void SetScale(float _scale, bool _computeTransformation = true);
		void SetScale(glm::vec3 _scale, bool _computeTransformation = true);

		void Scale(glm::vec3 _scale, bool _computeTransformation = true);
		void Scale(float _x, float _y, float _z, bool _computeTransformation = true);

		void CalculateBoundingBox();

		bool HasOrigin();
		void SetOriginBlockScale(float _scale = 0.2f);
		void PlaceOriginBlock(float _scale = 0.2f);

		void MoveOrigin(glm::vec3 _offset);
		void MoveOrigin(float _x, float _y, float _z);


		Bounds::Box GetBoundingBox();
		void DrawBoundingBox();

		glm::vec3 ComputeCollisions(Bounds::Box _boudingBox, glm::vec3 _velocity);
	private:
		std::string name;
		std::string path;

		bool visible = false;
		Set* parent = nullptr;
		std::vector<Set*> childs;
		glm::mat4* bone = nullptr;

		std::vector<Blocks::Block> blocks;
		std::vector<Blocks::BlockType*>* typesInstances = nullptr;
		bool useParentRendering = false;

		glm::vec3 position;
		glm::vec3 rotation;
		glm::vec3 scale;

		Bounds::BoundingBox boundingBox;

		std::vector<Animation::Animation> animations;
	};

	void UpdateVisibility();

	Set* Create();
	void Insert(Set* _set);
	void Erase(Set* _set);

	std::vector<Set*>* GetAll();
	std::vector<Set*> GetAllParents();

	bool ChildTree(GameData* _gameData, Set* _parentSet);
	bool SetTree(GameData* _gameData, Set* _set);

	void Menu(GameData* _gameData, float _yOffset);
	void Edition(GameData* _gameData, bool _allowControls = true);

	Set* GetEditedSet();
	void SetEditedSet(Set* _set);
}

#endif // !SET_H