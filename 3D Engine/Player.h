#ifndef PLAYER_H
#define PLAYER_H
#include "Peds.h"
#include "Physics.h"
namespace Players
{
	class Player
	{
	public:
		Player();
		~Player();

		void Control(GameData* _gameData);

		Peds::Ped* GetPed();
		void SetPed(Peds::Ped* _ped);

	private:
		//int serverId;
		Peds::Ped* ped;
		
	};
	Player* Create();
	void Erase(Player* _player);
	std::vector<Player*>* GetPool();
}

#endif // !PLAYER_H