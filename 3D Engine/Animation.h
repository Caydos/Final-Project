#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"
#include <string>

namespace Sets
{
	class Set;
}

namespace Animations
{
	class Sequence
	{
	public:
		Sequence();
		~Sequence();

	private:
		float duration;
	};

	class Animation
	{
	public:
		Animation();
		~Animation();


		std::string GetName();
		void SetName(std::string _name);

	private:
		std::string name;
		std::vector<Sequence> sequences;
		float duration;
	};


	namespace UI
	{
		void Menu(GameData* _gameData);
		void Bar(GameData* _gameData);
	}
}

#endif

