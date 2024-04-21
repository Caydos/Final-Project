#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"
#include <string>
#include "Clock.h"

namespace Sets
{
	class Set;
}

namespace Animations
{
	class Movement
	{
	public:
		Movement();
		~Movement();

		glm::vec3 GetOffset();
		void SetOffset(glm::vec3 _offset);

		glm::vec3 GetRotation();
		void SetRotation(glm::vec3 _rotation);

		Sets::Set* GetObject();
		void SetObject(Sets::Set* _object);

		void Update(float _elapsedTime, float _duration);
	private:
		glm::vec3 offset;
		glm::vec3 rotation;
		Sets::Set* object;
	};
	class Sequence
	{
	public:
		Sequence();
		~Sequence();

		std::string GetName();
		void SetName(std::string _name);

		float GetDuration();
		void SetDuration(float _duration);


		std::vector<Movement>* GetMovements();
	private:
		std::string name;
		float duration = 0.0f;
		std::vector<Movement> movements;
	};

	class Animation
	{
	public:
		Animation();
		~Animation();

		Clock* AccessClock();
		std::string GetName();
		void SetName(std::string _name);

		std::vector<Sequence>* GetSequences();

		float GetDuration();
		void CalculateDuration();

		void Update();
	private:
		Clock clock;
		std::string name;
		std::vector<Sequence> sequences;
		float duration = 0.0f;
	};


	namespace UI
	{
		void MenuPopUps(GameData* _gameData);
		void Menu(GameData* _gameData);
		void Bar(GameData* _gameData);
	}
}

#endif

