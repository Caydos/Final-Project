#ifndef MONSTER_H
#define MONSTER_H

#include "Common.h"
#include "Peds.h"
#include "Physics.h"
#include "Clock.h"
#include "Player.h"
#include "Maze.h"

namespace Monster
{
    class Monster
    {
    public:
        Monster();
        ~Monster();

        void Update(GameData* _gameData);
        Peds::Ped* GetPed();
        void SetPed(Peds::Ped* _ped);
        void SetTargetPlayer(Players::Player* _player);

    private:
        Peds::Ped* ped;
        Clock movementClock;
        Players::Player* targetPlayer = nullptr;
        float directionX = 0.0f; 
        float directionZ = 0.0f; 
        bool changeInX = true; //Changement de direction soit X (true) ou Z (false)
       
    };

    Monster* Create();
    void Erase(Monster* monster);
    std::vector<Monster*>* GetPool();
}

#endif // MONSTER_H
