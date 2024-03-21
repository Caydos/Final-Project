#ifndef ANIMATION_H
#define ANIMATION_H

#include "Common.h"
#include <string>

namespace Sets
{
	class Set;
}

namespace Animation
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

	private:
		std::string name;
		std::vector<Sequence> sequences;
		float duration;
	};


	namespace UI
	{

		void SubMenu(GameData* _gameData);
		void Menu(GameData* _gameData);
	}
}

#endif

