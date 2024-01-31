#ifndef SET_H
#define SET_H
#include "Common.h"
#include "Blocks.h"
#include "Bounds.h"

namespace Sets
{
	class Set
	{
	public:
		Set();
		~Set();
		void Initialize();
		void Erase();

		std::string GetName();
		void SetName(std::string _name);

		bool IsVisible();
		void CheckVisibility();
		void AppplyVisibility();

		void ApplyTransformation();



		std::vector<Blocks::Block> GetBlocks();

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

		void PlaceOriginBlock();

		void MoveOrigin(glm::vec3 _offset);
		void MoveOrigin(float _x, float _y, float _z);

	private:
		std::string name;

		bool visible;
		glm::mat4* parent;
		glm::mat4* bone;
		std::vector<Set> sets;
		std::vector<Blocks::Block> blocks;

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
	void Edition();
}

#endif // !SET_H