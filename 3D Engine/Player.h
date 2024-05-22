#ifndef PLAYER_H
#define PLAYER_H
#include "Peds.h"
#include "Physics.h"
#include "Clock.h"

namespace Players
{
	class Player
	{
	public:
		Player();
		~Player();

		void SetFootStepSound(Audio::Sound* _sound);
		void SetFootStepSound2(Audio::Sound* _sound);
		void SetFastBreathing(Audio::Sound* _sound);
		void Control(GameData* _gameData);

		Peds::Ped* GetPed();
		void SetPed(Peds::Ped* _ped);
	private:
		//int serverId;
		Peds::Ped* ped;
		Clock jumpClock;
		Audio::Sound* footSteps;
		Audio::Sound* footSteps2;
		Audio::Sound* fastBreathing;

	};
	Player* Create();
	void Erase(Player* _player);
	std::vector<Player*>* GetPool();
}

#endif // !PLAYER_H