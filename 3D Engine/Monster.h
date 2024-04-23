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
        void SetVelocity(glm::vec3 _velocity);
        void SetDirection(int _direction);

    private:
        Peds::Ped* ped;
        Clock movementClock;
        Players::Player* targetPlayer = nullptr;
        glm::vec3 velocity;
        int direction;

    };

    Monster* Create();
    void Erase(Monster* monster);
    std::vector<Monster> GetPool();
}

#endif // MONSTER_H